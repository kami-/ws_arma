#include "wsa_macros.h"


wsa_ext_fnc_preinit = {
    wsa_ext_hasError = false;
    DEBUG("wsa.ext","Preinit done.");
};

wsa_ext_fnc_checkError = {
    FUN_ARGS_1(_result);

    TRACE("wsa.ext",FMT_1("Checking for error in result '%1'.",_result));
    if (isNil {_result} || {_result select RESULT_TYPE_IDX == RESULT_TYPE_ERROR}) then {
        ERROR("wsa.ext",FMT_1("There was an error in result '%1'.",_result));
        wsa_ext_hasError = true;
    };
};

wsa_ext_fnc_test = {
    private _request = format ["%1%2", MESSAGE_ID_EMPTY, PARAMETER_SEPARATOR];
    DEBUG("wsa.ext",FMT_1("Testing extension with request '%1'.",_request));
    _result = EXTENSION_NAME callExtension _request;
    DEBUG("wsa.ext",FMT_1("Extension test result was '%1'.",_result));

    _result;
};

wsa_ext_fnc_start = {
    private _request = format ["%1%2%3", MESSAGE_ID_EMPTY, PARAMETER_SEPARATOR];
    DEBUG("wsa.ext",FMT_1("Testing extension with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'connect' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_isSession = {
    private ["_request", "_result"];
    _request = format ["%1", CALL_TYPE_ID_IS_SESSION];
    DEBUG("wsa.ext",FMT_1("Calling extension 'is_session' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'is_session' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_mission = {
    private ["_request", "_result"];
    _request = format ["%1", CALL_TYPE_ID_MISSION];
    DEBUG("wsa.ext",FMT_1("Calling extension 'mission' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'mission' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_missionAttribute = {
    FUN_ARGS_3(_attributeTypeId,_numericValue,_charValue);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3:%4", CALL_TYPE_ID_MISSION_ATTRIBUTE, _attributeTypeId, _numericValue, _charValue];
    DEBUG("wsa.ext",FMT_1("Calling extension 'mission_attribute' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'mission_attribute' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_missionEvent = {
    FUN_ARGS_3(_eventTypeId,_numericValue,_charValue);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3:%4:%5", CALL_TYPE_ID_MISSION_EVENT, time, _eventTypeId, _numericValue, _charValue];
    DEBUG("wsa.ext",FMT_1("Calling extension 'mission_event' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'mission_event' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_entity = {
    FUN_ARGS_1(_entityId);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3", CALL_TYPE_ID_ENTITY, _entityId, time];
    DEBUG("wsa.ext",FMT_1("Calling extension 'entity' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'entity' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_entityAttribute = {
    FUN_ARGS_4(_entityId,_attributeTypeId,_numericValue,_charValue);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3:%4:%5", CALL_TYPE_ID_ENTITY_ATTRIBUTE, _entityId, _attributeTypeId, _numericValue, _charValue];
    DEBUG("wsa.ext",FMT_1("Calling extension 'entity_attribute' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'entity_attribute' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_entityEvent = {
    FUN_ARGS_4(_entityId,_eventTypeId,_numericValue,_charValue);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3:%4:%5:%6", CALL_TYPE_ID_ENTITY_EVENT, _entityId, time, _eventTypeId, _numericValue, _charValue];
    DEBUG("wsa.ext",FMT_1("Calling extension 'entity_event' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'entity_event' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};

wsa_ext_fnc_entityPosition = {
    FUN_ARGS_5(_entityId,_positionTypeId,_position,_height,_direction);

    private ["_request", "_result"];
    _request = format ["%1:%2:%3:%4:%5:%6:%7:%8:%9", CALL_TYPE_ID_ENTITY_POSITION, _entityId, time, _positionTypeId, _position select 0, _position select 1, _position select 2, _height, _direction];
    DEBUG("wsa.ext",FMT_1("Calling extension 'entity_position' command with request '%1'.",_request));
    _result = call compile (EXTENSION_NAME callExtension _request);
    DEBUG("wsa.ext",FMT_1("Extension 'entity_position' command result was '%1'.",_result));
    [_result] call wsa_ext_fnc_checkError;

    _result select RESULT_DATA_IDX;
};