// SPDX-License-Identifier: MIT

#include "timestamp_format.h"

#include <math.h>
#include <stdio.h>

#define NTP_JAN_1970 2208988800UL  // First day of UNIX in NTP timestamp era.
#define MASK_32_BIT 0xFFFFFFFFUL   // 32-bit mask.
#define SEC_TO_NSEC 1000000000UL   // Ratio to convert seconds to nanoseconds.

static inline uint64_t EndianReverseU64(uint64_t x) {
  uint64_t step32 = x << 32 | x >> 32;
  uint64_t step16 = (step32 & 0x0000FFFF0000FFFFULL) << 16 | (step32 & 0xFFFF0000FFFF0000ULL) >> 16;
  return (step16 & 0x00FF00FF00FF00FFULL) << 8 | (step16 & 0xFF00FF00FF00FF00ULL) >> 8;
}

#define N2H_U64(x) EndianReverseU64((uint64_t)x)
#define H2N_U64(x) EndianReverseU64((uint64_t)x)

int NtpToUnixTimestamp(uint64_t ntp_timestamp, uint64_t* unix_timestamp_ns) {
  if (!unix_timestamp_ns) {
    return -1;
  }
  ntp_timestamp = N2H_U64(ntp_timestamp);
  uint64_t ntp_seconds = (ntp_timestamp >> 32) & MASK_32_BIT;
  uint64_t ntp_fraction = ntp_timestamp & MASK_32_BIT;

  if (ntp_seconds < NTP_JAN_1970) {
    // Too early for UNIX.
    return -2;
  }

  uint64_t unix_subseconds = round((double)ntp_fraction / (1UL << 32) * SEC_TO_NSEC);
  *unix_timestamp_ns = (ntp_seconds - NTP_JAN_1970) * SEC_TO_NSEC + unix_subseconds;
  return 0;
}

int UnixToNtpTimestamp(uint64_t unix_timestamp_ns, uint64_t* ntp_timestamp) {
  uint64_t unix_seconds = unix_timestamp_ns / SEC_TO_NSEC;
  uint64_t unix_sub_seconds = unix_timestamp_ns % SEC_TO_NSEC;

  uint64_t ntp_fraction = round((double)(unix_sub_seconds) * (1UL << 32) / SEC_TO_NSEC);
  *ntp_timestamp = H2N_U64(((unix_seconds + NTP_JAN_1970) << 32) + ntp_fraction);
  return 0;
}
