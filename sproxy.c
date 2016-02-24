/*
 * sproxy.c
 *
 *  Created on: Feb 20, 2016
 *      Author: Eamon Dysinger
 */

#include "sproxy.h"

/*****************************************************************************************************
 METHOD:      main
 DESCRIPTION: listens to sockets belonging to the cProxy and the telnet daemon and relays messages
              between them
 PARAMS:      the port number to listen to for the cProxy connection
 RETURNS:     -1 if failure, 0 otherwise
******************************************************************************************************/

int main(int argc, char * argv[]){
	// variables
	char *cBuffer[1024], daemonBuffer[1024]; // a buffer for storing messages sent to the sProxy
	int cProxySocket, daemonSocket, cTemp;   // sockets for the client and the telnet daemon, respectively
	int opSuccess, selectParam;              // catches return values from server processes; highest number socket
	struct sockaddr_in cP, tD, temp;         // sockaddr_in structs for the sockets, filled w/ command line inputs
	fd_set toRead;                           // fd_set to keep track of all of the sockets that are ready to read

	// populate the tN struct for the telnet
	cP.sin_family = AF_INET;
	cP.sin_addr.s_addr = INADDR_ANY;
	cP.sin_port = htons(23);
	bzero(&cP.sin_zero, 8);

	// pupulate the sP struct for the sProxy
	tD.sin_family = AF_INET;
	tD.sin_addr.s_addr = inet_addr("127.0.0.1");
	tD.sin_port = htons(atoi(argv[1]));
	bzero(&tD.sin_zero,8);

	// create the sockets
	if(((cProxySocket = socket(AF_INET,SOCK_STREAM,0))  == -1) || ((daemonSocket = socket(AF_INET,SOCK_STREAM,0))  == -1)){
		fprintf(stderr,"ERROR 1: SERVERPROXY failed to create socket\n");
		exit(-1);
	}

	// set up the sockaddr_in structs
	if((opSuccess = socketPrep(&cP, &tD, argv[1])) == -1){
		fprintf(stderr,"ERROR 2: SERVERPROXY failed to populate structs\n");
		exit(-1);
	}

	// bind to the cProxy socket and await connections
	if((opSuccess = bind(cProxySocket, (struct sockaddr *)&cP, sizeof(struct sockaddr_in))) == -1){
		fprintf(stderr,"ERROR 3: SERVERPROXY failed to bind cProxy socket\n");
		exit(-1);
	}

	// listen to the bound cProxy socket for connections
	if((opSuccess = listen(cProxySocket, 0)) == -1){
		fprintf(stderr,"ERROR 4: SERVERPROXY failed listen on cProxy socket\n");
		exit(-1);
	}

	// send the

	// wait for inputs from either socket and pass them between the two sockets
	while(1){
		if((cTemp  = accept(cProxySocket,(struct sockaddr *) &temp, NULL)) == -1){
			fprintf(stderr,"ERROR 5: SERVERPROXY failed accept\n");
			break;
		}
		// connect to the telnet daemon
		if((opSuccess = connect(daemonSocket, (struct sockaddr *)&tD,sizeof(struct sockaddr_in))) == -1){
			fprintf(stderr,"ERROR 6: SERVERPROXY failed to connect to telnet Daemon\n");
			exit(-1);
		}

		// add both telnet daemon and cproxy to select set
		FD_SET(cTemp, &toRead);
		FD_SET(daemonSocket, &toRead);

		// finds out which is the highest number of socket
		selectParam = cTemp +1;
		if(selectParam <= daemonSocket + 1)
			selectParam = daemonSocket + 1;

		// send login message
		sprintf(cBuffer,"I'm in.\n");
		send(cTemp,cBuffer, sizeof(cBuffer),0);

		// loop through until no new input received
		while((opSuccess = select(selectParam, &toRead, NULL, NULL, NULL)) > 0){
			if(opSuccess == -1){
				fprintf(stderr,"ERROR 7: SERVERPROXY failed select\n");
				exit(-1);
			}
			// purge the buffers to prepare for receipt of input
			memset(cBuffer, 0, sizeof(cBuffer));
			memset(daemonBuffer, 0, sizeof(daemonBuffer));
			// check if one or both of the descriptors are ready to write data
			if (FD_ISSET(cTemp, &toRead)) {
				recv(cTemp, cBuffer, sizeof(cBuffer), 0);
			}
			if (FD_ISSET(daemonSocket, &toRead)) {
				recv(daemonSocket, daemonBuffer, sizeof(daemonBuffer), 0);
			}
			// write the data back
			if((opSuccess = send(cTemp, daemonBuffer, sizeof(daemonBuffer),0)) == -1){
				break;
			}
			if((opSuccess = send(daemonSocket, cBuffer, sizeof(cBuffer),0)) == -1){
				break;
			}
		}
	}

	// close the two sockets
	close(cProxySocket);
	close(daemonSocket);
	return 0;
}

/*****************************************************************************************************
 METHOD:      socketPrep
 DESCRIPTION: populates the socket structs for the cProxy and telnet daemon sockets
 PARAMS:      pointers to the two structs, the port number to listen on
 RETURNS:     -1 if invalid information supplied, 0 on success
******************************************************************************************************
void socketPrep(struct sockaddr_in *cP, struct sockaddr_in *tD, char *cProxyPort){

}*/
