#include "server.h"
#include "log.h"
#include "Extension.h"

namespace wsa {
namespace server {

namespace {
    Extension* extension;
    struct lws* clientSocket;
}

    int clientCallback(struct lws* wsi, enum lws_callback_reasons reason, void* user, void* in, size_t len) {
        struct Client* client = (struct Client*) user;
        if (reason == LWS_CALLBACK_ESTABLISHED) { // Initialize session
            if (clientSocket) {
                log::logger->error("There is a client already connected! Multiple clients are unsupported!");
                return 1;
                
            }
            clientSocket = wsi;
            return 0;
        }
        else if (reason == LWS_CALLBACK_SERVER_WRITEABLE) {
            auto message = extension::popGameMessage();
            if (message.id == Message::POISON_ID) {
                extension::stop();
                return 0;
            }
            if (extension::copyMessageToClientBuffer(message, client) <= MAX_DATA_LENGTH) {
                int writeResult = lws_write(wsi, &client->dataBuffer[LWS_PRE], client->dataLength, (lws_write_protocol)LWS_WRITE_TEXT);
                if (writeResult < 0) {
                    log::logger->error("There was an error writing message '{}' to client socket!", message.id);
                    return 0;
                }
                if (writeResult < (int)client->dataLength) {
                    log::logger->error("Partial write of message '{}' to client socket!", message.id);
                    return 0;
                }
            }
            else {
                log::logger->error("Data too long! Supported data length is '{}'. Dropping message '{}'!", MAX_DATA_LENGTH, message.id);
                return 0;
            }
            client->dataLength = -1;
        }
        else if (reason == LWS_CALLBACK_RECEIVE) {
            if (!lws_is_final_fragment(wsi)) {
                log::logger->error("Fragemted messages are unsupported! Dropping message fragment!");
                return 0;
            }
            if (lws_frame_is_binary(wsi)) {
                log::logger->error("Binary messages are unsupported! Dropping message!");
                return 0;
            }
            if (len > MAX_DATA_LENGTH) {
                log::logger->error("Data too long! Supported data length is '{}'. Dropping data!", MAX_DATA_LENGTH);
                return 0;
            }
            memcpy(&client->dataBuffer[LWS_PRE], in, len);
            client->dataLength = len;
            extension::processClientMessage(client);
        }
        else if (reason == LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED) {
            if (strcmp((char*)in, "permessage-deflate")) {
                return 1;
            }
        }
        else if (reason == LWS_CALLBACK_CLOSED) {
            clientSocket = nullptr;
        }
        return 0;
    }

    void run(int port) {
        const struct lws_protocols protocols[2] = {
            { "ws_arma", clientCallback, sizeof(struct Client), MAX_DATA_LENGTH },
            { nullptr, nullptr, 0 }
        };

        const struct lws_extension lws_extensions[3] = {
            { "permessage-deflate", lws_extension_callback_pm_deflate, "permessage-deflate; client_no_context_takeover; client_max_window_bits" },
            { "deflate-frame", lws_extension_callback_pm_deflate, "deflate_frame" },
            { nullptr, nullptr, nullptr }
        };

        struct lws_context_creation_info info;
        memset(&info, 0, sizeof info);
        info.port = port;
        info.gid = -1;
        info.uid = -1;
        info.options = LWS_SERVER_OPTION_VALIDATE_UTF8;
        info.protocols = protocols;
        info.extensions = lws_extensions;
        lws_set_log_level(1, lwsl_emit_syslog);

        log::logger->info("Starting websocket server on port '{}'.", port);
        struct lws_context* context = lws_create_context(&info);
        if (!context) {
            log::logger->error("Failed to create websocket server context!");
            extension::stop();
            return;
        }

        int n = 0;
        while (n >= 0 && !extension::isStopped()) {
            n = lws_service(context, 10);
            if (clientSocket && !extension::hasGameMessage()) {
                lws_callback_on_writable(clientSocket);
            }
        }
        lws_context_destroy(context);
        log::logger->info("Stopped websocket server.", port);
    }
} // namespace server
} // namespace wsa
