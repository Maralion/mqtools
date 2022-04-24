MQHOME=/opt/mqm

scyexit_chnlstartq: scyexit_chnlstartq.c
	gcc -I $(MQHOME)/inc -shared -m64 -fPIC -o scyexit_chnlstartq scyexit_chnlstartq.c
	chmod 755 scyexit_chnlstartq

install:
	cp -vf scyexit_chnlstartq /var/mqm/exits64/Installation1/scyexit_chnlstartq

remove:
	rm -vf /var/mqm/exits64/Installation1/scyexit_chnlstartq

clean:
	rm -f *.o scyexit_chnlstartq
