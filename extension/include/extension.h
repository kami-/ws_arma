#ifndef EXTENSION_H
#define EXTENSION_H

#define WIN32_LEAN_AND_MEAN

#include "id_generator.h"
#include "server.h"
#include "Queue/Queue.h"

#define WS_ARMA_EXTENSION_VERSION       "0.2.0"
#define MAX_DATA_LENGTH                 10000

namespace wsa {

    struct Message {
        static const uint32_t EMPTY_ID = 0;
        static const uint32_t POISON_ID = 1;
        static const uint32_t ERROR_ID = 2;
        static const uint32_t START_ID = 3;
        static const uint32_t GET_ID = 4;

        uint32_t id;
        std::string data;
    };

namespace extension {

    bool initialize();
    void finalize();
    void call(char *output, int outputSize, const char *function);
    bool isStopped();
    void stop();
    bool hasGameMessage();
    Message popGameMessage();
    size_t copyMessageToClientBuffer(const Message& message, server::Client* client);
    void processClientMessage(const server::Client* client);

} // namespace extension
} // namespace wsa

#endif // EXTENSION_H