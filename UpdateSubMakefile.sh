#!/bin/bash

ValidFlag="false"

MODULES="Init \
	Common \
	SimpleFile \
	LibCLite \
	Core \
	Main \
	Driver \
	Net \
	Test \
	Protocol \
	Command \
	UCos \
	Xui \
	Shell"

echo $1
echo $#

if [ $# -ne 1 ]
then
	echo "Usage of "
	exit -1
fi

for mod in $MODULES
do
	if [ $mod = $1 ]
	then
		ValidFlag="true"
		break
	fi
done

echo $ValidFlag
if [ $ValidFlag = "false" ]
then
	exit -1
fi

for mod in $MODULES 
do
	echo $mod
	if [ $mod != $1 ]
	then
		cp $1/makefile $mod/
	fi
done
