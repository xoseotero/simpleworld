#! /bin/bash

ALIVE_BUGS="$(dirname $0)/alive_bugs.py"
MUTATIONS="$(dirname $0)/mutations.py"

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1

for bug in $($ALIVE_BUGS $DATABASE); do
    echo "Mutations of $bug:"
    $MUTATIONS $DATABASE $bug
    echo
done
