#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 3:
    print >> sys.stderr, "Database file and Bug ID must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]
BUG_ID = sys.argv[2]

connection = sqlite3.connect(DATABASE)

def father(bug_id):
    cursor = connection.cursor()
    cursor.execute("SELECT father_id FROM Bug WHERE id = ?", (bug_id, ))
    data = cursor.fetchone()
    if data is None:
        return None
    else:
        return data[0]

def birth(bug_id):
    cursor = connection.cursor()
    cursor.execute("SELECT birth FROM AliveBug WHERE bug_id = ? UNION SELECT birth FROM DeadBug WHERE bug_id = ?", (bug_id, bug_id))
    data = cursor.fetchone()
    return data[0]

def mutations(bug_id, total=False):
    cursor = connection.cursor()
    if total:
        cursor.execute("SELECT time, position, original, mutated FROM Mutation WHERE bug_id = ?", (bug_id, ))
    else:
        cursor.execute("SELECT time, position, original, mutated FROM Mutation WHERE bug_id = ? AND time < ?", (bug_id, birth(bug_id)))
    for mutation in cursor:
        if mutation[2] is None:
            # addition
            print "Position %d(%d):\tNULL\t->\t0x%08x" % (mutation[1],
                                                          mutation[0],
                                                          mutation[3])
        elif mutation[3] is None:
            # deletion
            print "Position %d(%d):\t0x%08x\t->\tNULL" % (mutation[1],
                                                          mutation[0],
                                                          mutation[2])
        else:
            # change
            print "Position %d(%d):\t0x%08x\t->\t0x%08x" % (mutation[1],
                                                            mutation[0],
                                                            mutation[2],
                                                            mutation[3])
    father_id = father(bug_id)
    if father_id:
        mutations(father_id, False)


mutations(BUG_ID, True)


sys.exit(0)
