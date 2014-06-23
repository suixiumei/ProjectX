#pragma once
struct ChunkStruct{
	struct{
		CString result;		//清除缓存
	}ClearCache;
	struct{
		CString result;		//是否更新
	}CheckUpdate;
};