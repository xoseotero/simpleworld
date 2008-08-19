#! /bin/sh
# Create/Remove branches with svnmerge management.
# Note: merges must be done by hand before the deletion of the branch.

SVN="svn"
SVNMERGE="svnmerge"
CD="cd"
RM="rm"
WC="wc"

message=""
remove=0
branch_name=""


usage()
{
    echo "\
Use: svn_branch.sh [OPTIONS] BRANCH_NAME
Create (or remove) a new branch named BRANCH_NAME from the trunk.

Options:
  --message DESCRIPTION      description of the new branch
  --remove                   remove a branch
  --help                     show this text"

    exit $1
}

confirm()
{
    echo -n "$1 [y/N] "
    read answer
    if [ "$answer" != "y" -a "$answer" != "Y" ]; then
	exit 1
    fi
}

parse_cmd()
{
    if [ "$1" == "--help" ]; then
	usage 0
    elif [ "$1" == "--message" ]; then
	message=$2
	echo "1 = $1, 2 = $2"
	shift
	shift
	echo "1 = $1, 2 = $2"
	parse_cmd "$@"
    elif [ "$1" == "--remove" ]; then
	remove=1
	shift
	parse_cmd "$@"
    elif [ "$1" != "" ]; then
	branch_name=$1
    else
	usage 1
    fi

    if [ $# -gt 1 ]; then
	echo "Too many or wrong parameters."
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

check_nomerge()
{
    trunk=$1
    branch=$2

    pwd=`pwd`

    ${CD} "${trunk}"
    avail=`${SVNMERGE} avail -S "../branches/${branch}" | ${WC} -l`
    if [ $avail -ne 0 ]; then
	confirm "There are changes not merged, do you want to continue?"
    fi

    ${CD} "${pwd}"
}

create_branch()
{
    name=$1

    check_modifications "trunk"

    # branch
    text="Create branch ${name}"
    if [ "${message}" ]; then
	text="\
${text}
${message}"
    fi

    ${SVN} copy trunk/ branches/${name} && \
    ${SVN} ci -m "${text}" branches/${name} && \
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
    check_nomerge "trunk" "branches/${name}"

    # trunk
    ${CD} trunk/ && \
    ${SVNMERGE} uninit -S ../branches/${name} && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt

    ${CD} ..

    # branch
    text="Close branch ${name}"
    if [ "${message}" ]; then
	text="\
${text}
${message}"
    fi

    ${CD} branches/${name} && \
    ${SVNMERGE} uninit -S ../../trunk && \
    ${SVN} ci -F svnmerge-commit-message.txt && \
    ${RM} -f svnmerge-commit-message.txt && \
    ${CD} ../.. && \
    ${SVN} remove --force branches/${name} && \
    ${SVN} ci -m "${text}" branches/${name}
}


parse_cmd "$@"
check_root
${SVN} update
if [ $remove == 0 ]; then
    create_branch "${branch_name}"
else
    remove_branch "${branch_name}"
fi
