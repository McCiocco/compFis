#! /usr/bin/bash

currpath=$(pwd)
makefile=".mymakefile"
changefile=".mychangefile"

if [ ! -f $makefile ]; then
    echo "Dove cazzo sta il makefile mongoloide"
    exit 1
fi

if [ ! -f $changefile ]; then
    echo "Dove cazzo sta il changefile mongoloide"
    exit 1
fi

while IFS='.' read -r line; do
    g++ -c -O3 $line
done < $changefile

toCompile=""

while IFS= read -r line; do
    toCompile+=$line".o "
done < $makefile

name="anim"

g++ $toCompile -o $name -lsfml-graphics -lsfml-window -lsfml-system

chmod +x $name

./$name