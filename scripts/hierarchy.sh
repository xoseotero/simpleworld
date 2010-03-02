#! /bin/bash

ALIVE_BUGS=$(dirname $0)/alive_bugs.py
HIERARCHY=$(dirname $0)/hierarchy.py

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1

for bug in $($ALIVE_BUGS $DATABASE); do
    echo "Hierarchy of $bug:"
    $HIERARCHY $DATABASE $bug
    echo
done
