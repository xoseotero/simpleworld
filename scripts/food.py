#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 2:
    print >> sys.stderr, "Database file must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]

connection = sqlite3.connect(DATABASE)

def food():
    cursor = connection.cursor()
    cursor.execute("SELECT size FROM Food;")
    for row in cursor:
        yield row[0]

for f in food():
    print f

sys.exit(0)
