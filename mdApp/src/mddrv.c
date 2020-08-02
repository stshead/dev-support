#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>

#include <dbDefs.h>
#include <registryFunction.h>
#include <iocsh.h>
#include <epicsThread.h>
#include <epicsExport.h>
#include <subRecord.h>
#include <aiRecord.h>
#include <dbScan.h>
#include <link.h>
#include <devSup.h>

// Thread list
epicsThreadId tid[10];

// Parameters of type double
double D_Parameters[3];
enum Double_Parameters{
  D_null,
  D_param1,
  D_param2
};
//D_Parameters[D_null]=0; // data resource for typos in field INP

// Sub function
static long readval(subRecord *precord)
{
  precord->val = D_Parameters[D_param1];
  return 0;
}
epicsRegisterFunction(readval);


/* Thread with random data creation */
static void procserver(void *ctx){
  double num;
  srand(time(NULL));
  printf("Random number generator started.\n");
  while(1){
    num = (double)(rand()%1000);
    D_Parameters[D_param1]=num;
    D_Parameters[D_param2]=num/10.0;
    postEvent(eventNameToHandle("trig1"));
    sleep(1);
  }
}

/* IOC Shell function + argument handling */
static const int mytest(const char *str1, int val1){
  printf("STR: %s\n", str1);
  printf("Val: %d\n", val1);
  // Init thread 0 - Process server
  tid[0]=epicsThreadCreate("Process_Server", epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium), procserver, 0);
  if (!tid[0]){
    printf("epicsThreadCreate [0] failed\n");
  }else{
    printf("Process_Server: Running...\n");
  }
  /* create handlers for events  */
  eventNameToHandle("trig1");

   /* reset data array */
   D_Parameters[D_null]=0; // data resource for typos in field INP
  return 0;
}

/* IOCSH function with arguments */
static const iocshArg mytestArg0 = {"Test String 1", iocshArgString};
static const iocshArg mytestArg1 = {"Test Val 1", iocshArgInt};
static const iocshArg * const mytestArgs[] = {&mytestArg0, &mytestArg1};
static const iocshFuncDef mytestDef = {"mytest", 2, mytestArgs};
static void mytestCallFunc(const iocshArgBuf *args){
  mytest(args[0].sval, args[1].ival);
}
static void mdRegister(void)
{
  iocshRegister(&mytestDef, mytestCallFunc);
}
epicsExportRegistrar(mdRegister);


/* common dset for device support */
typedef struct {
	long		   number;
	DEVSUPFUN	 dev_report;
	DEVSUPFUN	 init;
	DEVSUPFUN	 init_record;
	DEVSUPFUN	 get_ioint_info;
	DEVSUPFUN	 process;
  DEVSUPFUN  special_linconv;
} commonDSET;

/* init_record function for DTYP Mydrv for ai */
long initrecord(aiRecord *precord){
  char *inp = precord->inp.value.instio.string;
  int index;
       if (strcmp(inp, "D_param1")==0) index=D_param1;
  else if (strcmp(inp, "D_param2")==0) index=D_param2;
  else index=D_null;
  precord->dpvt = (void*)&D_Parameters[index];
  return 0;
}

/* process_record function for DTYP Mydrv for ai  */
long process_record(aiRecord *precord){
  precord->val = *( (double*)precord->dpvt );
  //Set undefined to False
  precord->udf = 0;
  // return 0 to activate conversion of rval or
  // return 2 to ignore convertion of rval into val
  return 2;
}

commonDSET Mydrv = {6, NULL, NULL, initrecord, NULL, process_record, NULL};
epicsExportAddress(dset, Mydrv);
