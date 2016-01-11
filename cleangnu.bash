#!/usr/bin/bash
# Clean Plato gnu source ready for archiving.

# Check script is being run from Plato directory.
if test ! -d "Library"; then 
  echo "Please run from the Plato directory."
  exit 1
fi

# Function to clean autoconf files from a program dir.
cleandir() {
pushd $1
rm -f -r autom4te.cache
rm -f -r build
rm -f aclocal.m4
rm -f configure
rm -f depcomp
rm -f install-sh
rm -f Makefile.in
rm -f missing
rm -f sources.in
popd
}

# Clean Plato.
cleandir .

# Clean PCRE.
cleandir Packages/pcre

# Clean the Plato Library.
cleandir Library

# Clean the Models.
pushd Models
MODELS=`ls`
#MODELS=HelloWorldModel
for MODEL in $MODELS; do
cleandir $MODEL
done
popd

# End
