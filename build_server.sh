#!/bin/sh

set -e

git reset --hard
python3.3 build/replace_versions.py

if ! test -e build_server_ok
then
	./switch_build.sh server
	autoreconf || true
	automake --add-missing || true
	libtoolize || true
	autoreconf --install
	./configure
	touch build_server_ok
fi

./switch_build.sh server

make
make dist