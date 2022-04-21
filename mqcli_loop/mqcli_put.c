#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <cmqc.h>

// export MQSERVER='CHL.SVR/TCP/localhost(14001),localhost(14002)'

static void pgm_msg(char *msg, ...);
static void pgm_err(char *msg, ...);

int main(int argc, char **argv)
{
	char qmgr[MQ_Q_MGR_NAME_LENGTH];

	MQHCONN hconn;                      // Connection handle
	MQHOBJ  hobj;                       // Object handle

	MQOD    od = { MQOD_DEFAULT };      // Object descriptor
	MQMD    md = { MQMD_DEFAULT };      // Message descriptor
	MQPMO   popt = { MQPMO_DEFAULT };   // PUT message options

	MQOD    rod = { MQOD_DEFAULT };     // Ref object descriptor
	MQMD    rmd = { MQMD_DEFAULT };     // Ref message descriptor
	MQPMO   rpopt = { MQPMO_DEFAULT };  // Ref PUT message options

	MQLONG  cc;                         // Completion code
	MQLONG  re;                         // Reason code
	MQLONG  pcc;

	time_t vtime;
	struct tm *stime;
	char btime[64];

	char msgbuf[65536];
	int msglen;
	int cpt=0;

	pgm_msg("Version "__DATE__" "__TIME__"\n");
	pgm_msg("QMGR=%s\n",argv[1]);
	pgm_msg("QNAME=%s\n",argv[2]);

	memset(qmgr, 0, sizeof(qmgr));
	strncpy(qmgr, argv[1], MQ_Q_MGR_NAME_LENGTH);

	pcc=0;

	for (;;)
	{
		sleep(1);

		MQCONN(qmgr, &hconn, &cc, &re);
		if (cc == MQCC_FAILED)
		{
			if (pcc!=cc)
				pgm_err("MQCONN() cc=%d re=%d\n", cc, re);
			pcc=cc;
			continue;
		}
		pcc=0;

		memcpy(&od,&rod,sizeof(od));
		strncpy(od.ObjectName, argv[2], MQ_Q_NAME_LENGTH);

		MQOPEN(hconn, &od, MQOO_OUTPUT | MQOO_FAIL_IF_QUIESCING, &hobj, &cc, &re);
		if (cc == MQCC_FAILED && re != MQRC_NONE)
		{
			pgm_err("MQOPEN() cc=%d re=%d\n", cc, re);
			MQDISC(&hconn, &cc, &re);
			continue;
		}

		memcpy(&popt,&rpopt,sizeof(popt));
		popt.Options = MQPMO_SYNCPOINT | MQPMO_FAIL_IF_QUIESCING;

		for(;;)
		{
			time(&vtime);
			stime=localtime(&vtime);
			strftime(btime,sizeof(btime)-1,"%Y%m%d.%H%M%S",stime);

			if (argc<4)
				msglen=snprintf(msgbuf,sizeof(msgbuf),"%s %d",btime,cpt);
			else
				msglen=snprintf(msgbuf,sizeof(msgbuf),"%s %d %s",btime,cpt,argv[3]);

			memcpy(&md,&rmd,sizeof(md));

			MQPUT(hconn, hobj, &md, &popt, msglen, msgbuf, &cc, &re);
			if (re != MQRC_NONE)
			{
				pgm_err("MQPUT() cc=%d re=%d\n", cc, re);
				break;
			}

			MQCMIT(hconn, &cc, &re);
			if (re != MQRC_NONE)
			{
				pgm_err("MQCMIT() cc=%d re=%d\n", cc, re);
				break;
			}

			pgm_msg("MQPUT() %s %s\n",argv[1],msgbuf);

			cpt++;

			sleep(1);
		}

		MQCLOSE(hconn, &hobj, MQCO_NONE, &cc, &re);
		if (re != MQRC_NONE)
			pgm_err("MQCLOSE() cc=%d re=%d\n", cc, re);

		MQDISC(&hconn, &cc, &re);
		if (re != MQRC_NONE)
			pgm_err("MQDISC() cc=%d re=%d\n", cc, re);
	}

	return 0;
}

static void pgm_msg(char *msg, ...)
{
	time_t vtime;
	struct tm *stime;
	char btime[64];

    va_list args;

	time(&vtime);
	stime=localtime(&vtime);
	strftime(btime,sizeof(btime)-1,"%Y-%m-%d %H:%M:%S",stime);

	printf("%s - ",btime);

    va_start(args,msg);
	vprintf(msg,args);
	va_end(args);
}

static void pgm_err(char *msg, ...)
{
	time_t vtime;
	struct tm *stime;
	char btime[64];

    va_list args;

	time(&vtime);
	stime=localtime(&vtime);
	strftime(btime,sizeof(btime)-1,"%Y-%m-%d %H:%M:%S",stime);

	fprintf(stderr,"%s - ",btime);

    va_start(args,msg);
	vfprintf(stderr,msg,args);
	va_end(args);
}