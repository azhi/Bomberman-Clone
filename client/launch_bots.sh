#!/bin/sh

for i in $(seq 1 $2)
do
  ./bomberman $1 --bot 2> /dev/null &
  echo "spawned $i"
  sleep 0.5
done
