#!/bin/bash

for((i=37;i<=55;i++))
do
	j=$(($i-18))
	echo $j
	sed -i ""$i"s/^/s"$j":&/" simple.c
done

