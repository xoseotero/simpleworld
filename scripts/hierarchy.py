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

def hierarchy(bug_id):
    while True:
        bug_id = father(bug_id)
        if bug_id:
            yield bug_id
        else:
            break


for father_id in hierarchy(BUG_ID):
    print father_id


sys.exit(0)
