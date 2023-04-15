# NTP timestamps and conversion

Reference: https://www.rfc-editor.org/rfc/rfc5905


## Definition

NTP timestamp format: 32-bit Seconds + 32-bit Fraction.
```
|------ 32-bit ------||------ 32-bit ------|
|      seconds       ||      fraction      |
```

The start time of the epoch, i.e. the base date of era 0, is midnight Jan 1th, 1900 UTC.


## Conversion to Unix Timestamps

The Unix epoch is midnight Jan 1th, 1970. So to convert between Unix timestamp and NTP timestamp, the first thing is to add or subtract the time difference, i.e. 2,208,988,800 seconds.

The second thing to do is to convert the time to the format of 32-bit integer seconds + 32-bit fraction.

If we assume the unix time is get by the [`gettimeofday`](https://man7.org/linux/man-pages/man2/settimeofday.2.html) call (which is obsolete), the unix time will be get in the format of `struct timeval`. The conversion can be done by:
```C
/* The conversion included in RFC5905. */

typedef unsigned long long tstamp;   /* NTP timestamp format */

#define JAN_1970   2208988800UL      /* 1970 - 1900 in seconds */
#define FRIC       65536.                  /* 2^16 as a double */
#define FRAC       4294967296.             /* 2^32 as a double */
#define U2LFP(a)   (((unsigned long long) \
                       ((a).tv_sec + JAN_1970) << 32) + \
                       (unsigned long long) \
                       ((a).tv_usec / 1e6 * FRAC))

/*
 * get_time - read system time and convert to NTP format
 */
tstamp
get_time()
{
        struct timeval unix_time;
        gettimeofday(&unix_time, NULL);
        /*
         * Conversion from Unix timestamp to NTP timestamp can be done
         * directly by the U2LFP macro.
         */
        return (U2LFP(unix_time));
}

/*
 * step_time - step system time to given offset value
 */
void
step_time(
        double  offset          /* clock offset */
        )
{
        struct timeval unix_time;
        tstamp ntp_time;

        /*
         * Convert from double to native format (signed) and add to the
         * current time.  Note the addition is done in native format to
         * avoid overflow or loss of precision.
         */
        gettimeofday(&unix_time, NULL);
        /*
         * Convert the unix_time to NTP timestamp by the U2LFP macro.
         */
        ntp_time = D2LFP(offset) + U2LFP(unix_time);
        /*
         * Convert the offsetted NTP timestamp back to Unix timestamp.
         */
        unix_time.tv_sec = ntp_time >> 32;
        unix_time.tv_usec = (long)(((ntp_time - unix_time.tv_sec) <<
            32) / FRAC * 1e6);
        settimeofday(&unix_time, NULL);
}
```
