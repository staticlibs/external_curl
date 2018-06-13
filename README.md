cURL library build for Staticlibs
=================================

[![travis](https://travis-ci.org/staticlibs/external_curl.svg?branch=master)](https://travis-ci.org/staticlibs/external_curl)
[![appveyor](https://ci.appveyor.com/api/projects/status/github/staticlibs/external_curl?svg=true)](https://ci.appveyor.com/project/staticlibs/external-curl)

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [cURL library](https://curl.haxx.se/libcurl/) that
can be used to build sources imported from [CentOS lookaside](https://github.com/staticlibs/lookaside_curl.git).

Link to the [documentation](http://curl.haxx.se/libcurl/c/).

How to build
------------

[CMake](http://cmake.org/) is required for building.

[pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/) utility is used for dependency management.
For Windows users ready-to-use binary version of `pkg-config` can be obtained from [tools_windows_pkgconfig](https://github.com/staticlibs/tools_windows_pkgconfig) repository.
See [StaticlibsPkgConfig](https://github.com/staticlibs/wiki/wiki/StaticlibsPkgConfig) for Staticlibs-specific details about `pkg-config` usage.

[Perl](https://www.perl.org/) is also required for building, Windows users can obtain ready-to-use
Perl distribution from [tools_windows_perl](https://github.com/staticlibs/tools_windows_perl) repository.

[NASM](http://nasm.us/) is also required for building on Windows.
You can obtain ready-to-use NASM distribution from 
[tools_windows_nasm](https://github.com/staticlibs/tools_windows_nasm) repository.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone https://github.com/staticlibs/external_zlib.git
    git clone https://github.com/staticlibs/lookaside_zlib.git
    git clone https://github.com/staticlibs/external_openssl.git
    git clone https://github.com/staticlibs/lookaside_openssl.git
    git clone https://github.com/staticlibs/external_curl.git
    git clone https://github.com/staticlibs/lookaside_curl.git
    cd external_curl
    mkdir build
    cd build
    cmake ..
    msbuild external_curl.sln

Cloning of [external_zlib](https://github.com/staticlibs/external_zlib) and 
[external_openssl](https://github.com/staticlibs/external_openssl.git) is not required on Linux - 
system libraries will be used instead.

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2018-06-13**

 * version 7.29.0-7
 * include April 2018 patches

**2018-04-10**

 * version 7.29.0-6
 * allow to specify WinXP compatibility

**2018-03-02**

 * version 7.29.0-5
 * sdk8.1+ build fix

**2018-02-28**

 * version 7.29.0-4
 * drop submodule

**2017-12-31**

 * version 7.29.0-3
 * vs2017 support

**2016-11-08**

 * version 7.29.0-2
 * cURL sources update
 * Android and Linux builds

**2016-10-29**

 * version 7.29.0-1
 * use cURL sources from CentOS

**2016-01-20**

 * version 7.40.5
 * minor CMake changes

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
