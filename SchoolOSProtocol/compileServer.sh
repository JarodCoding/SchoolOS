#!/bin/sh
#Warning Requires protocol Buffers (developers.google.com/protocol-buffers/)
protoc -I=/Server --cpp_out=/Server/CPP /Server/User/Login.proto
protoc -I=/Server --cpp_out=/Server/CPP /Server/Data/Files.proto
