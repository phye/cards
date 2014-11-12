#!/bin/sh
mkdir -p m4
touch NEWS README AUTHORS ChangeLog COPYING
libtoolize
export LIBTOOL=/usr/bin/libtool ; autoreconf --force --install -I m4
#rm NEWS README AUTHORS ChangeLog COPYING INSTALL
