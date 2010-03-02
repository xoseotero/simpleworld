#! /bin/bash

MUTATIONS="$(dirname $0)/mutations.sh"

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1


species=$(${MUTATIONS} ${DATABASE} 2> /dev/null | grep -A 1 "^Mutations" | grep "^Position" | sort | uniq | wc -l)

echo "Species = $species"
