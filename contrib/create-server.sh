#!/bin/bash
#
# $ sh create-server.sh 1
# $ sh create-server.sh 2
# $ sh create-server.sh 3

NODE_NUMBER=$1
IDENTITY="$((10000000+${1}))-0000-0000-0000-000000000000"

mkdir -p node-${NODE_NUMBER}
pushd node-${NODE_NUMBER}

echo "port = $(((${NODE_NUMBER-1})*4+8000))"
echo "port = $(((${NODE_NUMBER-1})*4+9000))"
echo "port = $(((${NODE_NUMBER-1})*4+10000))"

echo "[node]" > config.ini
echo "fault-domain = fault-domain-$(((${NODE_NUMBER-1})))" >> config.ini
echo "[partition-minus]" >> config.ini
echo "port = $(((${NODE_NUMBER-1})*4+8000))" >> config.ini
echo "ping-interval-in-seconds = 3" >> config.ini
echo "[partition-zero]" >> config.ini
echo "port = $(((${NODE_NUMBER-1})*4+9000))" >> config.ini
echo "ping-interval-in-seconds = 3" >> config.ini
echo "[partition-plus]" >> config.ini
echo "port = $(((${NODE_NUMBER-1})*4+10000))" >> config.ini
echo "ping-interval-in-seconds = 3" >> config.ini
ulimit -c unlimited

# Run
dafs-server --identity=${IDENTITY} --port=$((${NODE_NUMBER}+7000))
popd
