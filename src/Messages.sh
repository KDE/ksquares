#! /bin/sh
$EXTRACTRC `find . -name \*.ui -o -name \*.rc` >> rc.cpp || exit 11
$XGETTEXT *.cpp -o $podir/ksquares.pot
rm -f rc.cpp
