#! /usr/bin/env python

import sys
import sqlite3

if len(sys.argv) < 2:
    print >> sys.stderr, "Database file must be passed"
    sys.exit(1)

DATABASE = sys.argv[1]

connection = sqlite3.connect(DATABASE)

def bug_energy():
    cursor = connection.cursor()
    cursor.execute("SELECT SUM(energy) FROM Bug_alive;")
    energy = cursor.fetchone()[0]
    return energy if energy != None else 0

def code_size():
    cursor = connection.cursor()
    cursor.execute("SELECT SUM(size) FROM Code WHERE bug_id IN (SELECT id FROM Bug_alive);")
    size = cursor.fetchone()[0]
    return size if size != None else 0

def food_size():
    cursor = connection.cursor()
    cursor.execute("SELECT SUM(size) FROM Food;")
    size = cursor.fetchone()[0]
    return size if size != None else 0

def energy():
    bug = bug_energy()
    code = code_size()
    food = food_size()

    return bug + code + food

print energy()

sys.exit(0)
