# SPDX-License-Identifier: MIT

add_executable(timestamp_format_test timestamp_format_test.cc)
target_link_libraries(timestamp_format_test PRIVATE timestamp_format GTest::gtest_main)
add_test(timestamp_format_test timestamp_format_test)
