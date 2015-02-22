
cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_clang CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_curl CXX )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists_common.cmake )

# variabled for makefiles
if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
    set ( ${PROJECT_NAME}_DEBUG "--enable-debug" )
endif ( )
if ( CMAKE_BUILD_TYPE MATCHES "Release" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
    set ( ${PROJECT_NAME}_DEBUG "--disable-debug" )
endif ( )

# dependencies
staticlib_add_subdirectory ( external_zlib )
staticlib_add_subdirectory ( external_openssl )

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
enable_testing ( )
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
add_executable ( curl_test ${CMAKE_CURRENT_LIST_DIR}/test/curl_test.cpp )
target_include_directories ( curl_test BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
target_link_libraries ( curl_test ${PROJECT_NAME} rt dl external_zlib external_openssl external_openssl_ssl )
add_test ( curl_test ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/curl_test )

# headers export
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
