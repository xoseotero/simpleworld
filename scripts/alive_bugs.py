#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 2:
    print >> sys.stderr, "Database file must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]

connection = sqlite3.connect(DATABASE)

def alive_bugs():
    cursor = connection.cursor()
    cursor.execute("SELECT id FROM Bug_alive;")
    for row in cursor:
        yield row[0]

for bug in alive_bugs():
    print bug

sys.exit(0)
