#! /bin/sh
# Set the SVN properties for source files.

for file in $*; do
	svn propset svn:mime-type text/plain ${file}
	svn propset svn:eol-style native ${file}
	svn propset svn:keywords "Date Author Id Revision HeadURL" ${file}
done
