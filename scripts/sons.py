#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 3:
    print >> sys.stderr, "Database file and Bug ID must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]
BUG_ID = sys.argv[2]

connection = sqlite3.connect(DATABASE)

def sons(bug_id):
    cursor = connection.cursor()
    cursor.execute("SELECT id FROM Bug WHERE father_id = ?", (bug_id, ))
    for son in cursor:
        yield son[0]


for son_id in sons(BUG_ID):
    print son_id


sys.exit(0)
