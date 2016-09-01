#!/bin/sh

autoheader
libtoolize
aclocal -I m4
automake --add-missing
autoconf

