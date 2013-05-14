#!/bin/bash
## -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-
#
# Copyright (c) 2012 University of Washington
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

#
# This script calls test.py to get a list of all tests and examples.
# It then runs all of the C++ examples with full logging turned on,
# i.e. NS_LOG="*", to see if that causes any problems with the
# example.
#

cd ..
`./test.py -l >& /tmp/test.out`

while read line
do
  # search for examples, strip down $line as necessary
  if [[ "$line" == example* ]]
    then
      name=${line#example      }
      NS_LOG="*" ./waf --run "$name" >& /dev/null
      status="$?"
      echo "program $name status $status"
  fi
done < "/tmp/test.out"

rm -rf /tmp/test.out
cd utils
