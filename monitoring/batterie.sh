#!/bin/bash

# @param type p:% t:time

pourcentage() {
	upower -d | grep percentage | cut -d" " -f 15
}

time_to_empty() {
	# en heure
	upower -d | grep time | cut -d" " -f 14
}

case $1 in
	p)
		pourcentage;;
	t)
		time_to_empty;;
	*)
		echo "ERROR PARAM : p or t"
esac
