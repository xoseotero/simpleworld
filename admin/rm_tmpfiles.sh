#! /bin/sh
# Remove temp files.

find . -name "*~" -exec rm "{}" \;
find . -name "#*" -exec rm "{}" \;
find . -name "*.swE" -exec rm "{}" \;
find . -name "*.swo" -exec rm "{}" \;
