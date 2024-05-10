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
```bash
pip install pybind11
```

_Build:_
```bash
mkdir build && cd build
cmake -DCMAKE_PREFIX_PATH=path/to/pybind11 ..
make
```

_Run:_
```bash
python test.py
```
