#!/bin/bash

# Get the current repo version
# and format appropriately as a
# Javascript variable for inclusion in html files

# Distance from last tag
# Zero distance means we're at the tag
distance=`hg log -r tip --template '{latesttagdistance}'`

if [ $distance -eq 0 ]; then
    version=`hg log -r tip --template '{latesttag}'`
else
    repo=`basename $PWD`
    version=`hg log -r tip --template "$repo @ {node|short}"`
fi

jsver="var ns3_version = \"$version\";"
echo $jsver > doc/ns3_html_theme/static/ns3_version.js

# Copy to html directories
# This is done automatically by the Doxygen and Sphinx build steps
cd doc
for d in {manual,models,tutorial{,-pt-br}}/build/html/_static html ; do
    cp ns3_html_theme/static/ns3_version.js $d
done

# Show what was done
echo ns-3 javascript version: $jsver
