cURL library build for Staticlibs
=================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [cURL library](https://github.com/bagder/curl). 
Wrapper doesn't use cURL's own CMake script directly as it appeared too hard to integrate.

cURL GitHub repository is used as a git submodule of this project. cURL is pinned to version 7.40.

Link to [documentation](http://curl.haxx.se/libcurl/c/).

How to build
------------

[CMake](http://cmake.org/) is required for building.

[pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/) utility is used for dependency management.
For Windows users ready-to-use binary version of `pkg-config` can be obtained from [tools_windows_pkgconfig](https://github.com/staticlibs/tools_windows_pkgconfig) repository.
See [PkgConfig](https://github.com/staticlibs/wiki/wiki/PkgConfig) for Staticlibs-specific details about `pkg-config` usage.

This project depends on [zlib](http://www.zlib.net/) and [openssl](https://www.openssl.org/) libraries.

See [StaticlibsDependencies](https://github.com/staticlibs/wiki/wiki/StaticlibsDependencies) for more 
details about dependency management with Staticlibs.

To build this project manually:

 * checkout all the dependent projects
 * configure these projects using the same output directory:

Run:

    mkdir build
    cd build
    cmake .. -DCMAKE_LIBRARY_OUTPUT_DIRECTORY=<my_lib_dir>

 * build all the dependent projects
 * configure this projects using the same output directory and build it:

[Perl](https://www.perl.org/) is also required for building, Windows users can obtain ready-to-use
Perl distribution from [tools_windows_perl](https://github.com/staticlibs/tools_windows_perl) repository.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone --recursive https://github.com/staticlibs/external_curl.git
    cd external_curl
    mkdir build
    cd build
    cmake .. -DSTATICLIB_TOOLCHAIN=windows_i386_msvc
    msbuild external_curl.sln

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2015-11-10**

 * version 7.40.4
 * separate `configure` and `build` stages for `Makefile` builds

**2015-11-05**

 * version 7.40.3 - `pkg-config` integration

**2015-07-08**

 * version 7.40.2 - toolchains update

**2015-07-01**

 * version 7.40.1 - toolchains update, minor cmake fixes

**2015-05-23**

 * 7.40, initial public version
