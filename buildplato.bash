#!/usr/bin/bash
# Build the Plato components.
# Version: 0.1.0.0, 24th Septmber 2008.
# This script simply calls configure (from a build subdir) and make.

# Check script is being run from Plato directory.
if test ! -d "Library"; then 
  echo "Please run from the Plato directory."
  exit 1
fi

# Function to configure and make package.
build() {
 pushd $1
 mkdir -p build
 cd build
 if ! ( ../configure && make ); then
  popd
  echo "build: Could not build "$1". Error 1"
  return 1
 fi
 popd
}

# Build PCRE.
if ! ( build Packages/pcre ); then
 exit 1
fi

# Build the Plato Library.
if ! ( build Library ); then
 exit 1
fi

# Build the Models.
pushd Models
#MODELS=`ls`
MODELS=HelloWorldModel
for MODEL in $MODELS; do
  build $MODEL
done
popd

# End
