#pragma once
#include "LoginManager.h"

#define PROJECTX_LOGINMANAGER_LUA_CLASS "ProjectX.LoginManager.Class"
#define PROJECTX_LOGINMANAGER_LUA_OBJ "ProjectX.LoginManager"

class LuaLoginManager
{
public:
    static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
    static void* __stdcall GetInstance(void* ud);

	static int Login(lua_State* luaState);
	static int Logout(lua_State* luaState);
};
