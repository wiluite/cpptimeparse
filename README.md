# cpptimeparse
### Time expression parser

This is an analogue of Python's [pytimeparse](https://github.com/wroberts/pytimeparse), but written in C++ without using regular expressions.

The original product uses regular expressions in their rather advanced capabilities, the repetition of which directly in C++ using its standard regex library is quite problematic (although probably possible).
Other than that, the current implementation of the regex is slow. In general, the topic of regular expressions in C++ is undervalued and not very popular.

On the other hand, despite the seeming lack of alternative to using this heavy tool for this topic, this is not entirely true: upon careful consideration of all the correct variants of expressions, you can
build a completely linear passage algorithm, where many incorrect cases are immediately or sequentially eliminated from work, and the correct cases are not so diverse. 
So, tests should confirm the correctness of this assumption.

Hopefully using this tool to calculate TimeDelta data types as part of the csvkit alternatives will be a high speed option.


_Prepare:_

Install GCC, cmake tools if absent. Also you should probably add python3-dev in some systems. For example (or similar):
```bash
sudo apt install cmake
sudo apt install build-essential
sudo apt install python3-dev
```
Install pybind11:
```bash
pip install pybind11
```

_Build:_
```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=path/to/pybind11 -DCMAKE_BUILD_TYPE=Release ..
make
cd ..
```

_Run:_
```bash
python test.py
```

**Source of test.py:**
```python
import sys

sys.path.append("build/")

from cpptimeparse import time_parser
from decimal import Decimal

Parser = time_parser()
if Parser.parse(" 2d, 1hr, 13 mins  02.1667891s ") == True:
    print("Print formatted time with nanoseconds:")
    print(Parser.str(time_parser.NANO))

    time_triple = Parser.time_tuple()
    print(time_triple)
    print("Print formatted time from triple (seconds, microseconds and nanoseconds):")
    print(Parser.str(time_triple[0] + time_triple[1]/1000000 + time_triple[2]/1000000000,  time_parser.NANO))

    print("Print formatted time <seconds.microseconds> converted to double:")
    print(Parser.str(Decimal(Parser.time()), time_parser.NANO))

print("Print from any double (with microseconds):")
print(Parser.str(177182.1667899, time_parser.MICRO))
print("Print from any double (with nanoseconds):")
print(Parser.str(177182.1667899, time_parser.NANO))

print("days=", Parser.days())
print("clock_hours=", Parser.clock_hours())
print("clock_minutes=", Parser.clock_minutes())
print("clock_seconds=", Parser.clock_seconds())
print("microseconds=", Parser.microseconds())
print("nanoseconds=", Parser.nanoseconds())
```

**Output:**
```
Print formatted time with nanoseconds:
2 days, 1:13:02.166789,100ns
(177182, 166789, 100)
Print formatted time from triple (seconds, microseconds, nanoseconds):
2 days, 1:13:02.166789,100ns
Print formatted time <seconds.microseconds> converted to double:
2 days, 1:13:02.166789
Print from any double (with microseconds):
2 days, 1:13:02.166790
Print from any double (with nanoseconds):
2 days, 1:13:02.166789,900ns
days= 2
clock_hours= 1
clock_minutes= 13
clock_seconds= 2
microseconds= 166789
nanoseconds= 900
```
