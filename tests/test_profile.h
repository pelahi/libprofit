/**
 * profile-wide tests
 *
 * ICRAR - International Centre for Radio Astronomy Research
 * (c) UWA - The University of Western Australia, 2016
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

#include <cxxtest/TestSuite.h>

#include "profit/profit.h"

using namespace profit;

class TestProfile : public CxxTest::TestSuite {

private:

	void test_params_i(const std::string &profile_name,
	                   const std::vector<std::string> &unknown_names,
	                   const std::vector<std::string> &bool_names,
	                   const std::vector<std::string> &uint_names,
	                   const std::vector<std::string> &double_names) {

		profit::Model m;
		auto &prof = m.add_profile(profile_name);

		// invalid parameters
		for(auto name: unknown_names) {
			TS_ASSERT_THROWS(prof.parameter(name, true), const invalid_parameter &);
			TS_ASSERT_THROWS(prof.parameter(name, 1u), const invalid_parameter &);
			TS_ASSERT_THROWS(prof.parameter(name, 1.), const invalid_parameter &);
		}

		// valid boolean parameters
		for(auto name: bool_names) {
			prof.parameter(name, true);
			TS_ASSERT_THROWS(prof.parameter(name, 1u), const invalid_parameter &);
			TS_ASSERT_THROWS(prof.parameter(name, 1.), const invalid_parameter &);
		}

		// valid uint parameters
		for(auto name: uint_names) {
			TS_ASSERT_THROWS(prof.parameter(name, true), const invalid_parameter &);
			prof.parameter(name, 1u);
			TS_ASSERT_THROWS(prof.parameter(name, 1.), const invalid_parameter &);
		}

		// valid double parameters
		for(auto name: double_names) {
			TS_ASSERT_THROWS(prof.parameter(name, true), const invalid_parameter &);
			TS_ASSERT_THROWS(prof.parameter(name, 1u), const invalid_parameter &);
			prof.parameter(name, 1.);
		}

	}

	void test_radial_params(const std::string &profile_name) {
		test_params_i(profile_name, {"Xcen", "yCen", "magnitude", "axisrat"},
	              {"adjust", "rough"}, {"max_recursions", "resolution"},
	              {"xcen", "ycen", "mag", "ang", "axrat", "box", "acc", "rscale_switch", "rscale_max"});
	}

public:

	void test_brokenexp_parameters() {
		test_radial_params("brokenexp");
		test_params_i("brokenexp",
				{"b", "H1", "H2", "h3", "1h", "unknown"},
				{},
				{},
				{"a", "h1", "h2", "rb"});
	}

	void test_coresersic_parameters() {
		test_radial_params("coresersic");
		test_params_i("coresersic",
				{"v", "c", "Nser", "Re", "RE", "unknown"},
				{},
				{},
				{"a", "b", "nser", "rb", "re"});
	}

	void test_ferrer_parameters() {
		test_radial_params("ferrer");
		test_params_i("ferrer",
				{"v", "c", "rin", "Rout", "unknown"},
				{},
				{},
				{"a", "b", "rout"});
	}

	void test_king_parameters() {
		test_radial_params("king");
		test_params_i("king",
				{"b", "c", "r", "rg", "unknown"},
				{},
				{},
				{"a", "rc", "rt"});
	}

	void test_moffat_parameters() {
		test_radial_params("moffat");
		test_params_i("moffat",
				{"fmwh", "fwmh", "fmhw", "unknown"},
				{},
				{},
				{"con", "fwhm"});
	}

	void test_psf_parameters() {
		test_params_i("psf",
				{"mag1", "YCEN", "xcenter"},
				{},
				{},
				{"xcen", "ycen", "mag"});
	}
	void test_sky_parameters() {
		test_params_i("sky",
				{"bg2", "bg1", "BG", "unknown"},
				{},
				{},
				{"bg"});
	}

	void test_sersic_parameters() {
		test_radial_params("sersic");
		test_params_i("sersic",
				{"unknown", "RE", "nser "},
				{"rescale_flux"},
				{},
				{"nser", "re"});
	}
};