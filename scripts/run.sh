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
MINFOOD=32
CYCLES=1024

#NOTHING="nothing.swo"
#NOTHING_ENERGY=64
HERBIVOROUS="herbivorous.swo"
HERBIVOROUS_ENERGY=512
BRAINLESS="brainless.swo"
BRAINLESS_ENERGY=1024

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

    # add a new bug if there are less than MINBUGS bugs
    if [ ${alive_bugs} -lt ${MINBUGS} ]; then
        #let new_bugs=${MINBUGS}-${alive_bugs}
        let new_bugs=${MINBUGS}-${alive_bugs}

        echo "Adding ${new_bugs} bugs in $i" >> ${LOGFILE}
        let new_bugs=${new_bugs}/3+1
        for j in $(seq ${new_bugs}); do
            #${SIMPLEWORLD} egg --code ${NOTHING} --energy ${NOTHING_ENERGY} ${DATABASE} >> ${LOGFILE}
            ${SIMPLEWORLD} egg --code ${HERBIVOROUS} --energy ${HERBIVOROUS_ENERGY} ${DATABASE} >> ${LOGFILE}
            ${SIMPLEWORLD} egg --code ${BRAINLESS} --energy ${BRAINLESS_ENERGY} ${DATABASE} >> ${LOGFILE}
        done

        # add food if there are less than MINFOOD food
        if [ ${food} -lt ${MINFOOD} ]; then
            let new_food=${MINFOOD}-${food}

            echo "Adding ${new_food} food in $i" >> ${LOGFILE}
            for j in $(seq ${new_food}); do
                ${SIMPLEWORLD} food --size 512 ${DATABASE} >> ${LOGFILE}
            done
        fi
    fi

    ${SIMPLEWORLD} run --cycles 1024 ${DATABASE} >> ${LOGFILE}

    ${INFO} ${DATABASE} > ${INFOFILE}
    ${STATS} ${DATABASE} >> ${STATSFILE}

    echo -ne "\r"
done

echo -n "End: "
date
