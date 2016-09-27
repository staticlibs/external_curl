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
set ( ${PROJECT_NAME}_STATICLIB_VERSION 7.29.0-1 )
set ( ${PROJECT_NAME}_DESCRIPTION "cURL library build for Staticlibs" )
set ( ${PROJECT_NAME}_URL https://github.com/staticlibs/external_curl.git )
include ( ${CMAKE_CURRENT_LIST_DIR}/resources/macros.cmake )

# standalone build
if ( NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
    set ( ZLIB_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
endif ( )
if ( NOT DEFINED STATICLIB_TOOLCHAIN )
    if ( NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
        external_curl_add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/../external_zlib )
        external_curl_add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/../external_openssl )
    endif ( )
    if ( NOT DEFINED ${PROJECT_NAME}_TOOLCHAIN )
        if ( WIN32 )
            if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_amd64_msvc" )
            else ( )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_i386_msvc" )
            endif ( )
        elseif ( APPLE )
            set ( ${PROJECT_NAME}_TOOLCHAIN "macosx_amd64_clang" )
        elseif ( UNIX )
            set ( ${PROJECT_NAME}_TOOLCHAIN "linux_amd64_gcc" )
        else ( )
            message ( FATAL_ERROR "Cannot determine toolchain to use, please specify [${PROJECT_NAME}_TOOLCHAIN]" )
        endif ( )
    endif ( )
else ( )
    set ( ${PROJECT_NAME}_TOOLCHAIN ${STATICLIB_TOOLCHAIN} )
endif ( )

# lookaside build for windows
if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_msvc" )
    # curl subproject
    add_subdirectory ( ${CMAKE_CURRENT_LIST_DIR}/resources/curl_7290_cmake ${CMAKE_CURRENT_BINARY_DIR}/curl )
    add_custom_target ( ${PROJECT_NAME} DEPENDS curl )

    # pkg-config
    set ( ${PROJECT_NAME}_MODULE curl )
    set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_LIST_DIR}/resources/curl_7290_cmake/include_${${PROJECT_NAME}_TOOLCHAIN}" )
    set ( ${PROJECT_NAME}_PC_CFLAGS "${${PROJECT_NAME}_PC_CFLAGS} -I${CMAKE_CURRENT_LIST_DIR}/lookaside_curl/include" )
    set ( ${PROJECT_NAME}_PC_CFLAGS "${${PROJECT_NAME}_PC_CFLAGS} -DCURL_STATICLIB" )
    set ( ${PROJECT_NAME}_PC_LIBS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -lcurl" )
    configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/curl.pc )
    configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/libcurl.pc )
else ()

    # variables for makefiles
    if ( CMAKE_BUILD_TYPE MATCHES "Release" )
        set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
        set ( ${PROJECT_NAME}_DEBUG "--disable-debug" )
        set ( ${PROJECT_NAME}_DEBUG_WINDOWS "no" )
        set ( ${PROJECT_NAME}_DEBUG_MARKER_WINDOWS "release" )
        set ( ${PROJECT_NAME}_DEBUG_LIBNAME_WINDOWS "libcurl_a.lib" )
    else ( )
        set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
        set ( ${PROJECT_NAME}_DEBUG "--enable-debug" )
        set ( ${PROJECT_NAME}_DEBUG_WINDOWS "yes" )
        set ( ${PROJECT_NAME}_DEBUG_MARKER_WINDOWS "debug" )
        set ( ${PROJECT_NAME}_DEBUG_LIBNAME_WINDOWS "libcurl_a_debug.lib" )    
    endif ( )
    if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_i386_[^_]+" )
        set ( ${PROJECT_NAME}_ARCH_WINDOWS "x86" )
    elseif ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_amd64_[^_]+" )
        set ( ${PROJECT_NAME}_ARCH_WINDOWS "x64" )
    endif ( )
    if ( TARGET external_zlib )
        set ( ${PROJECT_NAME}_ZLIB_INSTALLED_PATH "=${CMAKE_BINARY_DIR}/external_zlib/zlib_install" )
    endif ( )
    if ( TARGET external_openssl )
        set ( ${PROJECT_NAME}_OPENSSL_INSTALLED_PATH "=${CMAKE_BINARY_DIR}/external_openssl/openssl_install" )
    endif ( )

    # copy files
    file ( COPY ${CMAKE_CURRENT_LIST_DIR}/curl
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file ( COPY ${CMAKE_CURRENT_LIST_DIR}/resources/preconfigured/curlbuild-${${PROJECT_NAME}_TOOLCHAIN}.h
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/curl/include/curl/ )
    file ( RENAME ${CMAKE_CURRENT_BINARY_DIR}/curl/include/curl/curlbuild-${${PROJECT_NAME}_TOOLCHAIN}.h
            ${CMAKE_CURRENT_BINARY_DIR}/curl/include/curl/curlbuild.h )
    if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
        configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/windows/MakefileBuild.vc
                ${CMAKE_CURRENT_BINARY_DIR}/curl/winbuild/MakefileBuild.vc )
    endif ( )

    # check deplibs cache
    if ( STATICLIB_USE_DEPLIBS_CACHE )
        set ( ${PROJECT_NAME}_CACHED_LIB_PATH ${STATICLIB_DEPLIBS_CACHE_DIR}/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX} )
        if ( EXISTS ${${PROJECT_NAME}_CACHED_LIB_PATH} )
            add_library( curl STATIC IMPORTED GLOBAL )
            set_target_properties( curl PROPERTIES IMPORTED_LOCATION ${${PROJECT_NAME}_CACHED_LIB_PATH} )
            add_custom_target( ${PROJECT_NAME} )
            message ( STATUS "Using cached library: [${${PROJECT_NAME}_CACHED_LIB_PATH}]" )
            return ( )
        endif ( )
    endif ( )


    # dependencies
    set ( ${PROJECT_NAME}_DEPS zlib openssl )
    external_curl_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

    # library config
    if ( NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY )
        set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
    endif ( )
    # copy makefile
    configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/Makefile-${${PROJECT_NAME}_TOOLCHAIN}
            ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${${PROJECT_NAME}_TOOLCHAIN})
    if ( ${PROJECT_NAME}_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
        set ( ${PROJECT_NAME}_MAKE_PROGRAM nmake )
    else ( )
        set ( ${PROJECT_NAME}_MAKE_PROGRAM ${CMAKE_MAKE_PROGRAM} )
    endif ( )

    # run configure
    execute_process ( COMMAND ${${PROJECT_NAME}_MAKE_PROGRAM}
            -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${${PROJECT_NAME}_TOOLCHAIN} configure
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
    # add build target
    add_custom_target( ${PROJECT_NAME} ALL COMMAND ${${PROJECT_NAME}_MAKE_PROGRAM} 
            -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${${PROJECT_NAME}_TOOLCHAIN} build
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
    add_library( curl STATIC IMPORTED GLOBAL )
    set_target_properties( curl PROPERTIES IMPORTED_LOCATION 
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX} )
    add_dependencies( curl ${PROJECT_NAME} )
    # required here for curl configure-time checks
    if ( TARGET external_zlib )
        add_dependencies( ${PROJECT_NAME} external_zlib )
    endif ( )
    if ( TARGET external_openssl )
        add_dependencies( ${PROJECT_NAME} external_openssl )
    endif ( )

    # pkg-config
    set ( ${PROJECT_NAME}_MODULE libcurl )
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
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/libcurl.pc )
endif ( )
