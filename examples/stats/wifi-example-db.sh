#!/bin/sh

DISTANCES="25 50 75 100 125 145 147 150 152 155 157 160 162 165 167 170 172 175 177 180"
TRIALS="1 2 3 4 5"

echo WiFi Experiment Example

pCheck=`which sqlite3`
if [ -z "$pCheck" ]
then
  echo "ERROR: This script requires sqlite3 (wifi-example-sim does not)."
  exit 255
fi

pCheck=`which gnuplot`
if [ -z "$pCheck" ]
then
  echo "ERROR: This script requires gnuplot (wifi-example-sim does not)."
  exit 255
fi

pCheck=`which sed`
if [ -z "$pCheck" ]
then
  echo "ERROR: This script requires sed (wifi-example-sim does not)."
  exit 255
fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:bin/

if [ -e ../../data.db ]
then
  echo "Kill data.db? (y/n)"
  read ANS
  if [ "$ANS" = "yes" -o "$ANS" = "y" ]
  then
    echo Deleting database
    rm ../../data.db
  fi
fi

for trial in $TRIALS
do
  for distance in $DISTANCES
  do
    echo Trial $trial, distance $distance
    ../../waf --run "wifi-example-sim --format=db --distance=$distance --run=run-$distance-$trial"
  done
done

#
#Another SQL command which just collects raw numbers of frames receved.
#
#CMD="select Experiments.input,avg(Singletons.value) \
#    from Singletons,Experiments \
#    where Singletons.run = Experiments.run AND \
#          Singletons.name='wifi-rx-frames' \
#    group by Experiments.input \
#    order by abs(Experiments.input) ASC;"

mv ../../data.db .

CMD="select exp.input,avg(100-((rx.value*100)/tx.value)) \
    from Singletons rx, Singletons tx, Experiments exp \
    where rx.run = tx.run AND \
          rx.run = exp.run AND \
          rx.name='receiver-rx-packets' AND \
          tx.name='sender-tx-packets' \
    group by exp.input \
    order by abs(exp.input) ASC;"

sqlite3 -noheader data.db "$CMD" > wifi-default.data
sed -i "s/|/   /" wifi-default.data
gnuplot wifi-example.gnuplot

echo "Done; data in wifi-default.data, plot in wifi-default.eps"
