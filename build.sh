#!/bin/bash

if [[ "$2" =~ "c" ]]; then
  rm -rf build
fi

mkdir -p build
cd build

OPTIONS="" # just library by default

echo "SECOND ARG: $1"

if [[ "$1" =~ "d" ]]; then
  OPTIONS="${OPTIONS} -DTOOLBOX_DEVELOPMENT=ON"
fi

if [[ "$1" =~ "t" ]]; then
  OPTIONS="${OPTIONS} -DTOOLBOX_TEST=ON"
fi

if [[ "$1" =~ "c" ]]; then
  OPTIONS="${OPTIONS} -DTOOLBOX_TEST=ON -DTOOLBOX_COVERAGE=ON"
fi

if [ -n "${OPTIONS}" ]; then
  cmake ${OPTIONS} ..
else
  cmake ..
fi
# make -j$(nproc + 1)

echo "-----------------------"

# Print accessible targets
cmake --build . --target help
