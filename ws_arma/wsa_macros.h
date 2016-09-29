#ifndef WSA_MACROS_H
#define WSA_MACROS_H

#define LOGGING_LEVEL_INFO
#define LOGGING_TO_RPT
#include "logbook.h"

#define WSA_VERSION                     "0.1.0"

#define CONCAT_ADDON_PATH(FILE)         x\ark\addons\ws_arma\FILE
#define ADDON_PATH(FILE)                #CONCAT_ADDON_PATH(FILE)

#define WSA_BASE_CONFIG                 "ArkStats"
#define WSA_MISSION_CONFIG_FILE         missionConfigFile >> STATS_BASE_CONFIG
#define WSA_CONFIG_FILE                 configFile >> STATS_BASE_CONFIG

#define WSA_EXTENSION_NAME              "ws_arma_extension"

#define PARAMETER_SEPARATOR             "¤"

#define RESULT_ID_IDX                   0
#define RESULT_DATA_IDX                 1

#define MESSAGE_ID_EMPTY                0
#define MESSAGE_ID_ERROR                2
#define MESSAGE_ID_START                3
#define MESSAGE_ID_GET                  4

#endif //WSA_MACROS_H