include Make.defines

PROGS =	 TCPTx TCPRx

OPTIONS = -DUNIX  -DUSE_SIGACTION -DANSI


COBJECTS =	AddressHelper.o SocketHelper.o utils.o 
CSOURCES =	AddressHelper.c SocketHelper.c  utils.c 

CPLUSOBJECTS = 

COMMONSOURCES =

CPLUSSOURCES =

all:	${PROGS}



TCPTx:	TCPSender.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(COMMONSOURCES) $(SOURCES)
		${CC} $(INCLUDES) ${LINKOPTIONS}  $@ TCPSender.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)

TCPRx:	TCPReceiver.o $(CPLUSOBJECTS) $(COBJECTS)
		${CC} $(INCLUDES) ${LINKOPTIONS} $@ TCPReceiver.o  $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)

.cc.o:	$(HEADERS)
	$(CPLUS) $(CPLUSFLAGS) $(OPTIONS) $<

.c.o:	$(HEADERS)
	$(CC) $(CFLAGS) $(OPTIONS) $<



backup:
	rm -f TCPTxRx.tar.gz
	rm -f TCPTxRx.tar
	tar -cf TCPTxRx.tar *
	gzip -f TCPTxRx.tar

clean:
		rm -f ${PROGS} ${CLEANFILES}

depend:
		makedepend TCPReceiver.c TCPSender.c $(INCLUDES) $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)
#		mkdep $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)

# DO NOT DELETE

