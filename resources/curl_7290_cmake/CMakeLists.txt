# Copyright 2016, alex at staticlibs.net
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
project ( curl C )
set ( ${PROJECT_NAME}_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/../../lookaside_curl )
include ( ${CMAKE_CURRENT_LIST_DIR}/../macros.cmake )

# standalone build
if ( NOT DEFINED STATICLIB_TOOLCHAIN )
    if ( NOT DEFINED ${PROJECT_NAME}_TOOLCHAIN )
        if ( WIN32 )
            if ( CMAKE_SIZEOF_VOID_P EQUAL 8 )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_amd64_msvc" )
            else ( )
                set ( ${PROJECT_NAME}_TOOLCHAIN "windows_i386_msvc" )
            endif ( )
        else ( )
            message ( FATAL_ERROR "Cannot determine toolchain to use, please specify [${PROJECT_NAME}_TOOLCHAIN]" )
        endif ( )
    endif ( )
else ( )
    set ( ${PROJECT_NAME}_TOOLCHAIN ${STATICLIB_TOOLCHAIN} )
endif ( )

# dependencies
set ( ${PROJECT_NAME}_DEPS zlib openssl )
external_curl_pkg_check_modules ( ${PROJECT_NAME}_DEPS_PC REQUIRED ${PROJECT_NAME}_DEPS )

# definitions
set ( ${PROJECT_NAME}_DEFINITIONS
        -DCURL_STATICLIB 
        -DBUILDING_LIBCURL 
        -DUSE_SSLEAY
        -DHAVE_ZLIB_H
        -DHAVE_ZLIB
        -DHAVE_LIBZ
        -DWIN32
        -DWIN32_LEAN_AND_MEAN )

# sources
set ( ${PROJECT_NAME}_SRC 
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/file.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/timeval.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/base64.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostip.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/progress.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/formdata.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cookie.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sendf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ftp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/url.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/dict.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/if2ip.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/speedcheck.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ldap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ssluse.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/version.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/getenv.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/escape.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/mprintf.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/telnet.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/netrc.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/getinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/transfer.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/strequal.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/easy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/security.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/krb4.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_fnmatch.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/fileinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ftplistparser.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/wildcard.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/krb5.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/memdebug.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http_chunks.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/strtok.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/connect.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/llist.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hash.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/multi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/content_encoding.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/share.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http_digest.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/md4.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/md5.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_rand.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http_negotiate.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/inet_pton.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/strtoofft.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/strerror.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/amigaos.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostasyn.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostip4.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostip6.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostsyn.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/inet_ntop.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/parsedate.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/select.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/gtls.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/sslgen.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/tftp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/splay.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/strdup.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/socks.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/ssh.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nss.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/qssl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/rawstr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_addrinfo.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/socks_gssapi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/socks_sspi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_sspi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/slist.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/nonblock.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_memrchr.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/imap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pop3.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/smtp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/pingpong.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/rtsp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_threads.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/warnless.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hmac.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/polarssl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_rtmp.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/openldap.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_gethostname.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/gopher.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/axtls.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/idn_win32.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http_negotiate_sspi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/cyassl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/http_proxy.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/non-ascii.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/asyn-ares.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/asyn-thread.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_gssapi.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_ntlm.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_ntlm_wb.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_ntlm_core.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_ntlm_msgs.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_sasl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_schannel.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_multibyte.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/curl_darwinssl.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/hostcheck.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/bundles.c
        ${${PROJECT_NAME}_SOURCE_DIR}/lib/conncache.c )
source_group ( "src" FILES ${${PROJECT_NAME}_SRC} )

# library
add_library ( ${PROJECT_NAME} STATIC 
        ${${PROJECT_NAME}_SRC} 
        ${${PROJECT_NAME}_ASMOBJ} )
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include_${${PROJECT_NAME}_TOOLCHAIN}
        ${${PROJECT_NAME}_SOURCE_DIR}/include
        ${${PROJECT_NAME}_SOURCE_DIR}/lib
        ${${PROJECT_NAME}_DEPS_PC_INCLUDE_DIRS} ) 
target_compile_definitions ( ${PROJECT_NAME} PRIVATE ${${PROJECT_NAME}_DEFINITIONS} )

