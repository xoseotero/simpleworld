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
SELECT Bug.id as id, AliveBug.energy as energy, length(code) as size,
       World.position_x as x, World.position_y as y,
       (SELECT MAX(time) FROM Environment) - AliveBug.birth as age,
       COUNT(Mutation.bug_id) as mutations
FROM Bug
LEFT JOIN AliveBug
ON Bug.id = AliveBug.bug_id
LEFT JOIN World
ON World.id = AliveBug.world_id
LEFT JOIN Mutation
ON Bug.id = Mutation.bug_id
WHERE Bug.id IN (SELECT bug_id FROM AliveBug)
GROUP BY Bug.id
ORDER BY AliveBug.energy DESC;"


# Bugs ordered by age
sql_age="\
SELECT bug_id as id, (SELECT max(time) FROM Environment) - birth AS age
FROM AliveBug
ORDER BY age DESC;"

# Bugs with more sons
sql_sons="\
SELECT father_id, count(father_id) AS sons
FROM Bug
WHERE father_id IS NOT NULL AND father_id IN (SELECT bug_id
                                              FROM AliveBug)
GROUP BY father_id
ORDER BY sons DESC;"


# Bugs with more kills
sql_kills="\
SELECT killer_id, count(killer_id) AS kills
FROM DeadBug
WHERE killer_id IS NOT NULL AND killer_id IN (SELECT bug_id
                                              FROM AliveBug)
GROUP BY killer_id
ORDER BY kills DESC;"

# Bugs with more mutations
sql_mutations="\
SELECT bug_id, count(position) AS mutations
FROM Mutation
WHERE bug_id IN (SELECT bug_id
                 FROM AliveBug)
GROUP BY bug_id
ORDER BY mutations DESC;"


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
