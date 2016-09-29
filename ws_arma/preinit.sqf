#include "wsa_macros.h"

wsa_isInitialized = false;

[] call compile preProcessFileLineNumbers ADDON_PATH(config_functions.sqf);
wsa_isEnabled = ["isEnabled"] call wsa_config_fnc_getBool;

if (wsa_isEnabled) then {
    [] call compile preProcessFileLineNumbers ADDON_PATH(extension_functions.sqf);
    [] call wsa_ext_fnc_preinit;

    private _testResult = [] call wsa_ext_fnc_test;
    if (_testResult == "") then {
        ERROR("wsa","Failed to load extension! Disabling addon.");
        wsa_isEnabled = false;
    };

    if (wsa_isEnabled) then {
        [] call wsa_ext_fnc_start;
        wsa_isInitialized = true;
        INFO("wsa",FMT_1("WS_Arma version '%1' has been successfully initialized.",WSA_VERSION));
    } else {
        INFO("wsa",FMT_1("WS_Arma version '%1' has been disabled.",WSA_VERSION));
    };
};