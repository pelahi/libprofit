/**
 * Image class tests
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

#include <cxxtest/TestSuite.h>
#include <list>

#include "profit/profit.h"

using namespace profit;

class Test2DCoordinates : public CxxTest::TestSuite {

public:

	void test_create() {
		TS_ASSERT_EQUALS(0, Point().x);
		TS_ASSERT_EQUALS(0, Point().y);
	}

	void test_equality() {
		TS_ASSERT_EQUALS(Point(), Point());
		TS_ASSERT_EQUALS(Point(1, 1), Point(1, 1));
		TS_ASSERT_EQUALS(Point(34, 890), Point(34, 890));

		TS_ASSERT_DIFFERS(Point(0, 0), Point(0, 1));
		TS_ASSERT_DIFFERS(Point(1, 0), Point(0, 1));
		TS_ASSERT_DIFFERS(Point(0, 1), Point(1, 0));
		TS_ASSERT_DIFFERS(Point(34, 890), Point(34, 89));
	}

	void test_copy() {

		Point p1(1, 1);
		Point p2(p1);
		Point p3 = p1;

		TS_ASSERT_EQUALS(p1, p2);
		TS_ASSERT_EQUALS(p2, p3);
		TS_ASSERT_EQUALS(p1, p3);
	}

	void test_move() {

		Point p1(1, 1);
		Point p2(std::move(p1));
		TS_ASSERT_DIFFERS(p1, p2);
		TS_ASSERT_EQUALS(0, p1.x);
		TS_ASSERT_EQUALS(0, p1.y);

		p1 = Point(0, 1);
		Point p3 = std::move(p1);
		TS_ASSERT_DIFFERS(p1, p3);
		TS_ASSERT_EQUALS(0, p1.x);
		TS_ASSERT_EQUALS(0, p1.y);
	}

	void test_sum() {
		Point p1(1, 10);
		Point p2(2, 20);
		TS_ASSERT_EQUALS(p1 + p2, Point(3, 30));
	}

	void test_sub() {
		Point p1(1, 10);
		Point p2(3, 30);
		TS_ASSERT_EQUALS(p2 - p1, Point(2, 20));
	}

	void test_multiply() {
		Point p1(1, 10);
		TS_ASSERT_EQUALS(p1 * 10, Point(10, 100));
		TS_ASSERT_EQUALS(p1 * 15, Point(15, 150));
	}

	void test_divide() {
		Point p1(100, 300);
		TS_ASSERT_EQUALS(p1 / 10, Point(10, 30));
		TS_ASSERT_EQUALS(p1 / 3, Point(33, 100));
	}

	void test_mixed_operations() {
		Point p = Point(34, 56);
		TS_ASSERT_EQUALS((p * 2 + p + p) / 4, p);
	}

};

class TestImage : public CxxTest::TestSuite {

private:
	void assert_empty(const Image &im) {
		TS_ASSERT_EQUALS(true, im.empty());
		TS_ASSERT_EQUALS(0, im.size());
		TS_ASSERT_EQUALS(0, im.getWidth());
		TS_ASSERT_EQUALS(0, im.getHeight());
		TS_ASSERT_EQUALS(0, im.size());
	}

	void assert_not_empty(const Image &im) {
		TS_ASSERT_DIFFERS(true, im.empty());
		TS_ASSERT_DIFFERS(0, im.size());
		TS_ASSERT_DIFFERS(0, im.getWidth());
		TS_ASSERT_DIFFERS(0, im.getHeight());
		TS_ASSERT_DIFFERS(0, im.size());
	}

public:

	void test_empty() {
		Image empty;
		assert_empty(empty);
	}

	void test_correct_dimensions() {
		Image im1;
		Image im2({1}, 1, 1);
		Image im3({1, 2}, 1, 2);
		Image im4({1, 2}, 2, 1);
		Image im5({1, 2, 3}, 1, 3);
		Image im6({1, 2, 3}, 3, 1);
		Image im7({1, 2, 3, 4}, 2, 2);
	}

	void test_invalid_dimensions() {
		TS_ASSERT_THROWS(Image im({1, 2, 3, 4}, 1, 1), std::invalid_argument);
	}

	void test_copy() {

		// Copy assignment
		Image im1({1,2,3,4}, 2, 2);
		assert_not_empty(im1);
		Image im2 = im1;
		assert_not_empty(im1);
		assert_not_empty(im2);
		TS_ASSERT(im1 == im2);

		// Copy ctor
		Image im3({1,2,3,4}, 2, 2);
		assert_not_empty(im3);
		Image im4(im3);
		assert_not_empty(im3);
		assert_not_empty(im4);
		TS_ASSERT(im3 == im4);

		// Construct from vector with data copying
		std::vector<double> data {1, 2, 3, 4};
		Image im5(data, 2, 2);
		assert_not_empty(im5);
		TS_ASSERT_EQUALS(false, data.empty());
		TS_ASSERT_EQUALS(data, std::vector<double>(im5.begin(), im5.end()));
	}

	void test_move() {

		// Move assignment
		Image im1({1,2,3,4}, 2, 2);
		assert_not_empty(im1);
		Image im2(std::move(im1));
		assert_empty(im1);
		assert_not_empty(im2);

		// Move ctor
		Image im3({1,2,3,4}, 2, 2);
		assert_not_empty(im3);
		Image im4 = std::move(im3);
		assert_empty(im3);
		assert_not_empty(im4);

		// Construct from vector with data movement
		std::vector<double> data {1, 2, 3, 4};
		Image im5(std::move(data), 2, 2);
		assert_not_empty(im5);
		TS_ASSERT_EQUALS(true, data.empty());
	}

	void test_subscript() {

		auto r = []() { return double(rand()); };
		auto x = r();
		Image im({x}, 1, 1);
		TS_ASSERT_EQUALS(x, im[0]);
		auto val = im[Point{0, 0}];
		TS_ASSERT_EQUALS(x, val);

		auto x2 = r();
		im[Point{0, 0}] = x2;
		TS_ASSERT_EQUALS(x2, im[0]);
		val = im[Point{0, 0}];
		TS_ASSERT_EQUALS(x2, val);

		Image larger_im {{r(), r(), r(), r(), r(), r(), r(), r(), r()}, 3, 3};
		val = larger_im[Point{2, 2}];
		TS_ASSERT_EQUALS(larger_im[8], val);
		val = larger_im[Point{1, 2}];
		TS_ASSERT_EQUALS(larger_im[7], val);
		val = larger_im[Point{2, 1}];
		TS_ASSERT_EQUALS(larger_im[5], val);
	}

	void test_iterators() {
		auto x = double(rand());
		Image im({x}, 1, 1);

		auto it = im.begin();
		TS_ASSERT_EQUALS(x, *it);
		it++;
		TS_ASSERT_EQUALS(im.end(), it);
	}

	void test_add() {

		std::vector<double> data {1, 2, 3, 4};
		Image im1(data, 2, 2);
		Image im2(data, 2, 2);

		// Normal sum
		Image im3 = im1 + im2;
		// += sum
		im2 += im1;

		// Both sums should have worked
		for(auto im: {&im3, &im2}) {
			for(unsigned int i = 0; i < 4; i++) {
				TS_ASSERT_DELTA((*im)[i], data[i] * 2, 1e-6);
			}
		}
	}

	void test_divide() {

		std::vector<double> data {1, 2, 3, 4};
		Image im1(data, 2, 2);

		Image im2 = im1 / 2;
		im1 /= 2;

		// Both divisions should have worked
		for(auto im: {&im1, &im2}) {
			for(unsigned int i = 0; i < 4; i++) {
				TS_ASSERT_DELTA((*im)[i], data[i] / 2, 1e-6);
			}
		}
	}

	void test_normalize() {

		// use both flavours: const and not const
		Image im1({1, 2, 3, 4}, 2, 2);
		Image im2 = static_cast<const Image &>(im1).normalize();
		im1.normalize();
		for(auto im: {&im1, &im2}) {
			TS_ASSERT_DELTA(1, im->getTotal(), 1e-6);
		}

		// A zero-values image doesn't get normalized
		Image im3({0, 0, 0, 0}, 2, 2);
		Image im4 = static_cast<const Image &>(im3).normalize();
		im3.normalize();
		for(auto im: {&im3, &im4}) {
			TS_ASSERT_DELTA(0, im->getTotal(), 1e-6);
		}

		// Re-normalizing should get us to the same place
		const Image im5({1, 2, 3, 4}, 2, 2);
		Image im6 = static_cast<const Image &>(im5.normalize()).normalize();
		TS_ASSERT_DELTA(1, im6.getTotal(), 1e-6);
	}

	void test_crop() {


		// Dimensions are correct
		Image im1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 5, 2);
		Image im2 = im1.crop({2, 2}, {0, 0});
		TS_ASSERT_EQUALS(2, im2.getWidth());
		TS_ASSERT_EQUALS(2, im2.getHeight());
		im2 = im1.crop({4, 1}, {0, 1});
		TS_ASSERT_EQUALS(4, im2.getWidth());
		TS_ASSERT_EQUALS(1, im2.getHeight());

		// Try all combinations now
		im1 = Image({1, 2, 3, 4}, 2, 2);

		// Should get one pixel at a time
		im2 = im1.crop({1, 1}, {0, 0});
		TS_ASSERT_DELTA(1, im2[0], 1e-6);
		im2 = im1.crop({1, 1}, {1, 0});
		TS_ASSERT_DELTA(2, im2[0], 1e-6);
		im2 = im1.crop({1, 1}, {0, 1});
		TS_ASSERT_DELTA(3, im2[0], 1e-6);
		im2 = im1.crop({1, 1}, {1, 1});
		TS_ASSERT_DELTA(4, im2[0], 1e-6);

		// should get the one row at a time
		im2 = im1.crop({2, 1}, {0, 0});
		TS_ASSERT_DELTA(1, im2[0], 1e-6);
		TS_ASSERT_DELTA(2, im2[1], 1e-6);
		im2 = im1.crop({2, 1}, {0, 1});
		TS_ASSERT_DELTA(3, im2[0], 1e-6);
		TS_ASSERT_DELTA(4, im2[1], 1e-6);

		// should get one column at a time
		im2 = im1.crop({1, 2}, {0, 0});
		TS_ASSERT_DELTA(1, im2[0], 1e-6);
		TS_ASSERT_DELTA(3, im2[1], 1e-6);
		im2 = im1.crop({1, 2}, {1, 0});
		TS_ASSERT_DELTA(2, im2[0], 1e-6);
		TS_ASSERT_DELTA(4, im2[1], 1e-6);

		// gotta get them all!
		im2 = im1.crop({2, 2}, {0, 0});
		TS_ASSERT(im2 == im1);
	}

	void test_invalid_crops() {

		Image im({1, 2, 3, 4}, 2, 2);

		// too wide
		TS_ASSERT_THROWS(im.crop({3, 1}, {0, 0}), std::invalid_argument);

		// too tall
		TS_ASSERT_THROWS(im.crop({1, 3}, {0, 0}), std::invalid_argument);

		// horizontally overflows
		TS_ASSERT_THROWS(im.crop({0, 0}, {3, 1}), std::invalid_argument);

		// vertically overflows
		TS_ASSERT_THROWS(im.crop({0, 0}, {1, 3}), std::invalid_argument);
	}

	void test_extend() {

		Image im({1, 2, 3, 4}, 2, 2);

		// Correct dimensions
		for(auto x: {4u, 10u}) {
			for(auto y: {4u, 10u}) {
				Image im2 = im.extend({x, y}, {0, 0});
				TS_ASSERT_EQUALS(im2.getWidth(), x);
				TS_ASSERT_EQUALS(im2.getHeight(), y);
			}
		}

		// leave at 0, 0
		auto im2 = im.extend({3, 3}, {0, 0});
		TS_ASSERT_EQUALS(im[0], im2[0]);
		TS_ASSERT_EQUALS(im[1], im2[1]);
		TS_ASSERT_EQUALS(im[2], im2[3]);
		TS_ASSERT_EQUALS(im[3], im2[4]);

		// leave at 1, 1
		im2 = im.extend({3, 3}, {1, 1});
		TS_ASSERT_EQUALS(im[0], im2[4]);
		TS_ASSERT_EQUALS(im[1], im2[5]);
		TS_ASSERT_EQUALS(im[2], im2[7]);
		TS_ASSERT_EQUALS(im[3], im2[8]);

		// extend to the same dimensions, should be equals
		im2 = im.extend({2, 2}, {0, 0});
		TS_ASSERT(im == im2);
	}

	void test_invalid_extends() {

		Image im({1, 2, 3, 4}, 2, 2);

		// too thin
		TS_ASSERT_THROWS(im.extend({1, 5}, {0, 0}), std::invalid_argument);

		// too short
		TS_ASSERT_THROWS(im.crop({5, 1}, {0, 0}), std::invalid_argument);

		// horizontally overflows
		TS_ASSERT_THROWS(im.crop({5, 5}, {4, 0}), std::invalid_argument);

		// vertically overflows
		TS_ASSERT_THROWS(im.crop({5, 5}, {0, 4}), std::invalid_argument);

	}

	void test_upsampling() {

		Image im({1, 2, 3, 4}, 2, 2);

		// zero is not allowed
		TS_ASSERT_THROWS(im.upsample(0), std::invalid_argument);

		// identity upsampling
		TS_ASSERT_EQUALS(im, im.upsample(1));

		// Upsampling dimensions are correct
		TS_ASSERT_EQUALS(im.upsample(2).getDimensions(), (Dimensions{4, 4}));
		TS_ASSERT_EQUALS(im.upsample(4).getDimensions(), (Dimensions{8, 8}));
		TS_ASSERT_EQUALS(im.upsample(50).getDimensions(), (Dimensions{100, 100}));

		// Individual pixels seem fine when upsampling without total flux preservation
		auto upsampled = im.upsample(2, Image::UpsamplingMode::COPY);
		std::list<std::pair<double, std::list<Point>>> expectations {
			{im[0], {{0, 0}, {0, 1}, {1, 0}, {1, 1}}},
			{im[1], {{2, 0}, {3, 0}, {2, 1}, {3, 1}}},
			{im[2], {{0, 2}, {1, 2}, {0, 3}, {1, 3}}},
			{im[3], {{2, 2}, {3, 2}, {2, 3}, {3, 3}}}
		};
		for(auto& expectation: expectations) {
			for(auto &point: expectation.second) {
				TS_ASSERT_EQUALS(upsampled[point], expectation.first);
			}
		}

		// And everything is good when upsampling and preserving total flux
		upsampled = im.upsample(2, Image::UpsamplingMode::SCALE);
		for(auto& expectation: expectations) {
			for(auto &point: expectation.second) {
				TS_ASSERT_EQUALS(upsampled[point], expectation.first / 4);
			}
		}
	}

	void test_downsampling() {

		// Two images, we'll check that ceiling works with the im1
		Image im1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, 8, 2);
		Image im2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 5, 3);

		// zero is not allowed
		TS_ASSERT_THROWS(im1.downsample(0), std::invalid_argument);
		TS_ASSERT_THROWS(im2.downsample(0), std::invalid_argument);

		// identity downsampling
		TS_ASSERT_EQUALS(im1, im1.downsample(1));
		TS_ASSERT_EQUALS(im2, im2.downsample(1));

		// Downsampling dimensions are correct
		TS_ASSERT_EQUALS(im1.downsample(2).getDimensions(), (Dimensions{4, 1}));
		TS_ASSERT_EQUALS(im1.downsample(4).getDimensions(), (Dimensions{2, 1}));
		TS_ASSERT_EQUALS(im1.downsample(50).getDimensions(), (Dimensions{1, 1}));

		TS_ASSERT_EQUALS(im2.downsample(2).getDimensions(), (Dimensions{3, 2}));
		TS_ASSERT_EQUALS(im2.downsample(4).getDimensions(), (Dimensions{2, 1}));
		TS_ASSERT_EQUALS(im2.downsample(50).getDimensions(), (Dimensions{1, 1}));

		// Testing SAMPLE mode
		// Image 1
		auto downsampled1 = im1.downsample(2, Image::DownsamplingMode::SAMPLE);
		std::vector<std::pair<Point, double>> expectations1 {
			{{0, 0}, 1}, {{1, 0}, 3}, {{2, 0}, 5}, {{3, 0}, 7}
		};
		for(auto& expectation: expectations1) {
			TS_ASSERT_EQUALS(downsampled1[expectation.first], expectation.second);
		}

		// Image 2
		auto downsampled2 = im2.downsample(2, Image::DownsamplingMode::SAMPLE);
		std::list<std::pair<Point, double>> expectations2 {
			{{0, 0}, 1}, {{1, 0}, 3}, {{2, 0}, 5}, {{0, 1}, 11}, {{1, 1}, 13}, {{2, 1}, 15}
		};
		for(auto& expectation: expectations2) {
			TS_ASSERT_EQUALS(downsampled2[expectation.first], expectation.second);
		}

		// Testing SUM mode
		// Image 1
		downsampled1 = im1.downsample(2, Image::DownsamplingMode::SUM);
		expectations1 = {
			{{0, 0}, 22}, {{1, 0}, 30}, {{2, 0}, 38}, {{3, 0}, 46}
		};
		for(auto& expectation: expectations1) {
			TS_ASSERT_DELTA(downsampled1[expectation.first], expectation.second, 1e-8);
		}

		// Image 2
		downsampled2 = im2.downsample(2, Image::DownsamplingMode::SUM);
		expectations2 = {
			{{0, 0}, 16}, {{1, 0}, 24}, {{2, 0}, 15}, {{0, 1}, 23}, {{1, 1}, 27}, {{2, 1}, 15}
		};
		for(auto& expectation: expectations2) {
			TS_ASSERT_DELTA(downsampled2[expectation.first], expectation.second, 1e-8);
		}

		// Testing AVERAGE mode
		// Image 1
		downsampled1 = im1.downsample(2, Image::DownsamplingMode::AVERAGE);
		expectations1 = {
			{{0, 0}, 5.5}, {{1, 0}, 7.5}, {{2, 0}, 9.5}, {{3, 0}, 11.5}
		};
		for(auto& expectation: expectations1) {
			TS_ASSERT_DELTA(downsampled1[expectation.first], expectation.second, 1e-8);
		}

		// Image 2
		downsampled2 = im2.downsample(2, Image::DownsamplingMode::AVERAGE);
		expectations2 = {
			{{0, 0}, 4}, {{1, 0}, 6}, {{2, 0}, 7.5}, {{0, 1}, 11.5}, {{1, 1}, 13.5}, {{2, 1}, 15}
		};
		for(auto& expectation: expectations2) {
			TS_ASSERT_DELTA(downsampled2[expectation.first], expectation.second, 1e-8);
		}
	}
};