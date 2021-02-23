#!/bin/sh

CLEAN=0
REBUILD=0

# args
while [ ! -z $1 ];
do
    if [ "$1" == "CLEAN" ]; then
	    CLEAN=1
    elif [ "$1" == "REBUILD" ]; then
	    REBUILD=1
    fi
    
    shift
done

# clean build
if [ $CLEAN -eq 1 ]; then
    echo "cleaning build dir"
    rm -rf ./build
    REBUILD=1
fi

# verify build dir
if [ ! -d "./build" ]; then
    echo "recreating build dir"
    mkdir ./build
fi

pushd ./build

# rebuild cmake
if [ $REBUILD -eq 1 ]; then
    echo "rebuilding cmake project"
    cmake ../
fi

echo "building project"
make

popd > /dev/null
