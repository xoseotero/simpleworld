#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 2:
    print >> sys.stderr, "Database file must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]

connection = sqlite3.connect(DATABASE)

def age():
    cursor = connection.cursor()
    cursor.execute("SELECT SUM((SELECT MAX(time) FROM Environment) - birth) FROM Bug_alive;")
    size = cursor.fetchone()[0]
    return size if size != None else 0

print age()

sys.exit(0)
