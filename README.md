# dafs

[![Build Status](https://travis-ci.org/dgkimura/dafs.svg?branch=master)](https://travis-ci.org/dgkimura/dafs)
[![Coverage Status](https://coveralls.io/repos/github/dgkimura/dafs/badge.svg?branch=master)](https://coveralls.io/github/dgkimura/dafs?branch=master)

The dafs project is an elastic file storage service.


## Build
You will need a compiler with full C++11 support. You can then use CMake to
generate MakeFiles for this project.
```
$ mkdir build && cd build
$ cmake ..
$ make && ./unittests/all_unittests
```


## Setup
You can start the server by providing an identity for the server.

```
$ dafs-server --identity=11111111-1111-1111-1111-111111111111
```

You can get information about a server using the command-line tool.

```
$ dafs-cli --port=9001 --address=127.0.0.1
dafs> list-servers
Management          Minus Interface     Zero Interface      Plus Interface      Identity
127.0.0.1:9001      127.0.0.1:8070      127.0.0.1:8080      127.0.0.1:8090      11111111-1111-1111-1111-111111111111
dafs> list-files
<empty>
```


## Design
A server is a member of 3 partitions.
