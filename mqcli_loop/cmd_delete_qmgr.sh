#!/bin/bash

if [ "$USER" != "mqm" ]
then
   echo "Run script as mqm whih command: sudo su - mqm"
   exit 4
fi

# Set path

export PATH=${PATH}:/opt/mqm/bin

# End and delete old QMGRs

endmqm -w QMA
dltmqm QMA

endmqm -w QMB
dltmqm QMB

exit 0

