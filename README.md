# sfst-python

## About

This is pre-alpha work to wrap the bare minimum features of the [SFST Tools](http://www.cis.uni-muenchen.de/~schmid/tools/SFST/) with python. I am not aware of any other options that work with the latest versions of the tools.

The "API" is very simple - I provide an equivalent to the SFST `fst-infl` program. The wrapper is written by hand in C++.

I have included a morphology file `latmor.a` to analyze Latin. I have not tested with any other languages.

The wrapper should build with both Python 2.x and 3.x. Open an issue if you find a platform where this is not the case.

## Installation

Should be as simple as:
```bash
python setup.py --install
```

## Usage

This will be no good to you unless you can code a little bit. Here is the output from the test script:
```
python test.py
Internal SFST Version 1.4.7d

gallia ->

[SFST exception - Analysis failed]

est ->

esse<V><pres><ind><active><sg><3>

omnis ->

omnis<ADJ><positive><masc><sg><nom>
omnis<ADJ><positive><masc><sg><gen>
omnis<ADJ><positive><masc><sg><voc>
omnis<ADJ><positive><masc><pl><acc><alt>
omnis<ADJ><positive><fem><sg><nom>
omnis<ADJ><positive><fem><sg><gen>
omnis<ADJ><positive><fem><sg><voc>
omnis<ADJ><positive><fem><pl><acc><alt>
omnis<ADJ><positive><neut><sg><gen>

divisa ->

dividere<V><part><perf><passive><fem><sg><nom>
dividere<V><part><perf><passive><fem><sg><abl>
dividere<V><part><perf><passive><fem><sg><voc>
dividere<V><part><perf><passive><neut><pl><nom>
dividere<V><part><perf><passive><neut><pl><acc>
dividere<V><part><perf><passive><neut><pl><voc>
divisus<ADJ><positive><fem><sg><nom>
divisus<ADJ><positive><fem><sg><abl>
divisus<ADJ><positive><fem><sg><voc>
divisus<ADJ><positive><neut><pl><nom>
divisus<ADJ><positive><neut><pl><acc>
divisus<ADJ><positive><neut><pl><voc>
```

## License

This inherits GPLv2 from SFST; see LICENSE for details.

## TODO

- Write some proper Python classes on top of the lowlevel API to parse tags etc
- The C++ code is... problematic. It could do with modernisation and better style
- Unit tests

## Contributing

Fork and send an PR.

## Acknowledgements

The bindings are based on those for [functionsimsearch](https://github.com/google/functionsimsearch), and I am grateful to [Halvar](https://github.com/thomasdullien) for his initial advice.

```
Library Author: Helmut Schmid 
Binding Author: Ben Nagy
License: GPLv2 - see LICENSE file for details

(c) 2018 Ben Nagy. All Rights Reserved.
```
