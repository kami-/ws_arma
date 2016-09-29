#include "wsa_macros.h"

class CfgPatches {
    class WS_Arma {
        units[] = {};
        weapons[] = {};
        requiredVersion = 1.0;
        requiredAddons[] = {};
        author[] = {"Kami", "Ark"};
        authorUrl = "https://github.com/kami-";
    };
};

class Extended_PreInit_EventHandlers {
    class WS_Arma {
        init = "[] call compile preProcessFileLineNumbers 'x\ark\addons\wsa\preinit.sqf';";
    };
};

class Extended_PostInit_EventHandlers {
    class WS_Arma {
        init = "[] call compile preProcessFileLineNumbers 'x\ark\addons\wsa\postinit.sqf';";
    };
};

#include "ws_arma.h"
