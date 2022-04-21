#!/bin/bash

cat <<EOF
---------------------------------------------------------------------------------
- QLO test MQI
---------------------------------------------------------------------------------

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14001)"
./mqcli_get_c '*QM' QLO.A

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14001)"
./mqcli_put_c '*QM' QLO.A

---------------------------------------------------------------------------------
- QRE test MQI
---------------------------------------------------------------------------------

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14001)"
./mqcli_get_c QMA QLO.A

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14001)"
./mqcli_put_c QMA QRE.B

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14002)"
./mqcli_get_c QMB QLO.B

export LD_LIBRARY_PATH=/opt/mqm/lib64
export MQSERVER="CHL.SVR/TCP/127.0.0.1(14002)"
./mqcli_put_c QMB QRE.A

EOF
