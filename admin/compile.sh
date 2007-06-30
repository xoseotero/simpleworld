#! /bin/sh
# Compile, build and install Simple World.
# The prefix path can be passed as argument optionally

if [ "$1" != "" ]; then
	prefix=$1
else
	prefix=""
fi

rm -rf build/ && admin/autogen.sh ${prefix} && cd build/ && make && make tests && make install
