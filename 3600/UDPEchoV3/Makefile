include Make.defines

PROGS =	 client server  

OPTIONS = -DUNIX  -DANSI


COBJECTS =	AddressUtility.o DieWithError.o DieWithMessage.o  utils.o session.o
CSOURCES =	AddressUtility.c DieWithError.c DieWithMessage.c utils.c session.c

CPLUSOBJECTS = 

COMMONSOURCES =

CPLUSSOURCES =

all:	${PROGS}


client:		client.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(COMMONSOURCES) $(SOURCES)
		${CC} ${LINKOPTIONS}  $@ client.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)

server:		server.o $(CPLUSOBJECTS) $(COBJECTS)
		${CC} ${LINKOPTIONS} $@ server.o $(CPLUSOBJECTS) $(COBJECTS) $(LIBS) $(LINKFLAGS)


.cc.o:	$(HEADERS)
	$(CPLUS) $(CPLUSFLAGS) $(OPTIONS) $<

.c.o:	$(HEADERS)
	$(CC) $(CFLAGS) $(OPTIONS) $<



backup:
	rm -f UDPEchoV3.tar
	rm -f UDPEchoV3.tar.gz
	tar -cf UDPEchoV3.tar *
	gzip -f UDPEchoV3.tar

clean:
		rm -f ${PROGS} ${CLEANFILES}

depend:
		makedepend client.c server.c $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)
#		mkdep $(CFLAGS) $(HEADERS) $(SOURCES) $(COMMONSOURCES) $(CSOURCES)

# DO NOT DELETE

