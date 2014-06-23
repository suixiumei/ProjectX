#pragma once
#include "SystemManager.h"

#define PROJECTX_SYSTEMMANAGER_LUA_CLASS "ProjectX.SystemManager.Class"
#define PROJECTX_SYSTEMMANAGER_LUA_OBJ "ProjectX.SystemManager"

class LuaSystemManager
{
public:
    static void RegisterSelf(XL_LRT_ENV_HANDLE hEnv);
    static void* __stdcall GetInstance(void* ud);

	static int GetProPath(lua_State* luaState);
	static int GetSkinID(lua_State* luaState);
	static int GetTutorials(lua_State* luaState);
	static int GetScreenSize(lua_State* luaState);//获取屏幕宽高：去掉任务栏后
	static int GetVersion(lua_State* luaState);
	static int CheckUpdate(lua_State* luaState);
	static int GetProcessIsNot(lua_State* luaState);
	static int CheckOnline(lua_State* luaState);
	static int GetINI(lua_State* luaState);
	static int SetINI(lua_State* luaState);
	static int OpenUrl(lua_State* luaState);
	static int FuncStatis(lua_State* luaState);
	static int CopyCString(lua_State* luaState);
	static int Quit(lua_State* luaState);
	static int CovertChineseToPY(lua_State* luaState);//将一段传入的中文转换为拼音首字母并返回
	static int CovertToUTF8URLEncode(lua_State* luaState);//将一段传入的sCh转换为utf8的urlencode并返回
};
