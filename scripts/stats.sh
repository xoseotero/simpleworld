#! /bin/bash

ALIVE_BUGS="$(dirname $0)/alive_bugs.py"
FOOD="$(dirname $0)/food.py"
ENERGY="$(dirname $0)/energy.py"
AGE="$(dirname $0)/age.py"
MUTATIONS="$(dirname $0)/mutations.sh"

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1


bugs=$(${ALIVE_BUGS} ${DATABASE} | wc -l)
food=$(${FOOD} ${DATABASE} | wc -l)
energy=$(${ENERGY} ${DATABASE})
mutations=$(${MUTATIONS} ${DATABASE} | grep "^Position" | wc -l)
age="$(${AGE} ${DATABASE})"

echo -n "Bugs = $bugs, "
echo -n "Food = $food, "
echo -n "Energy = $energy, "
echo -n "Mutations = $mutations, "
echo "Age = $age"
