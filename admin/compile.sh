#! /bin/sh
# Compile, build and install Simple World.
# Arguments passed to compile.sh are passed to autogen.sh

rm -rf build/ && admin/autogen.sh $* && cd build/ && make && make test && make install
