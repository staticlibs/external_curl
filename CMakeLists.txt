
cmake_minimum_required ( VERSION 2.8 )

# common staticlibs setup
if ( NOT DEFINED STATICLIB_PARENT_DIR )
    set ( STATICLIB_TOOLCHAIN amd64_linux_clang CACHE STRING "toolchain triplet" )
    set ( STATICLIB_PARENT_DIR ${CMAKE_CURRENT_LIST_DIR}/../ CACHE INTERNAL "parent dir" )
    include ( ${STATICLIB_PARENT_DIR}/staticlib_util_cmake/CMakeLists_common.cmake )
    staticlib_set_toolchain ( ${STATICLIB_PARENT_DIR} ${STATICLIB_TOOLCHAIN} )
endif ( )

# project header
project ( staticlib_ext_curl C CXX )
set ( ${PROJECT_NAME}_VERSION_MAJOR 7 )
set ( ${PROJECT_NAME}_VERSION_MINOR 4 )
set ( ${PROJECT_NAME}_VERSION_MICRO 0 )

# variabled for makefiles
if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
    set ( ${PROJECT_NAME}_DEBUG "--enable-debug" )
endif ( )
if ( CMAKE_BUILD_TYPE MATCHES "Release" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
    set ( ${PROJECT_NAME}_DEBUG "--disable-debug" )
endif ( )

# library config
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/Makefile-${STATICLIB_TOOLCHAIN}
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_custom_target( ${PROJECT_NAME}_build
        COMMAND ${CMAKE_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/libcurl${CMAKE_STATIC_LIBRARY_SUFFIX} )
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_build )

# tests
#include_directories ( ${CMAKE_CURRENT_BINARY_DIR}/libxml2/include )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_LIBS )

# headers export
#set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/libxml2/include CACHE INTERNAL ${PROJECT_NAME}_INCLUDES )
