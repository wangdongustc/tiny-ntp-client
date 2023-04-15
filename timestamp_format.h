// SPDX-License-Identifier: MIT

#ifndef TINY_NTP_TIMESTAMP_FORMAT_
#define TINY_NTP_TIMESTAMP_FORMAT_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int NtpToUnixTimestamp(uint64_t ntp_timestamp, uint64_t* unix_timestamp_ns);

int UnixToNtpTimestamp(uint64_t unix_timestamp_ns, uint64_t* ntp_timestamp);

#ifdef __cplusplus
}
#endif

#endif  // TINY_NTP_TIMESTAMP_FORMAT_
