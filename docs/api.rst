API
===

.. contents:: Contents
   :local:

*libprofit* has two main data types:
the model class (:class:`Model`)
and the base profile class (:class:`Profile`).
We introduce the model first, then the base profile.

As a small reference, see the following class diagram.

.. image:: images/types.png

Library
-------


.. doxygenfunction:: profit::init()

.. doxygenfunction:: profit::finish()

.. doxygenfunction:: profit::version()

.. doxygenfunction:: profit::version_major()

.. doxygenfunction:: profit::version_minor()

.. doxygenfunction:: profit::version_patch()

.. doxygenfunction:: profit::has_openmp()

.. doxygenfunction:: profit::has_fftw()

.. doxygenfunction:: profit::has_fftw_with_openmp()

.. doxygenfunction:: profit::has_opencl()

.. doxygenfunction:: profit::opencl_version_major()

.. doxygenfunction:: profit::opencl_version_minor()

Exceptions
----------

.. doxygenclass:: profit::invalid_parameter
   :project: libprofit

Convolvers
----------

.. doxygenenum:: profit::ConvolverType

.. doxygenclass:: profit::Convolver
   :members: convolve

.. doxygenfunction:: profit::create_convolver(const ConvolverType, const ConvolverCreationPreferences&)

.. doxygenfunction:: profit::create_convolver(const std::string&, const ConvolverCreationPreferences&)

.. doxygenclass:: profit::ConvolverCreationPreferences
   :members:


Model class
-----------

.. doxygenclass:: profit::Model
   :members:

Profile classes
---------------

.. doxygenclass:: profit::Profile
   :members: convolve, parameter

.. doxygenclass:: profit::RadialProfile
   :members: xcen, ycen, mag, ang, axrat, box, rough, acc, rscale_switch,
             resolution, max_recursions, adjust, rscale_max

.. doxygenclass:: profit::SersicProfile
   :members: re, nser, rescale_flux

.. doxygenclass:: profit::MoffatProfile
   :members: fwhm, con

.. doxygenclass:: profit::FerrerProfile
   :members: rout, a, b

.. doxygenclass:: profit::CoreSersicProfile
   :members: re, rb, nser, a, b

.. doxygenclass:: profit::BrokenExponentialProfile
   :members: h1, h2, rb, a

.. doxygenclass:: profit::KingProfile
   :members: rc, rt, a

.. doxygenclass:: profit::PsfProfile
   :members: xcen, ycen, mag

.. doxygenclass:: profit::SkyProfile
   :members: bg

.. doxygenclass:: profit::NullProfile
