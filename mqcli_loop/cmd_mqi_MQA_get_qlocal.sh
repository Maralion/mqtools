#!/bin/bash

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14001)"
./mqcli_get_c '*QM' QLO.A

