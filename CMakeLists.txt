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

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_gcc CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_curl C CXX )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 7.40.2 )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/staticlibs_common.cmake )

# variabled for makefiles
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

# dependencies
staticlib_add_subdirectory ( deps external_zlib )
staticlib_add_subdirectory ( deps external_openssl )

# library config
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/Makefile-${STATICLIB_TOOLCHAIN}
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_custom_target( ${PROJECT_NAME}_build
        COMMAND ${CMAKE_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_dependencies( ${PROJECT_NAME}_build external_zlib )
add_dependencies( ${PROJECT_NAME}_build external_openssl )
add_dependencies( ${PROJECT_NAME}_build external_openssl_ssl )
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX} )
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_build )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/curl/include )

# tests
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} external_zlib external_openssl external_openssl_ssl )
set ( ${PROJECT_NAME}_TEST_OPTS "" )
if ( STATICLIB_TOOLCHAIN MATCHES "linux_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_TEST_LIBS ${${PROJECT_NAME}_TEST_LIBS} dl rt )
endif ( )
if ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_msvc" )
    set ( ${PROJECT_NAME}_TEST_OPTS -DCURL_STATICLIB )
endif ( )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME}_TEST_OPTS )

# headers export
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
