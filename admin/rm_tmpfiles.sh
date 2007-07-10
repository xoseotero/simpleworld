#! /bin/sh
# Remove temp files.

find . -name "*~" -exec rm "{}" \;
find . -name "#*" -exec rm "{}" \;
find . -name "svn-commit*.tmp" -exec rm "{}" \;
