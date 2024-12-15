# CMake generated Testfile for 
# Source directory: /home/txy/sdp/components/sdk/pcre/pcre-8.45
# Build directory: /home/txy/sdp/components/sdk/pcre/pcre-8.45/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(pcre_test "sh" "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre_test.sh")
set_tests_properties(pcre_test PROPERTIES  _BACKTRACE_TRIPLES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;866;ADD_TEST;/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;0;")
add_test(pcre_grep_test "sh" "/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/pcre_grep_test.sh")
set_tests_properties(pcre_grep_test PROPERTIES  _BACKTRACE_TRIPLES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;882;ADD_TEST;/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;0;")
add_test(pcrecpp_test "pcrecpp_unittest")
set_tests_properties(pcrecpp_test PROPERTIES  _BACKTRACE_TRIPLES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;927;ADD_TEST;/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;0;")
add_test(pcre_scanner_test "pcre_scanner_unittest")
set_tests_properties(pcre_scanner_test PROPERTIES  _BACKTRACE_TRIPLES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;928;ADD_TEST;/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;0;")
add_test(pcre_stringpiece_test "pcre_stringpiece_unittest")
set_tests_properties(pcre_stringpiece_test PROPERTIES  _BACKTRACE_TRIPLES "/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;929;ADD_TEST;/home/txy/sdp/components/sdk/pcre/pcre-8.45/CMakeLists.txt;0;")
