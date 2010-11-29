#! /bin/bash

SIMPLEWORLD="/home/xose/Binarios/simpleworld/bin/simpleworld"
#SIMPLEWORLD="nice -20 /home/xose/Binarios/simpleworld/bin/simpleworld"
LOGFILE="$(dirname $0)/log"
#LOGFILE="/dev/null"
FOOD="$(dirname $0)/food.py"
ALIVE_BUGS="$(dirname $0)/alive_bugs.py"
INFO="$(dirname $0)/info.sh"
INFOFILE="$(dirname $0)/info"
#INFOFILE="/dev/null"
STATS="$(dirname $0)/stats.sh"
STATSFILE="$(dirname $0)/stats"
#STATSFILE="/dev/null"

MINBUGS=32
MINBUGSFOOD=64
CYCLES=1024

NOTHING="nothing.swo"
BRAINLESS="brainless.swo"

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1


if [ $# -lt 2 ]; then
    RUNS=1
else
    RUNS=$2
fi


echo -n "Begin: "
date

for i in $(seq ${RUNS}); do
    echo -n "Run: ${i} of ${RUNS}"

    food=$(${FOOD} ${DATABASE} | wc -l)
    alive_bugs=$(${ALIVE_BUGS} ${DATABASE} | wc -l)

    # add a new bug if there are less than 32 bugs (1/8 of the world)
    if [ ${alive_bugs} -lt ${MINBUGS} ]; then
        let new_bugs=${MINBUGS}-${alive_bugs}
        #let new_bugs=(${MINBUGS}-${alive_bugs})/2

        echo -n " Adding bugs in $i"
        echo "Adding bugs" >> ${LOGFILE}
        for j in $(seq ${new_bugs}); do
            ${SIMPLEWORLD} egg --code ${NOTHING} --position=$(random -e 16; echo $?),$(random -e 16; echo $?) --energy 256 ${DATABASE} >> ${LOGFILE} 2> /dev/null
            ${SIMPLEWORLD} egg --code ${BRAINLESS} --position=$(random -e 16; echo $?),$(random -e 16; echo $?) --energy 1024 ${DATABASE} >> ${LOGFILE} 2> /dev/null
        done
    fi

    # add food if there are less than 64 bugs (1/4 of the world) and less
    # than 64 food
    if [ ${alive_bugs} -lt 64 -a ${food} -lt 64 ]; then
        let new_food=${MINBUGSFOOD}-${alive_bugs}

        echo -n " Adding food in $i"
        echo "Adding food" >> ${LOGFILE}
        for j in $(seq ${new_food}); do
            ${SIMPLEWORLD} food --position=$(random -e 16; echo $?),$(random -e 16; echo $?) --energy 512 ${DATABASE} >> ${LOGFILE} 2> /dev/null
        done
    fi

    ${SIMPLEWORLD} run --cycles 1024 ${DATABASE} >> ${LOGFILE}

    ${INFO} ${DATABASE} > ${INFOFILE}
    ${STATS} ${DATABASE} >> ${STATSFILE}

    echo -ne "\r"
done

echo -n "End: "
date
