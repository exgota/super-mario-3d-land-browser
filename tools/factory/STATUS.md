# Factory status, Oct 02 02:16 ET

State: **stopped**. Last sync with main: not yet.

Whole project on the factory branch: **2.06%** of code bytes (56,648 of 2,756,016), 858 of 18,054 functions exact.
Factory commits not yet in main: 3.

Queue: failed 1 jobs / 1 functions, matched 4 jobs / 91 functions, open 10084 jobs / 14179 functions

## Efficiency by tier

| Tier | Model | Runs | Functions matched | Bytes matched | Uncached input | Output | Tokens per byte | Minutes per run |
|---|---|---|---|---|---|---|---|---|
| 1 | gpt-6-luna medium | 12 | 41 | 1,288 | 212,056 | 34,898 | 1,660 | 2.5 |
| 2 | gpt-6.1-sol high | 3 | 50 | 4,200 | 101,259 | 13,428 | 255 | 5.0 |

Last hour: 5,488 bytes matched by workers in 15 runs.

## Alerts and errors

- 01:54 alert: regressions after sync: ['_ZN4sead12CalendarTimeC2ERKN2nn3fnd18DateTimeParametersE']

## Recent events

- 02:05 match: 25 functions, 2100 bytes, first 0x00352E20
- 02:04 match: 25 functions, 2100 bytes, first 0x00344688
- 02:03 match: 24 functions, 1152 bytes, first 0x00346E8C
- 01:57 run: t1_2 job 8632 tier 1: 0/23 matched, 35547 uncached + 6617 out, finished
- 01:55 run: t2_1 job 8577 tier 2: 25/25 matched, 21115 uncached + 1991 out, finished
- 01:54 run: t1_1 job 8631 tier 1: 24/25 matched, 18146 uncached + 2840 out, finished
- 01:54 alert: regressions after sync: ['_ZN4sead12CalendarTimeC2ERKN2nn3fnd18DateTimeParametersE']
- 01:52 stop: stop requested; finishing current jobs
- 01:52 run: t2_1 job 8576 tier 2: 25/25 matched, 33378 uncached + 2545 out, finished
- 01:52 run: t1_2 job 10078 tier 1: 0/21 matched, 24792 uncached + 4052 out, finished
- 01:51 run: t1_1 job 10077 tier 1: 0/25 matched, 26983 uncached + 3617 out, finished
- 01:48 start: slots {1: 2, 2: 1}
