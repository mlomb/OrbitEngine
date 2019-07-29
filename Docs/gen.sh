#!/usr/bin/env bash

# copy images directly to the output
cp -a /img/. /output/html/

git clone git://github.com/mosra/m.css
cd m.css/documentation
./doxygen.py ../../Doxyfile