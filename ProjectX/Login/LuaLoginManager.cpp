#include "StdAfx.h"
#include "LuaLoginManager.h"

void* __stdcall LuaLoginManager::GetInstance(void*ud)
{
	return LoginManager::GetInstance();
}

static XLLRTGlobalAPI LuaWizardSkinManagerMemberFunctions[] = 
{
	{ "Login", LuaLoginManager::Login },
	{ "Logout", LuaLoginManager::Logout },

    {NULL,NULL}
};

void LuaLoginManager::RegisterSelf(XL_LRT_ENV_HANDLE hEnv)
{
    assert(hEnv);
    if(hEnv == NULL)
    {
        return ;
    }

    XLLRTObject theObject;
	theObject.ClassName = PROJECTX_LOGINMANAGER_LUA_CLASS;
    theObject.MemberFunctions = LuaWizardSkinManagerMemberFunctions;
	theObject.ObjName = PROJECTX_LOGINMANAGER_LUA_OBJ;
    theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaLoginManager::GetInstance;

    long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
    assert(!result);
}

int LuaLoginManager::Login(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	const char* ltwo = lua_tostring(luaState, 3);
	BOOL lthree = lua_toboolean(luaState, 4);
	gc_account = Code_UTF8ToUnicode(lone);
	gc_pass = Code_UTF8ToUnicode(ltwo);
	gc_autoLogin = lthree;
	LoginManager::GetInstance()->Login();

    return 0;
}

int LuaLoginManager::Logout(lua_State* luaState)
{
	LoginManager::GetInstance()->Logout();
	lua_pushnil(luaState);
	return 1;
}