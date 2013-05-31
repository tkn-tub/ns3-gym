#!/bin/bash


# Copyright (c) 2013 Lawrence Livermore National Laboratory
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation;
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
# Author: Peter D. Barnes, Jr. <pdbarnes@llnl.gov>

#
#  Do a coverity build and submit report
#

me=`basename $0`

# echo commands and output to a log file

logf=coverity/coverity-build.log
echo | tee $logf

function say ()
{
    echo "$me:" $* | tee -a $logf
}
blank ()
{
    echo | tee -a $logf
}
function doo ()
{
    say "$ "$*
    $* 2>&1 | tee -a $logf
}




say $(date)
blank

doo ./waf clean
blank

doo ./waf configure $NS3CONFIG
blank

cov=coverity/cov-int
doo cov-build --dir $cov ./waf build
blank

tarf=coverity/ns-3.tgz
doo tar cvzf $tarf -C coverity cov-int
blank

useremail=$(hg showconfig ui.username | \
    egrep -o "\b[a-zA-Z0-9.-]+@[a-zA-Z0-9.-]+\.[a-zA-Z0-9.-]+\b")

repoversion="$(basename $(dirname $PWD))@$(hg id -i)"

# curl complains if this contains white space
description="Coverity-mods"
    
doo curl \
     --form file=@$tarf \
     --form project=ns-3 \
     --form password=4jk2BVX9 \
     --form email="$useremail" \
     --form version="$repoversion" \
     --form description="$description" \
     http://scan5.coverity.com/cgi-bin/upload.py
blank

say $(date)
blank
