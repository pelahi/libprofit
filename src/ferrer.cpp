/**
 * Ferrer profile implementation
 *
 * ICRAR - International Centre for Radio Astronomy Research
 * (c) UWA - The University of Western Australia, 2016
 * Copyright by UWA (in the framework of the ICRAR)
 * All rights reserved
 *
 * Contributed by Aaron Robotham, Dan Taranu, Rodrigo Tobar
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

#include <cmath>
#include <algorithm>

#include "profit/common.h"
#include "profit/exceptions.h"
#include "profit/ferrer.h"
#include "profit/utils.h"


using namespace std;

namespace profit
{

/**
 * The evaluation of the ferrer profile at ferrer coordinates (x,y).
 *
 * The ferrer profile has this form:
 *
 * (1-r_factor)^(a)
 *
 * where r_factor = (r/rscale)^(2-b)
 *              r = (x^{2+B} + y^{2+B})^{1/(2+B)}
 *              B = box parameter
 */
double FerrerProfile::evaluate_at(double x, double y, double r, bool reuse_r) const {

	double r_factor;
	if( reuse_r && box == 0 ) {
		r_factor = r;
	}
	else if( box == 0 ) {
		r_factor = sqrt(x*x + y*y);
	}
	else {
		double box = this->box + 2.;
		r_factor = pow( pow(abs(x), box) + pow(abs(y), box), 1./box);
	}

	r_factor /= rscale;
	if( r_factor < 1 ) {
		return pow(1 - pow(r_factor, 2 - b), a);
	}
	return 0;
}

void FerrerProfile::validate() {

	RadialProfile::validate();

	if ( rout <= 0 ) {
		throw invalid_parameter("rout <= 0, must have rout >= 0");
	}
	if ( a < 0 ) {
		throw invalid_parameter("a < 0, must have a >= 0");
	}
	if ( b > 2 ) {
		throw invalid_parameter("b > 2, must have b <= 2");
	}

}

eval_function_t FerrerProfile::get_evaluation_function() {
	return [](const RadialProfile &rp, double x, double y, double r, bool reuse_r) -> double {
		auto fp = static_cast<const FerrerProfile &>(rp);
		return fp.evaluate_at(x, y, r, reuse_r);
	};
}

double FerrerProfile::get_lumtot(double r_box) {

	/*
	 * Wolfram Alpha gave for g_factor:
	 *
	 *   G(a + 1) * G((4-b)/(2-b)) / G(a + 2/(2-b) + 1)
	 *
	 * But (4-b)/(2-b) == 1 + 2/(2-b), and G(a+1) == a*G(a)
	 * Thus the expression above equals to:
	 *
	 *   a * G(a) * G(1 + 2/(2-b)) / G(a + 2/(2-b) + 1)
	 *   a * B(a, 1+2/(2-b))
	 *
	 * Although the results are the same, high-level mathematical libs
	 * like GSL and R handle calculation errors better than we do and
	 * thus it's better to let them deal with intermediate results.
	 * For example b=1.99 gives NaN with our gamma-based calculations,
	 * but still converges using beta.
	 */
	double g_factor = a * beta(a, 1 + 2/(2-b));
	return pow(rout, 2) * M_PI * g_factor * axrat/r_box;
}

double FerrerProfile::adjust_rscale_switch() {
	return 0.5;
}

double FerrerProfile::adjust_rscale_max() {
	return 1;
}

double FerrerProfile::get_rscale() {
	return this->rout;
}

double FerrerProfile::adjust_acc() {
	return this->acc;
}

FerrerProfile::FerrerProfile(const Model &model, const string &name) :
	RadialProfile(model, name),
	rout(3), a(1), b(1)
{
	// this profile defaults to a different accuracy
	this->acc = 1;
}

bool FerrerProfile::parameter_impl(const string &name, double val) {

	if( RadialProfile::parameter_impl(name, val) ) {
		return true;
	}

	if( name == "rout" )   { rout = val; }
	else if( name == "a" ) { a = val; }
	else if( name == "b" ) { b = val; }
	else {
		return false;
	}

	return true;
}
} /* namespace profit */
