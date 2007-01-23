#! /bin/sh
# The prefix path can be passed as argument optionally

mkdir build
cd build

if [ "$1" != "" ]; then
	cmake -D CMAKE_INSTALL_PREFIX=$1 ..
else
	cmake ..
fi
