#!/bin/sh

set -e

WORKDIR=$(mktemp -d)
trap "rm -rf $WORKDIR" 0 INT QUIT ABRT PIPE TERM
cd $WORKDIR
cat <<EOF > librsctest.c
#include "rsc/runtime/Properties.h"

int main(int, char*[]) {
    rsc::runtime::Properties props;
    return 0;
}
EOF

g++ -o librsctest librsctest.c `pkg-config --cflags --libs rsc0.9`
echo "build: OK"
[ -x librsctest ]
./librsctest
echo "run: OK"
