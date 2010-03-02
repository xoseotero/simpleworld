#! /bin/bash

old_time=""

while true; do
	time=$(ls --full-time info | awk '{print $7}')
	if [ "$time" != "$old_time" ]; then
		old_time=$time

		clear
		head -n 55 info | tail -n 24
	fi

	sleep 1s
done
