/*
 * cproxy.h
 *
 *  Created on: Feb 20, 2016
 *      Author: Eamon Dysinger
 */

#ifndef CPROXY_H_
#define CPROXY_H_

// headers
#include <sys/types.h>
#include <sys/socket.h>      // Core socket functions and data structures
#include <netinet/in.h>      // AF_INET and AF_INET6 address families and their corresponding protocol families, PF_INET and PF_INET6. These include standard IP addresses and TCP and UDP port numbers.
#include <sys/un.h>          // PF_UNIX and PF_LOCAL address family. Used for local communication between programs running on the same computer.
#include <arpa/inet.h>       // Functions for manipulating numeric IP addresses.
#include <netdb.h>           // Functions for translating protocol names and host names into numeric addresses. Searches local data as well as name services.
#include <sys/select.h>      // functions for checking sockets to see if they're send/receive ready
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


class cproxy {
};

#endif /* CPROXY_H_ */
