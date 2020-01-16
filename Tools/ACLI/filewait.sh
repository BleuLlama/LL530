#!/bin/sh
#
#  2020-01-15 - added minsleep support

timeout=10


echo "waiting for -e $1.. " > /dev/stderr

minsleep=$2
echo "$minsleep"

while [ $minsleep != 0 ]
do
	echo "Sleeping ... $minsleep sec";
	sleep 1;
	((minsleep--))
done

while [ ! -e "$1" ];
do
	if [ $timeout == 1 ]; 
	then
		echo "Timed out." > /dev/stderr; 
		exit 1;
	fi
	((timeout--))
	echo "Waiting... $timeout seconds" > /dev/stderr ; 
	sleep 1;
done

