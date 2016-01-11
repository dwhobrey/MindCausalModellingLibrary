#!/usr/bin/bash
# Build the Plato documentation.
# Version: 0.1.0.0, 24th September 2008.
# The script should be run from the Plato project directory.
# The resulting documentation is placed in Plato/Documentation/tmp/html.
# This script requires at least doxygen (accessible in the PATH),
# and a help file compiler if a single compiled help file is required,
# in which case the HHC variable should be set accordingly.

PLATOVERSION="Version 0.1.0.0, "`date`
TMPDOCS=./tmp/html
# HHC="C:\Program Files\HTML Help Workshop\hhc.exe"
# The above Windows path needs to be converted to UNIX form, thus:
HHC="/cygdrive/c/Program Files/HTML Help Workshop/hhc"

# Check script is being run from Plato directory.
if test ! -d "Documentation"; then 
  echo "Please run from the Plato directory."
  exit 1
fi

pushd Documentation

# Clean up first.
rm -f -r tmp

# Build the API docs
if ! doxygen plato.doxygen.config; then
echo Running doxygen failed: check it is installed correctly.
popd
exit 1
fi

# Copy additional docs to tmp dir.
cp -f -r docs $TMPDOCS

# Update version number.
sed -i -e "s/PLATOVERSION/$PLATOVERSION/g" $TMPDOCS/docs/Plato.html

# Patch the website files: index.html etc.

cp -f patches/index.html $TMPDOCS
sed -i -e '/<div style="display: block;">/ r patches/tree.html.stub' $TMPDOCS/tree.html

# Patch the Help project files: index.hhp etc.

# Add table of content entries for additional docs to help TOC file. 
sed -i -e '/^<UL>/ r patches/index.hhc.stub' $TMPDOCS/index.hhc

# Add additional files to help project file.
cp -f patches/index.hhp.stub $TMPDOCS/index.hhp
find docs -name '*.*' -printf '%p\n' >> $TMPDOCS/index.hhp
pushd $TMPDOCS
ls *.html >> index.hhp
ls *.css >> index.hhp
ls *.gif >> index.hhp

# Compile html into a chm using a Help compiler.
if ! ( "$HHC" index.hhp ); then
 if test ! -f "Plato.chm"; then
 echo Running help compiler failed: check it is installed correctly.
 fi
fi

popd
popd

