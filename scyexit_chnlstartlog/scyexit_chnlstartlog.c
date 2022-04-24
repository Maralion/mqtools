#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <syslog.h>

#include <cmqc.h>
#include <cmqxc.h>

// Fonctions locales

static int slen(int len, char *ptr)
{
    int cpt=0;

    while (len>0)
    {
        if (*ptr==' ' || *ptr=='\0')
            break;

        len--;
        ptr++;
        cpt++;
    }

    return cpt;
}

// Dummy entry point

void MQStart() {;}

// Main exit code

void MQENTRY scyexit(PMQVOID pChannelExitParms,
                     PMQVOID pChannelDefinition,
                     PMQLONG pDataLength,
                     PMQLONG pAgentBufferLength,
                     PMQVOID pAgentBuffer,
                     PMQLONG pExitBufferLength,
                     PMQPTR  pExitBufferAddr)
{
    PMQCXP pParms = (PMQCXP)pChannelExitParms;
    PMQCD  pCD    = (PMQCD)pChannelDefinition;

    MQOD  obj_desc = { MQOD_DEFAULT  };
    MQMD  msg_desc = { MQMD_DEFAULT  };
    MQPMO put_opt  = { MQPMO_DEFAULT };

    MQLONG cc,re;

    struct timeval tm_cur;
    struct tm tm_det;

    char buf[512];
    int len;

    // Default return code

    pParms->ExitResponse=MQXCC_OK;

    // Exits if version MQCXP <7 & MQCD <7

    if (pParms->Version<MQCXP_VERSION_7)
        return;
    if (pCD->Version<MQCD_VERSION_7)
        return;

    // Exits if not an CHANNEL_SEC_EXIT

    if (pParms->ExitId!=MQXT_CHANNEL_SEC_EXIT || pParms->ExitReason!=MQXR_SEC_PARMS)
        return;

    // Get current time

    gettimeofday(&tm_cur,NULL);
    localtime_r(&(tm_cur.tv_sec),&tm_det);

    // Message log

    len=snprintf(buf,sizeof(buf),"STARTCHNL DATE=%04d-%02d-%02d TIME=%02d:%02d:%02d.%03d QMGR=%.*s CHNL=%.*s CONN=%.*s USER=%.*s SSLPEER=\"%.*s\"",
                 tm_det.tm_year+1900,tm_det.tm_mon+1,tm_det.tm_mday,tm_det.tm_hour,tm_det.tm_min,tm_det.tm_sec,tm_cur.tv_usec/1000,
                 slen(sizeof(pCD->QMgrName),pCD->QMgrName),pCD->QMgrName,
                 slen(sizeof(pCD->ChannelName),pCD->ChannelName),pCD->ChannelName,
                 slen(sizeof(pCD->ConnectionName),pCD->ConnectionName),pCD->ConnectionName,
                 slen(sizeof(pCD->RemoteUserIdentifier),pCD->RemoteUserIdentifier),pCD->RemoteUserIdentifier,
                 pCD->SSLPeerNameLength,pCD->SSLPeerNamePtr);

    // Put buffer to syslog

    openlog("chnlstartlog",LOG_NDELAY,LOG_USER);
    syslog(LOG_INFO,"%.*s\n",len,buf);
    closelog();
}
