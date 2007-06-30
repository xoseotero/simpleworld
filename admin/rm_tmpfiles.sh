#! /bin/sh
# Remove temp files.

find . -name "*~" -exec rm "{}" \;
find . -name "#*" -exec rm "{}" \;
