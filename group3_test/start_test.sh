#!/bin/bash

DIR=`pwd`
PIP_PATH=`which pip`
PY_PATH=`which python`
CASE_PRINT_PATH=../bin/TestCase_Print

if [ ! -n "$PIP_PATH" ]; then 
    PY_PATH=`which pip3`
fi 

if [ ! -n "$PY_PATH" ]; then 
    apt install python3-pip
    PIP_PATH=`which pip3`
fi 

if [ ! -n "$PY_PATH" ]; then 
    PY_PATH=`which python`
fi 

if [ ! -n "$PY_PATH" ]; then 
    apt install python3.10
    PY_PATH=`which python3.10`
fi 

$PIP_PATH install -r requirements.txt

rm -r $CASE_PRINT_PATH

$PY_PATH UnitTest.py ../bin/richman TestCase/