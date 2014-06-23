#include "StdAfx.h"
#include "LuaSystemManager.h"

void* __stdcall LuaSystemManager::GetInstance(void*ud)
{
	return SystemManager::GetInstance();
}

static XLLRTGlobalAPI LuaWizardSkinManagerMemberFunctions[] = 
{
	{ "GetProPath", LuaSystemManager::GetProPath },
	{ "GetSkinID", LuaSystemManager::GetSkinID },
	{ "GetTutorials", LuaSystemManager::GetTutorials },
	{ "GetScreenSize", LuaSystemManager::GetScreenSize },
	{ "GetVersion", LuaSystemManager::GetVersion },
	{ "CheckUpdate", LuaSystemManager::CheckUpdate },
	{ "GetProcessIsNot", LuaSystemManager::GetProcessIsNot },
	{ "CheckOnline", LuaSystemManager::CheckOnline },
	{ "GetINI", LuaSystemManager::GetINI },
	{ "SetINI", LuaSystemManager::SetINI },
	{ "OpenUrl", LuaSystemManager::OpenUrl },
	{ "FuncStatis", LuaSystemManager::FuncStatis },
	{ "CopyCString", LuaSystemManager::CopyCString },
	{ "Quit", LuaSystemManager::Quit },
	{ "CovertChineseToPY", LuaSystemManager::CovertChineseToPY },
	{ "CovertToUTF8URLEncode", LuaSystemManager::CovertToUTF8URLEncode },
	
    {NULL,NULL}
};

void LuaSystemManager::RegisterSelf(XL_LRT_ENV_HANDLE hEnv)
{
    assert(hEnv);
    if(hEnv == NULL)
    {
        return ;
    }

    XLLRTObject theObject;
	theObject.ClassName = PROJECTX_SYSTEMMANAGER_LUA_CLASS;
    theObject.MemberFunctions = LuaWizardSkinManagerMemberFunctions;
	theObject.ObjName = PROJECTX_SYSTEMMANAGER_LUA_OBJ;
    theObject.userData = NULL;
	theObject.pfnGetObject = (fnGetObject)LuaSystemManager::GetInstance;

    long result = XLLRT_RegisterGlobalObj(hEnv,theObject); 
    assert(!result);
}

int LuaSystemManager::GetProPath(lua_State* luaState)
{
	lua_pushstring(luaState, Code_UnicodeToUtf8(com.com_method_GetResDir()));
	return 1;
}

int LuaSystemManager::GetSkinID(lua_State* luaState)
{
	CString skin = SystemManager::GetInstance()->GetSkinID();
	lua_pushstring(luaState, Code_UnicodeToUtf8(skin));
	return 1;
}

int LuaSystemManager::GetTutorials(lua_State* luaState)
{
	CString result = SystemManager::GetInstance()->GetTutorials();
	lua_pushstring(luaState, Code_UnicodeToUtf8(result));
	return 1;
}

int LuaSystemManager::GetScreenSize(lua_State* luaState)
{
	//RECT rt;
	//SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rt, 0);
	int cx = 0;
	int cy = 0;
	SystemManager::GetInstance()->GetScreenSize(cx,cy);
	lua_pushinteger(luaState, cx);
	lua_pushinteger(luaState, cy);
	return 2;
}

int LuaSystemManager::GetVersion(lua_State* luaState)
{
	CString result = SystemManager::GetInstance()->GetVersion();
	lua_pushstring(luaState, Code_UnicodeToUtf8(result));
	return 1;
}

int LuaSystemManager::CheckUpdate(lua_State* luaState)
{
	SystemManager::GetInstance()->CheckUpdate();
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::GetProcessIsNot(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	CString result = SystemManager::GetInstance()->GetProcessIsNot(Code_UTF8ToUnicode(lone));
	lua_pushstring(luaState, Code_UnicodeToUtf8(result));
	return 1;
}

int LuaSystemManager::CheckOnline(lua_State* luaState)
{
	CString result = SystemManager::GetInstance()->CheckOnline();
	lua_pushstring(luaState, Code_UnicodeToUtf8(result));
	return 1;
}

int LuaSystemManager::GetINI(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	const char* ltwo = lua_tostring(luaState, 3);
	const char* lthree = lua_tostring(luaState, 4);
	CString result = SystemManager::GetInstance()->GetINI(Code_UTF8ToUnicode(lone), Code_UTF8ToUnicode(ltwo), Code_UTF8ToUnicode(lthree));
	lua_pushstring(luaState, Code_UnicodeToUtf8(result));
	return 1;
}

int LuaSystemManager::SetINI(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	const char* ltwo = lua_tostring(luaState, 3);
	const char* lthree = lua_tostring(luaState, 4);
	SystemManager::GetInstance()->SetINI(Code_UTF8ToUnicode(lone), Code_UTF8ToUnicode(ltwo), Code_UTF8ToUnicode(lthree));
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::OpenUrl(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	SystemManager::GetInstance()->OpenUrl(Code_UTF8ToUnicode(lone));
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::FuncStatis(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	char *buf = new char[strlen(lone) + 1];
	strcpy(buf, lone);
	SystemManager::GetInstance()->FuncStatis(buf);
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::CopyCString(lua_State* luaState)
{
	const char* lone = lua_tostring(luaState, 2);
	SystemManager::GetInstance()->CopyCString(Code_UTF8ToUnicode(lone));
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::Quit(lua_State* luaState)
{
	SystemManager::GetInstance()->Quit();
	lua_pushnil(luaState);
	return 1;
}

int LuaSystemManager::CovertChineseToPY(lua_State* luaState)
{
	const char* sCh = lua_tostring(luaState, 2);
	CString oPY = SystemManager::GetInstance()->CovertChineseToPY(Code_UTF8ToUnicode(sCh));
	lua_pushstring(luaState, Code_UnicodeToUtf8(oPY));
	return 1;
}

int LuaSystemManager::CovertToUTF8URLEncode(lua_State* luaState)
{
	const char* sCh = lua_tostring(luaState, 2);
	CString oPY = SystemManager::GetInstance()->CovertToUTF8URLEncode(Code_UTF8ToUnicode(sCh));
	lua_pushstring(luaState, Code_UnicodeToUtf8(oPY));
	return 1;
}