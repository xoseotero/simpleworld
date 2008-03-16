#! /bin/sh
# Prepare the project to be build in the directory build/.
#
# Use: autogen.sh [OPTIONS] PREFIX
#   PREFIX                     directory where to install
#   --release                  create a release build
#   --debug                    create a debug build

release=0
debug=0
prefix=""
directory="build/"
options="-D RUN_TESTS=ON"


usage()
{
    echo "\
Use: autogen.sh [OPTIONS] PREFIX
Prepare the project to be build in the directory build/ with PREFIX as
the install destination.

Options:
  --release                  create a release build
  --debug                    create debug information

  --help                     show this text"

    exit $1
}

parse_cmd()
{
    if [ "$1" == "--help" ]; then
	usage 0
    elif [ "$1" == "--release" ]; then
	release=1
	shift
	parse_cmd $*
    elif [ "$1" == "--debug" ]; then
	debug=1
	shift
	parse_cmd $*
    elif [ "$1" != "" ]; then
	prefix=$1
    fi

    if [ $# -gt 1 ]; then
	echo "Too many or wrong parameters."
	usage 1
    fi
}

create_build()
{
    directory=$1
    options=$2
    prefix=$3

    rm -rf ${directory}
    mkdir ${directory}
    cd ${directory}

    cmake ${options} ..

    if [ "${prefix}" != "" ]; then
	ln -s ${prefix} install_dir
    fi

    cd ..
}


# parse command line and set the options
parse_cmd $*
if [ "${prefix}" != "" ]; then
    options="${options} -D CMAKE_INSTALL_PREFIX=${prefix}"
fi
if [ "${debug}" == 1 ]; then
    options="${options} \
      -D CMAKE_BUILD_TYPE=Debug \
      -D DEBUG_MODE=ON \
      -D RUN_TESTS=ON \
      -D SQLite3_USE_INTERNAL=ON \
      -D SQLite3x_USE_INTERNAL=ON \
      -D XySSL_USE_INTERNAL=ON \
      -D getopt_USE_INTERNAL=ON"
fi
if [ "${release}" == 1 ]; then
    options="${options} \
      -D CMAKE_BUILD_TYPE=Release"
fi

create_build "${directory}" "${options}" "${prefix}"

exit 0
