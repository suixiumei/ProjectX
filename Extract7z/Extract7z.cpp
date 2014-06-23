extern "C"
{
#include "7z.h"
#include "7zAlloc.h"
#include "7zCrc.h"
#include "7zFile.h"
#include "CpuArch.h"
}

#include "Extract7z.h"

#ifdef _UNICODE
#define _T2CW
#else
LPCWSTR _T2CW(LPCSTR lpStr)
{
	static wchar_t s_wszStr[1024];
	MultiByteToWideChar(CP_ACP, 0, lpStr, -1, s_wszStr, sizeof(s_wszStr) / sizeof(wchar_t));
	return s_wszStr;
}
#endif

SHSTDAPI_(int) SHCreateDirectory(HWND hwnd, LPCWSTR pszPath);

static BOOL FindSignature(CSzFile *stream)
{
	DWORD dwSize = GetFileSize(stream->handle, NULL) - k7zStartHeaderSize;
	if ((int)dwSize <= 0)
		return FALSE;

	HANDLE hFileMap = CreateFileMapping(stream->handle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (hFileMap == NULL)
		return FALSE;

	char *lpBuffer = (char *)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	if (lpBuffer == NULL)
	{
		CloseHandle(hFileMap);
		return FALSE;
	}

	for (int i = 0; i != dwSize; i++)
	{
		if (*(WORD *)(lpBuffer + i) == 'z7' && memcmp(lpBuffer + i, k7zSignature, k7zSignatureSize) == 0)
		{
			if (CrcCalc(lpBuffer + i + 12, 20) == GetUi32(lpBuffer + i + 8))
			{
				Int64 iSkip = i;
				File_Seek(stream, &iSkip, SZ_SEEK_SET);

				UnmapViewOfFile(lpBuffer);
				CloseHandle(hFileMap);
				return TRUE;
			}
		}
	}

	UnmapViewOfFile(lpBuffer);
	CloseHandle(hFileMap);
	return FALSE;
}

UINT GetUnpackSize(LPCTSTR lpFileName)
{
	CrcGenerateTable();

	ISzAlloc allocImp;
	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	ISzAlloc allocTempImp;
	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	CFileInStream archiveStream;
	FileInStream_CreateVTable(&archiveStream);

	if (InFile_OpenW(&archiveStream.file, _T2CW(lpFileName)) != 0)
		return 0;

	if (FindSignature(&archiveStream.file) == FALSE)
	{
		File_Close(&archiveStream.file);
		return 0;
	}

	CLookToRead lookStream;
	LookToRead_CreateVTable(&lookStream, FALSE);

	lookStream.realStream = &archiveStream.s;
	LookToRead_Init(&lookStream);

	CSzArEx db;
	SzArEx_Init(&db);

	if (SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp) != SZ_OK)
	{
		File_Close(&archiveStream.file);
		return 0;
	}

	UINT nTotal = 0;

	for (int i = 0; i != db.db.NumFolders; i++)
		nTotal += (UINT)SzFolder_GetUnpackSize(db.db.Folders + i);

	SzArEx_Free(&db, &allocImp);
	File_Close(&archiveStream.file);

	return nTotal;
}

static UINT g_nBaseLen, g_nOffset;
static OnExSize g_lpOnExSize = NULL;

extern "C" void OnExtractPos(size_t nPos)
{
	g_nOffset = (UINT)nPos;

	if (g_lpOnExSize != NULL)
		g_lpOnExSize(g_nBaseLen + g_nOffset);
}

BOOL Extract7zFile(LPCTSTR lpFileName, LPCTSTR lpDestPath, OnExSize lpOnExSize)
{
	CrcGenerateTable();

	ISzAlloc allocImp;
	allocImp.Alloc = SzAlloc;
	allocImp.Free = SzFree;

	ISzAlloc allocTempImp;
	allocTempImp.Alloc = SzAllocTemp;
	allocTempImp.Free = SzFreeTemp;

	CFileInStream archiveStream;
	FileInStream_CreateVTable(&archiveStream);

	if (InFile_OpenW(&archiveStream.file, _T2CW(lpFileName)) != 0)
		return FALSE;

	if (FindSignature(&archiveStream.file) == FALSE)
	{
		File_Close(&archiveStream.file);
		return FALSE;
	}

	CLookToRead lookStream;
	LookToRead_CreateVTable(&lookStream, FALSE);

	lookStream.realStream = &archiveStream.s;
	LookToRead_Init(&lookStream);

	CSzArEx db;
	SzArEx_Init(&db);

	if (SzArEx_Open(&db, &lookStream.s, &allocImp, &allocTempImp) != SZ_OK)
	{
		File_Close(&archiveStream.file);
		return FALSE;
	}

	wchar_t szDestPath[MAX_PATH];
	lstrcpyW(szDestPath, _T2CW(lpDestPath));
	SHCreateDirectory(NULL, szDestPath);

	DWORD dwPathLen = lstrlenW(szDestPath);
	if (szDestPath[dwPathLen - 1] != '\\')
	{
		szDestPath[dwPathLen++] = '\\';
		szDestPath[dwPathLen] = 0;
	}

	BOOL bResult = TRUE;
	g_nBaseLen = g_nOffset = 0;
	g_lpOnExSize = lpOnExSize;

	UInt32 blockIndex = -1;
	Byte *outBuffer = NULL;
	size_t outBufferSize = 0;

	for (UInt32 i = 0; i < db.db.NumFiles; i++)
	{
		wchar_t *lpTemp = szDestPath + dwPathLen;
		SzArEx_GetFileNameUtf16(&db, i, (UInt16 *)lpTemp);

		for (int j = 0; lpTemp[j] != 0; j++)
		{
			if (lpTemp[j] == '/')
			{
				lpTemp[j] = 0;
				CreateDirectoryW(szDestPath, NULL);
				lpTemp[j] = CHAR_PATH_SEPARATOR;
			}
		}

		CSzFileItem *f = db.db.Files + i;
		if (f->IsDir)
		{
			CreateDirectoryW(szDestPath, NULL);
			continue;
		}

		size_t offset = 0;
		size_t outSizeProcessed = 0;

		if (SzArEx_Extract(&db, &lookStream.s, i, &blockIndex, &outBuffer, &outBufferSize, &offset, &outSizeProcessed, &allocImp, &allocTempImp) != SZ_OK)
		{
			bResult = FALSE;
			break;
		}

		g_nBaseLen += g_nOffset;
		g_nOffset = 0;

		CSzFile outFile;
		if (OutFile_OpenW(&outFile, szDestPath) == 0)
		{
			size_t processedSize = outSizeProcessed;
			File_Write(&outFile, outBuffer + offset, &processedSize);

			if (f->MTimeDefined)
			{
				FILETIME mTime;
				mTime.dwLowDateTime = f->MTime.Low;
				mTime.dwHighDateTime = f->MTime.High;
				SetFileTime(outFile.handle, NULL, NULL, &mTime);
			}

			File_Close(&outFile);
		}
	}

	IAlloc_Free(&allocImp, outBuffer);
	SzArEx_Free(&db, &allocImp);
	File_Close(&archiveStream.file);

	g_lpOnExSize = NULL;
	return bResult;
}
