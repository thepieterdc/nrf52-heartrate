//
// Created by mathieu on 17/12/19.
//
#include "IOTBTClient.h"
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <cerrno>

IOTBTClient::IOTBTClient() {
    struct sockaddr_rc addr = { 0 };
    int status;
    char dest[18] = BT_HRS_ADDRESS;

    // allocate a socket
    socketNumber = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) 1;
    str2ba( dest, &addr.rc_bdaddr );

    // connect to server
    status = connect(socketNumber, (struct sockaddr *)&addr, sizeof(addr));

    if( status < 0 ){
        reportError();
    }

}

void IOTBTClient::reportError(){
    perror("Shit has gone wrong");
}

IOTBTClient::~IOTBTClient() {
    close(socketNumber);
}

bool IOTBTClient::sendMessage(void *buffer, int length) {
    int status;
    status = write(socketNumber, buffer, length);

    if (status < 0){
        reportError();
        return false;
    }
    return true;
}

bool IOTBTClient::readMessage(char *buffer, int desiredLength) {
    ssize_t bytes_read = 0;
    //testing stuff, BUFSIZE = 4096
    printf("Received Socket # %d\nBuffer Size = %d\n", socketNumber, desiredLength);
    bytes_read = recv(socketNumber, buffer, desiredLength - 1, 0);
    while (bytes_read > 0)
    {
        buffer[bytes_read] = 0; // Null-terminate the buffer
        printf("Buffer: %s\n", buffer);
        bytes_read = recv(socketNumber, buffer, desiredLength - 1, 0);
    }
    if (bytes_read == -1)
    {
        // Requires inclusion of <errno.h>
        reportError();
        return false;
    }
    return true;
}



