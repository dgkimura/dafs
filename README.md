# dafs

[![Build Status](https://travis-ci.org/dgkimura/dafs.svg?branch=master)](https://travis-ci.org/dgkimura/dafs)
[![Coverage Status](https://coveralls.io/repos/github/dgkimura/dafs/badge.svg?branch=master)](https://coveralls.io/github/dgkimura/dafs?branch=master)

The dafs project is an elastic file storage service. It leverages a
decentralized, self-healing, ring-model architecture to provide a reliable
store capacity of 2^142 bytes.


## Build / Install
You will need a compiler with full C++11 support. You can then use CMake to
generate MakeFiles for this project.
```
$ mkdir build && cd build && cmake ..
$ make install && ./unittests/all_unittests
```


## Usage
Start a server...
```
$ sh contrib/create-server.sh 1 &
$ sh contrib/create-server.sh 2 &
$ sh contrib/create-server.sh 3 &
$ sh contrib/create-server.sh 4 &
$ sh contrib/create-server.sh 5 &
```

Join the servers into a cluster...
```
$ dafs-cli --port=7002 --command="join 127.0.0.1 7001"
$ dafs-cli --port=7003 --command="join 127.0.0.1 7001"
$ dafs-cli --port=7004 --command="join 127.0.0.1 7001"
$ dafs-cli --port=7005 --command="join 127.0.0.1 7001"
$ dafs-cli --port=7003 --command="list-servers"
```

Then upload a file...
```
$ echo "hello" > myfile.txt
$ dafs-cli --port=7001 --command="upload myfile.txt"
```

And download the file...
```
$ dafs-cli --port=7001 --command="list-files"
$ dafs-cli --port=7001 --command="download myfile.txt"
```

If one of the servers should become unavailable...
```
$ ps -ef | grep dafs-server | grep "port=7001" | awk '{print $2}' | xargs kill
```

Then the cluster will self-heal and the file will still be accessible through a
different server...
```
$ dafs-cli --port=7002 --command="list-files"
$ dafs-cli --port=7002 --command="list-servers"
```
