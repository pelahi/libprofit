/**
 * Definitions of library-related routines for libprofit
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

#ifndef PROFIT_INIT_FINI_H_
#define PROFIT_INIT_FINI_H_

#include <string>

#include "profit/common.h"

namespace profit {

/// Returns the version of this libprofit library
/// @return The version of this libprofit library
PROFIT_API std::string version();

/// Returns the major version of this libprofit library
/// @return The major version of this libprofit library
PROFIT_API unsigned short version_major();

/// Returns the minor version of this libprofit library
/// @return The minor version of this libprofit library
PROFIT_API unsigned short version_minor();

/// Returns the patch version of this libprofit library
/// @return The patch version of this libprofit library
PROFIT_API unsigned short version_patch();

/// Returns the version suffix (e.g., "dev" or "rc1") of this libprofit library.
/// If no version suffix is present in this version, an empty string is returned.
/// @return The version suffix of this libprofit library
PROFIT_API std::string version_suffix();

/// Initializes the libprofit library. This function must be called once
/// before using the library in any way. At the end, call finish().
/// If the user fails to call init() the library *might* work,
/// but it's not guaranteed that it will do so correctly, or as intended.
///
/// @return If the initialization was correct
PROFIT_API bool init();

/// Finalizes the libprofit library. All internal resources are freed.
/// This method should be called after the library has been used.
/// After a call to finish(), no other usage of the library should occur,
/// unless init() is called again.
PROFIT_API void finish();

/// Returns whether libprofit was compiled with OpenMP support
/// @return Whether libprofit was compiled with OpenMP support
PROFIT_API bool has_openmp();

/// Returns whether libprofit was compiled with FFTW support
/// @return Whether libprofit was compiled with FFTW support
PROFIT_API bool has_fftw();

/// Returns whether libprofit was compiled against an FFTW library with OpenMP support
/// @return Whether libprofit was compiled against an FFTW library with OpenMP support
PROFIT_API bool has_fftw_with_openmp();

/// Returns whether libprofit was compiled with OpenCL support
/// @return Whether libprofit was compiled with OpenCL support
PROFIT_API bool has_opencl();

/// If OpenCL is supported, returns the major portion of the highest OpenCL
/// platform version libprofit can work against. For example, if libprofit was
/// compiled against a platform supporting OpenCL 2.1, this method returns 2.
/// If OpenCL is not supported, the result is undefined.
/// @return The major highest OpenCL platform version that libprofit can work
/// against.
PROFIT_API unsigned short opencl_version_major();

/// If OpenCL is supported, returns the minor portion of the highest OpenCL
/// platform version libprofit can work against. For example, if libprofit was
/// compiled against a platform supporting OpenCL 1.2, this method returns 2.
/// If OpenCL is not supported, the result is undefined.
PROFIT_API unsigned short opencl_version_minor();

/// Clears the cache area used by libprofit. Depending on the supported features
/// this will remove certain files from the disk
PROFIT_API void clear_cache();

}

#endif // PROFIT_INIT_FINI_H_