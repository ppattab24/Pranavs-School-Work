#!/bin/sh

g++ fork.cpp -o testFork
g++ exec.cpp -o testExec
./testExec
rm testFork
rm testExec
