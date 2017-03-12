dafs
====

[![Build Status](https://travis-ci.org/dgkimura/dafs.svg?branch=master)](https://travis-ci.org/dgkimura/dafs)

The dafs project is a highly-scalable distributed file storage service.


## Build
You will need a compiler with full C++11 support. You can then use CMake to
generate MakeFiles for this project.
```
$ mkdir build && cd build
$ cmake ..
$ make && ./unittests/all_unittests
