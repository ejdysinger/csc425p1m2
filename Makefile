ASSIGNMENT = csc425project1phase2
CC = gcc
AR = ar
COBJS = cproxy.o sproxy.o
CSRCS = ${COBJS:.o=.c}
TARGET = ${COBJS:.o=}
README = README.txt

HDRS = *.h

CFLAGS = -Wall -g -std=gnu99 -I.

UNAME := $(shell uname -s)

all: $(COBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -c $(CSRCS)
	$(CC) $(CFLAGS) $(INCLUDE) cproxy.o -o cproxy
	$(CC) $(CFLAGS) $(INCLUDE) sproxy.o -o sproxy
	
clean:
	rm -f $(COBJS) $(TARGET)
              
submit: $(CSRCS) $(HDRS)
	tar cvzf $(ASSIGNMENT).tgz $(CSRCS) $(HDRS) Makefile $(README)
