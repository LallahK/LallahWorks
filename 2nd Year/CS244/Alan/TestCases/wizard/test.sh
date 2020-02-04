#!/bin/bash

if [ $# == 0 ]
then
	bash test.sh 1 &> something.out
	diff something.out correctoutput.out
elif [ $1 == '1' ]
then
	var=0
	for root in */
	do
		for dir in $root/*/
		do
			result="${dir%"${dir##*[!/]}"}" # extglob-free multi-trailing-/trim
			result="${result##*/}"          # remove everything before the last
			echo $result
			for file in $dir/*.alan
			do
				(( var++ ))
				#echo "TEST $var IN => " $file
				../bin/alanc $file
				#grep error $file
				#~/repos/alan/bin/alanc $file
			done
		done
	done
fi
