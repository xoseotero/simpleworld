#! /bin/sh

find . -name "*~" -exec rm "{}" \;
find . -name "#*" -exec rm "{}" \;
