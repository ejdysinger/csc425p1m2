/*
 * cproxy.c
 *
 *  Created on: Feb 20, 2016
 *      Author: Eamon Dysinger
 */

#include "cproxy.h"

/*****************************************************************************************************
 METHOD:      main
 DESCRIPTION: listens to sockets belonging to the sProxy and telnet and relays messages
              between them
 PARAMS:      the port number and IP to listen to for the connection to the sProxy
 RETURNS:     -1 if failure, 0 otherwise
******************************************************************************************************/
int main(int argc, char * argv[]){
	// variables
	char tnBuffer[1024], sProxyBuffer[1024]; // buffer for messages
	int telnetSocket, sProxySocket, tnTemp;  // sockets for the client and the telnet daemon, respectively
	int opSuccess, selectParam;              // catches return values from server processes
	struct sockaddr_in tN, sP, temp;         // sockaddr_in structs for the sockets, filled w/ command line inputs
	fd_set toRead;                           // fd_set to keep track of all of the sockets that are ready to read

	// set up the sockaddr_in structs
	tN.sin_family = AF_INET;
	tN.sin_addr.s_addr = INADDR_ANY;
	tN.sin_port = htons(atoi(argv[1]));
	bzero(&tN.sin_zero, 8);

	// pupulate the sP struct for the sProxy
	sP.sin_family = AF_INET;
	sP.sin_addr.s_addr = inet_addr(argv[2]);
	sP.sin_port = htons(atoi(argv[3]));
	bzero(&sP.sin_zero,8);

	// create the sockets
	if(((telnetSocket = socket(AF_INET,SOCK_STREAM,0)) == -1) || (sProxySocket = socket(AF_INET,SOCK_STREAM,0))  == -1)){
		fprintf(stderr,"ERROR 1: CLIENTPROXY failed to create telnet socket\n");
		exit(-1);
	}

	// bind to the telnet socket and await connections
	if((opSuccess = bind(telnetSocket, (struct sockaddr *)&tN, sizeof(struct sockaddr_in))) == -1){
		fprintf(stderr,"ERROR 4: CLIENTPROXY failed to bind telnet socket\n");
		exit(-1);
	}

	// listen to the bound socket for connections and accept
	if((opSuccess = listen(telnetSocket, 0)) == -1){
		fprintf(stderr,"ERROR 5: CLIENTPROXY failed listen on cProxy socket\n");
		exit(-1);
	}


	// wait for inputs from either socket and pass them between the two sockets
	while(1){

		if((tnTemp  = accept(telnetSocket,(struct sockaddr *) &temp, NULL)) == -1){
			fprintf(stderr,"ERROR 5: CLIENTPROXY failed accept\n");
			break;
		}

		// connect to the serverProxy
		if((opSuccess= connect(sProxySocket, (struct sockaddr *)&sP,sizeof(struct sockaddr_in))) == -1){
			fprintf(stderr,"ERROR 3: CLIENTPROXY failed to connect to telnet Daemon\n");
			exit(-1);
		}

		FD_SET(tnTemp, &toRead);
		FD_SET(sProxySocket, &toRead);

		if((opSuccess= connect(sProxySocket, (struct sockaddr *)&sP,sizeof(struct sockaddr_in))) == -1){
			fprintf(stderr,"ERROR 3: CLIENTPROXY failed to connect to sProxy\n");
			exit(-1);
		}
		// finds out which is the highest number of socket
			selectParam = tnTemp +1;
			if(selectParam <= sProxySocket + 1)
				selectParam = sProxySocket  + 1;

		// pass messages back and forth as they arrive
		while((opSuccess = select(selectParam, &toRead, NULL, NULL, NULL)) >0){
			if(opSuccess == -1){
				fprintf(stderr,"ERROR 7: SERVERPROXY failed select\n");
				exit(-1);
			}
			// clear the buffers
			memset(tnBuffer,0,sizeof(tnBuffer));
			memset(sProxyBuffer, 0, sizeof(sProxyBuffer));
			// check if incoming messages are present
			if (FD_ISSET(tnTemp, &toRead)) {
				recv(tnTemp, tnBuffer, sizeof(tnBuffer), 0);
			}
			if (FD_ISSET(sProxySocket, &toRead)) {
				recv(sProxySocket, sProxyBuffer, sizeof(sProxyBuffer), 0);
			}
			// if messages have been received, send them
			if((opSuccess = strlen(tnBuffer)) != 0)
				send(sProxySocket,tnBuffer,sizeof(tnBuffer),0);
			if((opSuccess = strlen(sProxyBuffer)) != 0)
				send(tnTemp,sProxyBuffer,sizeof(sProxyBuffer),0);
		}
		// close the connections and flush the select structs
		close(tnTemp);
		FD_ZERO(&toRead);

	}

	// close the two sockets

	return 0;
}

/*****************************************************************************************************
 METHOD:      socketPrep
 DESCRIPTION: populates the socket structs for the cProxy and telnet daemon sockets
 PARAMS:      pointers to the two structs, the telnet port number, the sProxy port number and IP addy
 RETURNS:     nothing
******************************************************************************************************
void socketPrep(struct sockaddr_in *tN, struct sockaddr_in *sP, char *telnetSocketNum, char *sProxyIP, char *sProxyPort){
	// populate the tN struct for the telnet
	tN->sin_family = AF_INET;
	tN.sin_addr.s_addr = INADDR_ANY;
	tN.sin_port = htons(atoi(telnetSocketNum));
	bzero(&tN.sin_zero, 8);

	// pupulate the sP struct for the sProxy
	sP.sin_family = AF_INET;
	sP.sin_addr.s_addr = inet_addr(sProxyIP);
	sP.sin_port = htons(atoi(sProxyPort));
	bzero(&sP.sin_zero,8);
}*/
