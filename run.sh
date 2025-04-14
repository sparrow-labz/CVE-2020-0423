#!/bin/bash

make clean;
make;
adb push spray1 /data/local/tmp;
adb shell /data/local/tmp/spray1;
