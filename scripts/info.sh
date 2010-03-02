#! /bin/bash

SQLITE="/usr/bin/sqlite3 -header -column -nullvalue NULL"

if [ $# -lt 1 ]; then
    echo "Database file must be passed"
    exit 1
fi

DATABASE=$1


# Environment
sql_env="\
SELECT *
FROM Environment
WHERE time = (SELECT max(time)
              FROM Environment);"


# Bugs alive
sql_alive="\
SELECT Bug.id as id, energy, position_x, position_y,
       (SELECT MAX(time) FROM Environment) - birth as age,
       COUNT(Mutation.bug_id) as mutations
FROM Bug
LEFT JOIN Mutation
ON Bug.id = Mutation.bug_id
WHERE dead IS NULL
GROUP BY Bug.id
ORDER BY energy DESC;"


# Bugs ordered by age
sql_age="\
SELECT id, (SELECT max(time) FROM Environment) - birth AS age
FROM Bug
WHERE dead IS NULL

UNION

SELECT id, dead - birth AS age
FROM Bug
WHERE dead IS NOT NULL

ORDER BY age DESC

LIMIT 100;"

# Bugs with more sons
sql_sons="\
SELECT father_id, count(father_id) AS sons
FROM Bug
WHERE father_id IS NOT NULL
GROUP BY father_id
ORDER BY sons DESC
LIMIT 100;"


# Bugs with more kills
sql_kills="\
SELECT killer_id, count(killer_id) AS kills
FROM Bug
WHERE killer_id IS NOT NULL
GROUP BY killer_id
ORDER BY kills DESC
LIMIT 100;"

# Bugs with more mutations
sql_mutations="\
SELECT bug_id, count(position) AS mutations
FROM Mutation
GROUP BY bug_id
ORDER BY mutations DESC
LIMIT 100;"


# sql_all="\
# SELECT Bug.id, Bug.energy, Bug.position_x, Bug.position_y,
#        (SELECT max(time) FROM Environment) - Bug.birth AS age, Bug.father_id, Bug.killer_id,
#        Mutation.position, Mutation.original, Mutation.mutated,
#        Code.size
# FROM Bug
# LEFT OUTER JOIN Mutation
#   ON Bug.id=Mutation.bug_id
# LEFT OUTER JOIN Code
#   ON Bug.id=Code.bug_id
# WHERE dead IS NULL

# UNION

# SELECT Bug.id, Bug.energy, Bug.position_x, Bug.position_y,
#        Bug.dead - Bug.birth AS age, Bug.father_id, Bug.killer_id,
#        Mutation.position, Mutation.original, Mutation.mutated,
#        Code.size
# FROM Bug
# LEFT OUTER JOIN Mutation
#   ON Bug.id=Mutation.bug_id
# LEFT OUTER JOIN Code
#   ON Bug.id=Code.bug_id
# WHERE dead IS NOT NULL;"


${SQLITE} -line ${DATABASE} "${sql_env}"
echo "     ----------     ---------     ---------     ----------     "
echo

# for sql in "${sql_alive}" "${sql_age}" "${sql_sons}" "${sql_kills}" \
#            "${sql_mutations}" "${sql_all}"; do
for sql in "${sql_alive}" "${sql_age}" "${sql_sons}" "${sql_kills}" \
           "${sql_mutations}"; do
    echo "${sql}"
    echo
    ${SQLITE} ${DATABASE} "${sql}"
    echo "     ----------     ---------     ---------     ----------     "
    echo
done
