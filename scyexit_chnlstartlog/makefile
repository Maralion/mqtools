MQHOME=/opt/mqm

scyexit_chnlstartlog: scyexit_chnlstartlog.c
	gcc -I $(MQHOME)/inc -shared -m64 -fPIC -o scyexit_chnlstartlog scyexit_chnlstartlog.c
	chmod 755 scyexit_chnlstartlog

install:
	cp -vf scyexit_chnlstartlog /var/mqm/exits64/Installation1/scyexit_chnlstartlog

remove:
	rm -vf /var/mqm/exits64/Installation1/scyexit_chnlstartlog

clean:
	rm -f *.o scyexit_chnlstartlog
