#include "StdAfx.h"
#include "LuaCustomManager.h"

void* __stdcall LuaCustomManager::GetInstance(void*ud)
{
	return CustomManager::GetInstance();
}

static XLLRTGlobalAPI LuaWizardSkinManagerMemberFunctions[] = 
{
	{ "ClearCache", LuaCustomManager::ClearCache },
	{ "WebObjInit", LuaCustomManager::WebObjInit },
	{ "CallJScript", LuaCustomManager::CallJScript },

    {NULL,NULL}
};

void LuaCustomManager::RegisterSelf(XL_LRT_ENV_HANDLE hEnv)
{
    assert(hEnv);
    if(hEnv == NULL)
    {
        return ;
    }

    XLLRTObject theObject;
	theObject.ClassName = PROJECTX_CUSTOMMANAGER_LUA_CLASS;
    theObject.MemberFunctions = LuaWizardSkinManagerMemberFunctions;
	theObject.ObjName = PROJECTX_CUSTOMMANAGER_LUA_OBJ;
    theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaCustomManager::GetInstance;

    long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
    assert(!result);
}

int LuaCustomManager::ClearCache(lua_State* luaState)
{
	CustomManager::GetInstance()->ClearCache();
	lua_pushnil(luaState);
	return 1;
}

int LuaCustomManager::WebObjInit(lua_State* luaState)
{
	const void* p = lua_touserdata(luaState, 2);
	CustomManager::GetInstance()->WebObjInit(*(IWebBrowser2**)p);
	lua_pushnil(luaState);
	return 1;
}

int LuaCustomManager::CallJScript(lua_State* luaState)
{
	const char* JSFunc = lua_tostring(luaState, 2);
	CustomManager::GetInstance()->CallJScript((CComBSTR)JSFunc);
	lua_pushnil(luaState);
	return 1;
}