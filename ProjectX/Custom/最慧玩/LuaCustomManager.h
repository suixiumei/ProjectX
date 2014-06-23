#pragma once
#include "CustomManager.h"

#define PROJECTX_CUSTOMMANAGER_LUA_CLASS "ProjectX.CustomManager.Class"
#define PROJECTX_CUSTOMMANAGER_LUA_OBJ "ProjectX.CustomManager"

class LuaCustomManager
{
public:
    static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
    static void* __stdcall GetInstance(void* ud);

	static int ClearCache(lua_State* luaState);
	static int WebObjInit(lua_State* luaState);
	static int CallJScript(lua_State* luaState);
};
