#!/bin/sh
# Run this to generate all the initial makefiles, etc.

srcdir=`dirname $0`
test -z "$srcdir" && srcdir=.

DIE=0

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have autoconf installed to compile esound."
	echo "Download the appropriate package for your distribution,"
	echo "or get the source tarball at http://ftp.gnu.org/pub/gnu/automake/"
	DIE=1
}

(libtool --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have libtool installed to compile esound."
	echo "Get it from http://ftp.gnu.org/pub/gnu/libtool/"
	DIE=1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "You must have automake installed to compile esound."
	echo "Get http://ftp.gnu.org/pub/gnu/automake/"
	echo "(or a newer version if it is available)"
	DIE=1
}

if test "$DIE" -eq 1; then
	exit 1
fi

THEDIR="`pwd`"
cd $srcdir
aclocal $ACLOCAL_FLAGS || exit 1
libtoolize --force || exit 1
autoheader || exit 1
autoconf || exit 1
automake --gnu --add-missing || exit 1
cd "$THEDIR"

if test -z "$*"; then
	echo "I am going to run ./configure with no arguments - if you wish "
        echo "to pass any to it, please specify them on the $0 command line."
fi

$srcdir/configure --enable-maintainer-mode "$@"

echo 
echo "Now type 'make' to compile esound."
