#!/bin/sh

timeout=5

echo "waiting for $@.. " > /dev/stderr
while [ ! -e "$@" ];
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

