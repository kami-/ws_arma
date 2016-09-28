#include "extension.h"

#include <fstream>
#include "shlobj.h"

#include "log.h"
#include "Poco/StringTokenizer.h"
#include "Poco/JSON/Parser.h"
#include "Poco/File.h"

namespace wsa {
namespace extension {

namespace {
    const std::string EMPTY_SQF_MESSAGE_DATA = "\"\"";
    const std::string SQF_DELIMITER = ":";//"¤";
    Queue<Message> clientMessages;
    Queue<Message> gameMessages;
    std::thread webSocketThread;
    std::atomic<bool> stopped = false;
    bool serverRunning = false;
    int port = 8085;
}

    void startServer() {
        if (!serverRunning) {
            log::logger->info("Starting websocket server.");
            serverRunning = true;
            webSocketThread = std::thread(server::run, port);
        }
        else {
            log::logger->info("Websocket server already running.");
        }
    }

    void respond(char* output, const Message& message) {
        std::string data = fmt::format("[{},{}]", message.id, message.data);
        data.copy(output, data.length());
        output[data.length()] = '\0';
    }

    std::vector<std::string>& split(const std::string &s, const std::string& delim, std::vector<std::string> &elems) {
        Poco::StringTokenizer tokenizer(s, delim, Poco::StringTokenizer::TOK_TRIM);
        for (auto token : tokenizer) {
            elems.push_back(token);
        }
        return elems;
    }

    std::string getExtensionFolder() {
        wchar_t wpath[MAX_PATH];
        std::string localAppData = ".";
        if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, wpath))) {
            Poco::UnicodeConverter::toUTF8(wpath, localAppData);
            Poco::File file(fmt::format("{}\\WSAExtension", localAppData));
            file.createDirectories();
            return file.path();
        }
        return localAppData;
    }

    uint32_t parseUnsigned(const std::string& str, const uint32_t defaultVal) {
        uint32_t number = defaultVal;
        if (!Poco::NumberParser::tryParseUnsigned(str, number)) {
            return defaultVal;
        }
        return number;
    }



    bool initialize() {
        std::string logLevelStr, portStr, extensionFolder(getExtensionFolder());
        std::ifstream file(fmt::format("{}\\{}", extensionFolder, "config.txt"));
        std::getline(file, logLevelStr);
        std::getline(file, portStr);
        port = parseUnsigned(portStr, port);
        log::initialze(extensionFolder, logLevelStr);
        log::logger->info("Starting ws_arma_extension version '{}'.", WS_ARMA_EXTENSION_VERSION);
        return true;
    }

    void finalize() {
        if (stopped) {
            clientMessages.push(Message{ Message::POISON_ID, EMPTY_SQF_MESSAGE_DATA });
            webSocketThread.join();
            serverRunning = false;
        }
        log::logger->info("Stopped ws_arma_extension version '{}'.", WS_ARMA_EXTENSION_VERSION);
    }

    void call(char* output, int outputSize, const char* function) {
        uint32_t messageId = id_generator::nextId();
        if (stopped) {
            respond(output, Message{ Message::ERROR_ID, EMPTY_SQF_MESSAGE_DATA });
            return;
        }

        std::vector<std::string> params;
        split(std::string(function), SQF_DELIMITER, params);
        if (params.size() == 2) {
            uint32_t id = parseUnsigned(params[0], messageId);
            Message gameMessage{ id, params[1] };
            if (gameMessage.id == Message::START_ID) {
                startServer();
                respond(output, Message{ Message::EMPTY_ID, EMPTY_SQF_MESSAGE_DATA });
                return;
            }
            else if (gameMessage.id == Message::GET_ID) {
                if (clientMessages.empty()) {
                    respond(output, Message{ Message::EMPTY_ID, EMPTY_SQF_MESSAGE_DATA });
                    return;
                }
                auto clientMessage = clientMessages.pop();
                respond(output, clientMessage);
                return;
            }
            else if (gameMessage.id != Message::EMPTY_ID && gameMessage.id != Message::ERROR_ID) {
                gameMessages.push(gameMessage);
                respond(output, Message{ id, EMPTY_SQF_MESSAGE_DATA });
                return;
            }
        }
        else {
            respond(output, Message{ Message::ERROR_ID, fmt::format("\"Invalid number of parameters given, expected 3 but got '{}'\"", params.size()) });
            return;
        }
        respond(output, Message{ Message::ERROR_ID, EMPTY_SQF_MESSAGE_DATA });
    }

    bool isStopped() {
        return stopped;
    }

    void stop() {
        stopped = true;
    }

    bool hasGameMessage() {
        return gameMessages.empty();
    }

    Message popGameMessage() {
        return gameMessages.pop();
    }

    size_t copyMessageToClientBuffer(const Message& message, server::Client* client) {
        Poco::JSON::Object json;
        json.set("id", message.id);
        json.set("data", message.data);
        std::ostringstream jsonOss;
        json.stringify(jsonOss);
        auto jsonStr = jsonOss.str();
        if (jsonStr.length() <= MAX_DATA_LENGTH) {
            client->dataLength = jsonStr.length();
            std::copy(jsonStr.begin(), jsonStr.end(), &client->dataBuffer[LWS_PRE]);
        }
        return jsonStr.length();
    }

    void processClientMessage(const server::Client* client) {
        const std::string jsonStr(&client->dataBuffer[LWS_PRE], &client->dataBuffer[LWS_PRE] + client->dataLength);
        Poco::JSON::Parser parser;
        auto json = parser.parse(jsonStr).extract<Poco::JSON::Object::Ptr>();
        try {
            const uint32_t id = json->has("id") ? json->getValue<uint32_t>("id") : Message::ERROR_ID;
            const std::string data = json->has("data") ? json->getValue<std::string>("data") : EMPTY_SQF_MESSAGE_DATA;
            if (id == Message::ERROR_ID) {
                log::logger->error("Bad message format! Dropping message '{}'!", jsonStr);
                return;
            }
            clientMessages.push(Message{ id, data });
        }
        catch (Poco::Exception& e) {
            log::logger->error("Bad message format! Dropping message '{}'! Exceptio: {}", jsonStr, e.message());
        }
    }

} // namespace extension
} // namespace wsa
