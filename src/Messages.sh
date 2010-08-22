#! /bin/sh
$EXTRACTRC `find . -name \*.ui -o -name \*.rc -o -name \*.kcfg` >> rc.cpp || exit 11
$XGETTEXT *.cpp -o $podir/ksquares.pot
rm -f rc.cpp
