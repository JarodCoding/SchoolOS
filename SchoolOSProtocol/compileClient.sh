#!/bin/sh
#Requires protocol Buffers (developers.google.com/protocol-buffers/)
protoc -I=/home/jarod/SchoolOS/SchoolOSProtocol/Client --cpp_out=/home/jarod/SchoolOS/SchoolOSProtocol/Client/CPP /home/jarod/SchoolOS/SchoolOSProtocol/Client/User/Login.proto
protoc -I=/home/jarod/SchoolOS/SchoolOSProtocol/Client --cpp_out=/home/jarod/SchoolOS/SchoolOSProtocol/Client/CPP /home/jarod/SchoolOS/SchoolOSProtocol/Client/Data/Files.proto
