#!/bin/bash
pourcentage() {
	upower -d | grep percentage | cut -d" " -f 15
}

time_to_empty() {
	# en heure
	upower -d | grep time | cut -d" " -f 14
}

case $1 in
	pourcentage)
		pourcentage;;
	timeto)
		time_to_empty;;
	*)
		echo "ERROR PARAM : pourcentage or timeto"
esac
