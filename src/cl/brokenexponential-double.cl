R"===(
/**
 * Double-precision Broken Exponential profile OpenCL kernel implementation for libprofit
 *
 * ICRAR - International Centre for Radio Astronomy Research
 * (c) UWA - The University of Western Australia, 2017
 * Copyright by UWA (in the framework of the ICRAR)
 * All rights reserved
 *
 * Contributed by Rodrigo Tobar
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

inline static double _d_broken_exponential(double r, double h1, double h2, double rb, double a) {

	/*
	 * See brokenexponential.cpp for an explanation about this
	 */
	double base = r - rb;
	double expo = 1 / h1 - 1 / h2;
	if (a * base < 40) {
		base = log(1 + exp(a * base)) / a;
	}

	return exp(-r / h1 + expo * base);
}

inline double d_evaluate_brokenexp(double x, double y, double box, double h1, double h2, double rb, double a) {
	private double r = pow(pow(fabs(x), 2+box) + pow(fabs(y), 2+box), 1/(2+box));
	return _d_broken_exponential(r, h1, h2, rb, a);
}

kernel void brokenexp_double(
	global double *image,
	global d_point_t *to_subsample,
	int width, int height,
	int rough,
	double scale_x, double scale_y,
	double xcen, double ycen,
	double cos_ang, double sin_ang, double axrat,
	double rscale, double rscale_switch, double rscale_max,
	double box, double h1, double h2, double rb, double a) {

	private int i = get_global_id(0);
	private double x = (i%width + 0.5)*scale_x;
	private double y = (i/width + 0.5)*scale_y;

	// image to profile coordinate conversion
	private double x_prof, y_prof;
	d_image_to_profile_coordiates(x, y, &x_prof, &y_prof, xcen, ycen, cos_ang, sin_ang, axrat);

	private double r_prof = sqrt(x_prof*x_prof + y_prof*y_prof);

	if( rscale_max > 0 && (r_prof/rscale) > rscale_max ) {
#if __OPENCL_C_VERSION__ <= 120
		image[i] = 0;
		to_subsample[i].x = -1;
#endif /* __OPENCL_C_VERSION__ */
	}
	else if( rough || (r_prof/rscale) > rscale_switch ) {
		image[i] = d_evaluate_brokenexp(x_prof, y_prof, box, h1, h2, rb, a);
#if __OPENCL_C_VERSION__ <= 120
		to_subsample[i].x = -1;
#endif /* __OPENCL_C_VERSION__ */
	}
	else {
#if __OPENCL_C_VERSION__ <= 120
		image[i] = 0;
#endif /* __OPENCL_C_VERSION__ */
		// subsample
		to_subsample[i].x = x;
		to_subsample[i].y = y;
	}

}

kernel void brokenexp_subsample_double(
	global d_ss_kinfo_t *kinfo,
	double acc,
	double xcen, double ycen,
	double cos_ang, double sin_ang, double axrat,
	double rscale, double rscale_switch, double rscale_max,
	double box, double h1, double h2, double rb, double a) {

	private int i = get_global_id(0);
	private d_ss_kinfo_t info = kinfo[i];
	private double x = info.point.x;
	private double y = info.point.y;

	// image to profile coordinate conversion
	// including delta_y_prof to test accuracy
	private double x_prof, y_prof;
	d_image_to_profile_coordiates(x, y, &x_prof, &y_prof, xcen, ycen, cos_ang, sin_ang, axrat);
	private double delta_y_prof = (-info.xbin * sin_ang + info.ybin * cos_ang)/axrat;

	private double val, testval;

	val = d_evaluate_brokenexp(x_prof, y_prof, box, h1, h2, rb, a);
	testval = d_evaluate_brokenexp(x_prof, fabs(y_prof) + fabs(delta_y_prof), box, h1, h2, rb, a);

	// no need for subsampling
	if( fabs(testval/val - 1.0) <= acc ) {
		kinfo[i].point.x = -1.;
		kinfo[i].point.y = -1.;
		kinfo[i].val = val;
	}
	// else we already have the correct coordinates for the next subsampling
	else {
		kinfo[i].val = 0;
	}

}
)==="
