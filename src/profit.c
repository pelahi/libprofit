/**
 * libprofit main entry-point routines
 *
 * ICRAR - International Centre for Radio Astronomy Research
 * (c) UWA - The University of Western Australia, 2016
 * Copyright by UWA (in the framework of the ICRAR)
 * All rights reserved
 *
 * Contributed by Aaron Robotham, Rodrigo Tobar
 *
 * This file is part of libprofit.
 *
 * libprofit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libprofit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libprofit.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "profit.h"
#include "sersic.h"
#include "sky.h"

struct _profit_profile_index {
	char *name;
	profit_profile *(* create)(void);
};

static
struct _profit_profile_index _all_profiles[] = {
	{"sky",    profit_create_sky},
	{"sersic", profit_create_sersic},
	{NULL, NULL, NULL} // Sentinel
};

profit_profile* profit_get_profile(const char const * name) {

	struct _profit_profile_index *p = _all_profiles;
	while(1) {
		if( p->name == NULL ) {
			break;
		}
		if( !strcmp(name, p->name) ) {
			profit_profile *profile = p->create();
			profile->error = NULL;
			profile->name = strdup(name);
			return profile;
		}
		p++;
	}

	return NULL;
}

void profit_make_model(profit_model *model) {

	unsigned int i, j, p;

	if( !model->width ) {
		model->error = strdup("Model's width is 0");
		return;
	}
	else if( !model->height ) {
		model->error = strdup("Model's height is 0");
		return;
	}
	else if( !model->res_x ) {
		model->error = strdup("Model's res_x is 0");
		return;
	}
	else if( !model->res_y ) {
		model->error = strdup("Model's res_y is 0");
		return;
	}

	model->xbin = model->width/(double)model->res_x;
	model->ybin = model->height/(double)model->res_y;
	model->image = (double *)malloc(sizeof(double) * model->width * model->height);
	if( !model->image ) {
		char *msg = "Cannot allocate memory for image with w=%u, h=%u";
		model->error = (char *)malloc( strlen(msg) - 4 + 20 ); /* 32bits unsigned max is 4294967295 (10 digits) */
		sprintf(model->error, msg, model->width, model->height);
		return;
	}

	/* Initialize all profiles. Each profile can fail during initialization
	 * in which case we don't proceed any further */
	for(p=0; p < model->n_profiles; p++) {
		profit_profile *profile = model->profiles[p];
		profile->init_profile(profile, model);
		if( profile->error ) {
			/* TODO: proper clean up */
			return;
		}
	}

	/*
	 * Generate a separate image for each profile.
	 *
	 * We optionally use OpenMP to parallelize this per-profile image
	 * generation. Depending on how many there are we might get a speed up, so
	 * probably we should study what is the best way to go here (e.g.,
	 * parallelize only if we have more than 2 or 3 profiles)
	 */
	double **profile_images = (double **)malloc(sizeof(double *) * model->n_profiles);
#if _OPENMP
	#pragma omp parallel for private(p)
#endif
	for(p=0; p < model->n_profiles; p++) {
		profit_profile *profile = model->profiles[p];
		profile_images[p] = (double *)malloc(sizeof(double) * model->width * model->height);
		profile->make_profile(profile, model, profile_images[p]);
	}

	/* Sum up all results and free individual profile images */
	memset(model->image, 0, sizeof(double) * model->width * model->height);
	for(p=0; p != model->n_profiles; p++) {
		for(i=0; i != model->width; i++) {
			for(j=0; j != model->height; j++) {
				model->image[j*model->width + i] += profile_images[p][j*model->width + i];
			}
		}
		free(profile_images[p]);
	}
	free(profile_images);

}

void profit_cleanup(profit_model *m) {

	unsigned int i;

	if( m->error ) {
		free(m->error);
	}
	for(i=0; i!=m->n_profiles; i++) {
		free(m->profiles[i]);
	}
	free(m->profiles);
	free(m->image);
	free(m);
}
