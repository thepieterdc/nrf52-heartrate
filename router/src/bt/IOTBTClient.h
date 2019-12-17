//
// Created by mathieu on 17/12/19.
//

#ifndef ROUTER_IOTBTCLIENT_H
#define ROUTER_IOTBTCLIENT_H

#include <bluetooth/bluetooth.h>


class IOTBTClient {
public:
    IOTBTClient();

    virtual ~IOTBTClient();

    bool sendMessage(void * buffer, int length);

    bool readMessage(char * buffer, int desiredLength);

protected:

    void reportError();

private:

    int socketNumber;

};


#endif //ROUTER_IOTBTCLIENT_H
