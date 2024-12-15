# Install script for directory: /home/txy/sdp/components/sdk/pcre/pcre-8.45

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcre.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcreposix.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcrecpp.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcregrep")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcregrep")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcretest")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcretest")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcrecpp_unittest")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcrecpp_unittest")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre_scanner_unittest")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_scanner_unittest")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre_stringpiece_unittest")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/pcre_stringpiece_unittest")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre.h"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/pcreposix.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/pcrecpp.h"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/pcre_scanner.h"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcrecpparg.h"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre_stringpiece.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man1" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre-config.1"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcregrep.1"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcretest.1"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/man/man3" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre16.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre32.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_assign_jit_stack.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_compile.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_compile2.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_config.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_copy_named_substring.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_copy_substring.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_dfa_exec.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_exec.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_free_study.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_free_substring.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_free_substring_list.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_fullinfo.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_get_named_substring.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_get_stringnumber.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_get_stringtable_entries.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_get_substring.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_get_substring_list.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_jit_exec.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_jit_stack_alloc.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_jit_stack_free.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_maketables.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_pattern_to_host_byte_order.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_refcount.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_study.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_utf16_to_host_byte_order.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_utf32_to_host_byte_order.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcre_version.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcreapi.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrebuild.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrecallout.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrecompat.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrecpp.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcredemo.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrejit.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrelimits.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrematching.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrepartial.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrepattern.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcreperform.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcreposix.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcreprecompile.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcresample.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcrestack.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcresyntax.3"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/pcreunicode.3"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/pcre/html" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/index.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre-config.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre16.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre32.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_assign_jit_stack.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_compile.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_compile2.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_config.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_copy_named_substring.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_copy_substring.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_dfa_exec.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_exec.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_free_study.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_free_substring.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_free_substring_list.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_fullinfo.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_get_named_substring.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_get_stringnumber.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_get_stringtable_entries.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_get_substring.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_get_substring_list.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_jit_exec.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_jit_stack_alloc.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_jit_stack_free.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_maketables.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_pattern_to_host_byte_order.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_refcount.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_study.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_utf16_to_host_byte_order.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_utf32_to_host_byte_order.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcre_version.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcreapi.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrebuild.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrecallout.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrecompat.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrecpp.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcredemo.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcregrep.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrejit.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrelimits.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrematching.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrepartial.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrepattern.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcreperform.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcreposix.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcreprecompile.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcresample.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcrestack.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcresyntax.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcretest.html"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/doc/html/pcreunicode.html"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcre.pc"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcrecpp.pc"
    "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/libpcreposix.pc"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE FILE PERMISSIONS OWNER_WRITE OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE FILES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre-config")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
