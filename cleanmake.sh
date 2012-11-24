#!/bin/bash

find . -type f -name "*.o" -exec rm -rf {} \;
find . -type f -name "*.d" -exec rm -rf {} \;

make