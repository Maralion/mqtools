MQHOME=/opt/mqm

all: mqcli_get mqcli_get_c mqcli_put mqcli_put_c

mqcli_get: mqcli_get.c
	gcc -I ${MQHOME}/inc -L ${MQHOME}/lib64 -lmqm -o mqcli_get mqcli_get.c
	chmod 755 mqcli_get

mqcli_get_c: mqcli_get.c
	gcc -I ${MQHOME}/inc -L ${MQHOME}/lib64 -lmqic -o mqcli_get_c mqcli_get.c
	chmod 755 mqcli_get_c

mqcli_put: mqcli_put.c
	gcc -I ${MQHOME}/inc -L ${MQHOME}/lib64 -lmqm -o mqcli_put mqcli_put.c
	chmod 755 mqcli_put

mqcli_put_c: mqcli_put.c
	gcc -I ${MQHOME}/inc -L ${MQHOME}/lib64 -lmqic -o mqcli_put_c mqcli_put.c
	chmod 755 mqcli_put_c

clean:
	rm -f *.o mqcli_get mqcli_get_c mqcli_put mqcli_put_c
