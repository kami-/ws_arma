#ifndef SERVER_H
#define SERVER_H

#include "libwebsockets.h"

#define MAX_DATA_LENGTH      10000

namespace wsa {

    class Extension;

namespace server {

    struct Client {
        unsigned char dataBuffer[LWS_PRE + MAX_DATA_LENGTH];
        size_t dataLength;
    };

    void run(int port);

} // namespace server
} // namespace wsa

#endif // SERVER_H
