#!/bin/sh

libtoolize
aclocal -I m4
autoconf
autoheader
automake --add-missing

