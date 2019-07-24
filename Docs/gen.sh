#!/usr/bin/env bash

export PATH="$PWD:$PATH"

git clone git://github.com/mosra/m.css
cd m.css/documentation
./doxygen.py ../../Doxyfile