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
```


# Setup
Create a `config.ini` file describing the partitions on the node. Each node
references 3 partitions.

```
    [partition-minus]
    port = 8070

    [partition-zero]
    port = 8080

    [partition-plus]
    port = 8090
```

Now start the server specifying an identity and a management endpoint.

```
$ dafs-server --identity=11111111-1111-1111-1111-111111111111 --address=127.0.0.1 --port=9000
```
