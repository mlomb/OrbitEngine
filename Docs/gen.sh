#!/usr/bin/env bash

cp -rf img output/xml/

git clone git://github.com/mosra/m.css
cd m.css/documentation
./doxygen.py ../../Doxyfile