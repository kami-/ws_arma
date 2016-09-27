#include "log.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/LocalDateTime.h"

namespace wsa {
namespace log {
    std::shared_ptr<spdlog::logger> logger;

    bool initialze(const std::string& extensionFolder, const std::string& logLevel) {
        logger = spdlog::rotating_logger_mt("wsa_extension", fmt::format("{}\\{}", extensionFolder, getLogFileName()), 1024 * 1024 * 20, 1, true);
        logger->set_level(getLogLevel(logLevel));
        return true;
    }

    void finalize() {
    };

    std::string getLogFileName() {
        std::string fileName = "wsa_extension_log_";
        Poco::DateTimeFormatter::append(fileName, Poco::LocalDateTime(), "%Y-%m-%d_%H-%M-%S");
        return fileName;
    }

    spdlog::level::level_enum getLogLevel(const std::string& logLevel) {
        if (logLevel == "debug") { return spdlog::level::debug; }
        if (logLevel == "trace") { return spdlog::level::trace; }
        return spdlog::level::info;
    }

} // namespace log
} // namespace wsa
