#pragma once 
#define DECODE_ERROR 0xffffffff 
static char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";   

int CStringEncodeBase64(const CString &szInput, CString &szOutput); 
int CStringDecodeBase64(const CString &szInput, CString &szOutput);   
int base64_encode(const void *data, const int size, char **str); 
static int pos(const char c); 
static unsigned int token_decode(const char *token); 
int base64_decode(const char *str, char *data); 

//
// URL±àÂë
//
CString getUTF8URLEncode(CString str);
CStringW getUTF8URLDecode(const wchar_t* pStr);
CString  getGB2312URLEncode(CString str);
CString  getGB2312URLDecode(CString str);

//
// ±àÂë¼ÓÃÜ
//
CString Code_Encrypt(CString S, WORD Key) ;
CString Code_Decrypt(CString S, WORD Key);

//
// ±àÂë×ª»»
//
char* Code_UnicodeToUtf8(CString unicode);	
char* Code_UnicodeToAnsi(CString unicode);
CString Code_UTF8ToUnicode(const char* str);
