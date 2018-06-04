#!/usr/bin/env bash

ScriptRoot=`( cd "$MY_PATH" && pwd )`
BuildRoot="$ScriptRoot/Build/Linux/"

echo "********** BUILDING FOR LINUX ON LINUX **********"

echo "Base Path: $ScriptRoot"
echo "Build Path: $BuildRoot"

mkdir -p "$BuildRoot"
cd "$BuildRoot"
cmake "$ScriptRoot"