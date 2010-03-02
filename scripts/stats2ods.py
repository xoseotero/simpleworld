#! /usr/bin/env python

import sys
import re

import ooolib

def read_stats(filename):
    values = []
    stats = open(filename)

    for line in stats:
        bugs = re.search("Bugs = ([0-9]+)", line)
        if bugs:
            bugs = bugs.groups()[0]
        food = re.search("Food = ([0-9]+)", line)
        if food:
            food = food.groups()[0]
        energy = re.search("Energy = ([0-9]+)", line)
        if energy:
            energy = energy.groups()[0]
        mutations = re.search("Mutations = ([0-9]+)", line)
        if mutations:
            mutations = mutations.groups()[0]
        age = re.search("Age = ([0-9]+)", line)
        if age:
            age = age.groups()[0]

        values.append((bugs, food, energy, mutations, age))

    stats.close()

    return values

def save_ods(filename, stats):
    doc = ooolib.Calc()

    row = 1

    # header
    doc.set_cell_property('bold', True)
    doc.set_cell_value(1, row, "string", "Bugs")
    doc.set_cell_value(2, row, "string", "Food")
    doc.set_cell_value(3, row, "string", "Energy")
    doc.set_cell_value(4, row, "string", "Mutations")
    doc.set_cell_value(5, row, "string", "Age")
    doc.set_cell_property('bold', False)

    # data
    for line in stats:
        row += 1
        if line[0]:
            doc.set_cell_value(1, row, "float", line[0])
        else:
            doc.set_cell_value(1, row, "formula", "=0/0")
        if line[1]:
            doc.set_cell_value(2, row, "float", line[1])
        else:
            doc.set_cell_value(2, row, "formula", "=0/0")
        if line[2]:
            doc.set_cell_value(3, row, "float", line[2])
        else:
            doc.set_cell_value(3, row, "formula", "=0/0")
        if line[3]:
            doc.set_cell_value(4, row, "float", line[3])
        else:
            doc.set_cell_value(4, row, "formula", "=0/0")
        if line[4]:
            doc.set_cell_value(5, row, "float", line[4])
        else:
            doc.set_cell_value(5, row, "formula", "=0/0")

    doc.save(filename)

if len(sys.argv) < 3:
    print >> sys.stderr, "Stats file and output ods must be passed"
    sys.exit(1)

INPUT = sys.argv[1]
OUTPUT = sys.argv[2]

save_ods(OUTPUT, read_stats(INPUT))
