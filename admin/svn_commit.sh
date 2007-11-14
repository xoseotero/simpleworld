#! /bin/sh
# Add diffstat information to the commits.

SVN=svn
DIFFSTAT=diffstat
TEMPFILE=commit.txt
EDITOR=nano
RM=rm

echo "comment for the commit here" > ${TEMPFILE}
echo >> ${TEMPFILE}
echo " -------------------" >> ${TEMPFILE}

${SVN} diff $* | ${DIFFSTAT} >> ${TEMPFILE}
${EDITOR} ${TEMPFILE}
${SVN} ci -F ${TEMPFILE} $*
${RM} ${TEMPFILE}
