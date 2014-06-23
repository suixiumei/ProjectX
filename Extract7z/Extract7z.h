#pragma once

typedef void (*OnExSize)(UINT);

UINT GetUnpackSize(LPCTSTR lpFileName);
BOOL Extract7zFile(LPCTSTR lpFileName, LPCTSTR lpDestPath, OnExSize lpOnExSize);
