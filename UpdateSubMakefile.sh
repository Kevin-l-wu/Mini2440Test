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

#Parameter valided check

if [ $# -ne 1 ]
then
	echo "Usage of: UpdateSubMakefile.sh Driver"
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

echo "Ready to copy makefile to other directorys..."

if [ $ValidFlag = "false" ]
then
	echo "Usage of: UpdateSubMakefile.sh Driver"
	exit -1
fi

for mod in $MODULES 
do
#	echo $mod
	if [ $mod != $1 ]
	then
		cp $1/makefile $mod/
	fi
done

echo "Copy Success"
