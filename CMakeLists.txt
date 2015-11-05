# Copyright 2015, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# project setup
project ( external_curl C CXX )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 7.40.3 )
set ( ${PROJECT_NAME}_DESCRIPTION "cURL library build for Staticlibs" )
set ( ${PROJECT_NAME}_URL https://github.com/staticlibs/external_curl.git )
include ( ${CMAKE_CURRENT_LIST_DIR}/resources/macros.cmake )

# variables for makefiles
if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
    set ( ${PROJECT_NAME}_DEBUG "--enable-debug" )
    set ( ${PROJECT_NAME}_DEBUG_WINDOWS "yes" )
    set ( ${PROJECT_NAME}_DEBUG_MARKER_WINDOWS "debug" )
    set ( ${PROJECT_NAME}_DEBUG_LIBNAME_WINDOWS "libcurl_a_debug.lib" )
endif ( )
if ( CMAKE_BUILD_TYPE MATCHES "Release" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
    set ( ${PROJECT_NAME}_DEBUG "--disable-debug" )
    set ( ${PROJECT_NAME}_DEBUG_WINDOWS "no" )
    set ( ${PROJECT_NAME}_DEBUG_MARKER_WINDOWS "release" )
    set ( ${PROJECT_NAME}_DEBUG_LIBNAME_WINDOWS "libcurl_a.lib" )
endif ( )
if ( STATICLIB_TOOLCHAIN MATCHES "windows_i386_[^_]+" )
    set ( ${PROJECT_NAME}_ARCH_WINDOWS "x86" )
elseif ( STATICLIB_TOOLCHAIN MATCHES "windows_amd64_[^_]+" )
    set ( ${PROJECT_NAME}_ARCH_WINDOWS "x64" )
endif ( )
if ( TARGET external_zlib )
    set ( ${PROJECT_NAME}_ZLIB_INSTALLED_PATH "=${CMAKE_BINARY_DIR}/external_zlib/zlib_install" )
endif ( )
if ( TARGET external_openssl )
    set ( ${PROJECT_NAME}_OPENSSL_INSTALLED_PATH "=${CMAKE_BINARY_DIR}/external_openssl/openssl_install" )
endif ( )

# dependencies
set ( ${PROJECT_NAME}_DEPS zlib openssl )
external_curl_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# library config
if ( NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
endif ( )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/Makefile-${STATICLIB_TOOLCHAIN}
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
if ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_MAKE_PROGRAM nmake )
else ( )
    set ( ${PROJECT_NAME}_MAKE_PROGRAM make )
endif ( )
add_custom_target( ${PROJECT_NAME} ALL
        COMMAND ${${PROJECT_NAME}_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_library( curl STATIC IMPORTED GLOBAL )
set_target_properties( curl PROPERTIES IMPORTED_LOCATION 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX} )
add_dependencies( curl ${PROJECT_NAME} )
if ( TARGET external_zlib )
    add_dependencies( ${PROJECT_NAME} external_zlib )
endif ( )
if ( TARGET external_openssl )
    add_dependencies( ${PROJECT_NAME} external_openssl )
endif ( )

# pkg-config
set ( ${PROJECT_NAME}_MODULE curl )
set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/curl/include" )
if ( WIN32 )
    set ( ${PROJECT_NAME}_PC_CFLAGS "${${PROJECT_NAME}_PC_CFLAGS} -DCURL_STATICLIB" )
endif ( )
set ( ${PROJECT_NAME}_PC_REQUIRES_PRIVATE "zlib openssl" )
set ( ${PROJECT_NAME}_PC_LIBS "-lcurl" )
if ( CMAKE_SYSTEM_NAME MATCHES "Linux" )
    set ( ${PROJECT_NAME}_PC_LIBS "${${PROJECT_NAME}_PC_LIBS} -ldl -lrt" )
endif ( )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/curl.pc )
