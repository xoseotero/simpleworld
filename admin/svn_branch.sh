#! /bin/sh
# Create/Remove branches with svnmerge management.
# Note: merges must be done by hand before the deletion of the branch.

SVN="svn"
SVNMERGE="svnmerge"
CD="cd"
RM="rm"
WC="wc"

remove=0
branch_name=""


usage()
{
    echo "\
Use: svn_branch.sh [OPTIONS] BRANCH_NAME
Create (or remove) a new branch named BRANCH_NAME from the trunk.

Options:
  --remove                   remove a branch
  --help                     show this text"

    exit $1
}

parse_cmd()
{
    if [ "$1" == "--help" ]; then
	echo 1
	usage 0
    elif [ "$1" == "--remove" ]; then
	remove=1
	shift
	parse_cmd $*
    elif [ "$1" != "" ]; then
	branch_name=$1
    else
	echo 2
	usage 1
    fi

    if [ $# -gt 1 ]; then
	echo "Too many or wrong parameters."
	echo 3
	usage 1
    fi
}

check_root()
{
    if [ -d "trunk" -a -d "branches" ]; then
	echo -n ""
    else
	echo "You must be in the root of the project."
	exit 1
    fi
}

check_modifications()
{
    directory=$1

    diff=`${SVN} diff ${directory} | ${WC} -l`
    if [ $diff -ne 0 ]; then
	echo "Directory ${directory} has changes."
	exit 1
    fi
}

create_branch()
{
    name=$1

    check_modifications "trunk"

    # branch
    ${SVN} copy trunk/ branches/${name} && \
    ${SVN} ci -m "Create branch ${name}" branches/${name} && \
    ${CD} branches/${name} && \
    ${SVNMERGE} init ../../trunk && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt

    ${CD} ../..

    # trunk
    ${CD} trunk/ && \
    ${SVNMERGE} init ../branches/${name} && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt
}

remove_branch()
{
    name=$1

    check_modifications "trunk"
    check_modifications "branches/${name}"

    # trunk
    ${CD} trunk/ && \
    ${SVNMERGE} uninit -S ../branches/${name} && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt

    ${CD} ..

    # branch
    ${CD} branches/${name} && \
    ${SVNMERGE} uninit -S ../../trunk && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt && \
    ${CD} ../.. && \
    ${SVN} remove --force branches/${name} && \
    ${SVN} ci -m "branch is now closed" branches/${name}
}


parse_cmd $*
check_root
if [ $remove == 0 ]; then
    create_branch "${branch_name}"
else
    remove_branch "${branch_name}"
fi
