# dafs

[![Build Status](https://travis-ci.org/dgkimura/dafs.svg?branch=master)](https://travis-ci.org/dgkimura/dafs)
[![Coverage Status](https://coveralls.io/repos/github/dgkimura/dafs/badge.svg?branch=master)](https://coveralls.io/github/dgkimura/dafs?branch=master)

The dafs project is an elastic file storage service. It can store
5,575,186,299,632,655,785,383,929,568,162,090,376,495,104 bytes (55 billion
yotta bytes) of unique data.


## Design
The dafs project uses a decentralized, self-healing ring-model architecture.
What this means is that no single entity knows about the entire system and that
each entity is fault-tolerant. These features allow the system to scale-out
with minimal maintenance cost.

Files in the system are represented as a collection of blocks. Blocks are
replicated on multiple servers through a [replication library](https://github.com/dgkimura/paxos).
Each block and each server have unqiue identifiers. Given that an identifier can
be represented as a number, then each block is guaranteed to most closely match
a single server. What this means is that the system can be thought of as a map
of blocks to servers and if we know the identity of the servers then we also
know where any given block is located.

A client knows how the blocks are used to represent a file. This decoupling
means that a server has no concept of a file. Instead a server is only concerned
with ensuring that blocks are safely replicated and returned to the client on
request.


## Build
You will need a compiler with full C++11 support. You can then use CMake to
generate MakeFiles for this project.
```
$ mkdir build && cd build
$ cmake ..
$ make && ./unittests/all_unittests
```


## Usage
Start a server...
```
$ dafs-server --address=127.0.0.1 --port=9000
```

Then upload a file...
```
$ echo "hello" > myfile.txt
$ dafs-cli --address=127.0.0.1 --port=9000 --command="upload myfile.txt"
```

And download the file...
```
$ rm myfile.txt
$ dafs-cli --address=127.0.0.1 --port=9000 --command="download myfile.txt"
```
