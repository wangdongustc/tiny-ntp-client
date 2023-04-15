// SPDX-License-Identifier: MIT

#include "timestamp_format.h"

#include <ctime>
#include <iomanip>
#include <iostream>

#include "gtest/gtest.h"

namespace {

void PrintUnixTime(uint64_t unix_timestamp_ns) {
  double time_in_seconds = static_cast<double>(unix_timestamp_ns) / 1e9;
  std::time_t t = time_in_seconds;
  std::tm tm = *std::gmtime(&t);
  std::cout << std::put_time(&tm, "%a %b %d %H:%M:");
  // We want the sencond part to be longer.
  double tm_sec = tm.tm_sec + time_in_seconds - t;
  std::printf("%09.6f ", tm_sec);
  std::cout << std::put_time(&tm, "%Y %Z") << std::endl;
}

}

TEST(TestTimestampFormat, TimestampConversionTest) {
  // Reference Timestamp: Apr 15, 2023 14:37:16.618067320 UTC
  // Copied from Wireshark captures.
  constexpr uint64_t kExpectedUnixTimeNs = 1681569436618067320;
  constexpr uint8_t kNtpTimeBytes[] = {0xe7, 0xe5, 0x35, 0x1c, 0x9e, 0x39, 0xa8, 0xf1};
  uint64_t ntp_time = 0;
  uint64_t unix_time = 0;
  memcpy(&ntp_time, kNtpTimeBytes, sizeof(ntp_time));

  // Convert NTP to UNIX.
  EXPECT_EQ(0, NtpToUnixTimestamp(ntp_time, &unix_time));
  std::cout << "UNIX timestamp: " << unix_time << "ns\n";
  PrintUnixTime(unix_time);
  EXPECT_NEAR(unix_time, kExpectedUnixTimeNs, 1);

  // Convert UNIX to NTP and back to UNIX.
  unix_time = 0;
  EXPECT_EQ(0, UnixToNtpTimestamp(kExpectedUnixTimeNs, &ntp_time));
  EXPECT_EQ(0, NtpToUnixTimestamp(ntp_time, &unix_time));
  std::cout << "UNIX timestamp: " << unix_time << "ns\n";
  PrintUnixTime(unix_time);
  EXPECT_NEAR(unix_time, kExpectedUnixTimeNs, 1);
}
