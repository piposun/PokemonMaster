#!/bin/sh

FOLDER=`pwd`
FOLDER=`dirname $FOLDER`
export FOLDER_PROJECT=$FOLDER

make --no-print-directory $@
