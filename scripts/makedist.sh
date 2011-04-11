#!/bin/sh

# Copied from cclive project (2011-01-07).

# This script will commit ChangeLog temporarily so that git-archive
# picks the updated ChangeLog to the tarball.

project=nomnom
branch=master

echo "WARNING!

Please note that this script will call git-reset (--hard) during
this process so be sure to commit your changes before you run this
script.

Last chance to bail out (CTRL-C) before we call 'git reset --hard'."
read -s -n1

descr=`git describe $branch`
basename="$project-$descr"
tarball="$basename.tar.gz"

echo ":: Generate $tarball"

# TODO: Elegant way.
git log --stat >ChangeLog && \
git commit -q ChangeLog -m "Generate $tarball" && \
git archive $branch --prefix="$basename/" | gzip >$tarball && \
git reset -q --hard HEAD~1
