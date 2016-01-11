#!/usr/bin/bash
# Reconfigure the Plato package.
# Version: 0.1.0.0, 24th September 2008.
# This script should be called when new source files are added. It performs two tasks:
# 1) Populates files called "sources.in", in a number of project directories, 
#    with the names of the source files for the respective project.
# 2) Calls autoreconf on packages with a configure.ac file but no configure.

# Check script is being run from Plato directory.
if test ! -d "Library"; then 
  echo "Please run from the Plato directory."
  exit 1
fi

# Populate source file lists.
SOURCEFILES=`find . -name 'Makefile.am' -printf '%h\n'`
for AMDIR in $SOURCEFILES; do
AMLINE=`grep -h 'RECONFIGURESOURCES = ' $AMDIR/Makefile.am`
if [ -n "$AMLINE" ]; then
pushd $AMDIR
echo -n ${AMLINE:21}_SOURCES = > sources.in
find . -name '*.cpp' -printf ' \\\n\t%p' >> sources.in
popd
fi
done

# Run autoreconf where necessary.
CONFIGFILES=`find . -name 'configure.ac' -printf '%h\n'`
for AMDIR in $CONFIGFILES; do
if [ ! -f "$AMDIR/configure" ]; then
pushd $AMDIR
autoreconf -i
popd
fi
done

# End
