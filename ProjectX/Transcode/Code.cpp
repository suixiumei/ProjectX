#include "stdafx.h"
#include <iostream>
#include <string>
using namespace std;
int CStringEncodeBase64(const CString &szInput, CString &szOutput)
{     
	int nRet = 0;     
	int nLeng = 0;     
	nLeng    = (szInput.GetLength() + 1)*sizeof(TCHAR);     
	char *pInput = new char[nLeng];    
	ZeroMemory(pInput, nLeng);     
	char *pOutput = new char[nLeng];        
	ZeroMemory(pOutput, nLeng);    
	//USES_CONVERSION;     
	strcpy_s(pInput, nLeng, CT2A(szInput));     
	nRet = base64_encode((void *)pInput, strlen(pInput), &pOutput);     
	szOutput = CA2T(pOutput);    
	delete []pInput;    
	delete []pOutput;    
	return nRet; 
} 

int CStringDecodeBase64(const CString &szInput, CString &szOutput) 
{     
	int nRet = 0;     
	int nLeng = 0;    
	nLeng    = (szInput.GetLength() + 1)*sizeof(TCHAR);     
	char *pInput = new char[nLeng];     
	ZeroMemory(pInput, nLeng);     
	char *pOutput = new char[nLeng];         
	ZeroMemory(pOutput, nLeng);     
	//USES_CONVERSION;     
	strcpy_s(pInput, nLeng, CT2A(szInput));     
	nRet = base64_decode(pInput, pOutput);     
	szOutput = CA2T(pOutput);     
	delete [] pInput;     
	delete [] pOutput;     
	return nRet;
} 

int base64_encode(const void *data, const int size, char **str)
{    
	char *s, *p;     
	int i;     
	int c;    
	const unsigned char *q;       
	p = s = (char *) malloc(size * 4 / 3 + 4);    
	if (p == NULL)    
	{        
		return -1;    
	}     
	q = (const unsigned char *) data;    
	i = 0;     
	for (i = 0; i < size;)     
	{         
		c = q[i++];         
		c *= 256;         
		if (i < size)         
		{            
			c += q[i];        
		}         
		i++;         
		c *= 256;         
		if (i < size)         
		{             
			c += q[i];        
		}         
		i++;         
		p[0] = base64_chars[(c & 0x00fc0000) >> 18];         
		p[1] = base64_chars[(c & 0x0003f000) >> 12];         
		p[2] = base64_chars[(c & 0x00000fc0) >> 6];         
		p[3] = base64_chars[(c & 0x0000003f) >> 0];        
		if (i > size)         
		{             
			p[3] = '=';         
		}         
		if (i > size + 1)         
		{             
			p[2] = '=';         
		}         
		p += 4;    
	}       
	*p = 0;     
	*str = s;     
	return strlen(s); 
} 

static int pos(const char c) 
{     
	char *p;     
	for (p = base64_chars; *p; p++)    
	{        
		if (*p == c)        
		{             
			return p-base64_chars;         
		}     
	}     
	return -1; 
} 

static unsigned int token_decode(const char *token) 
{     
	int i;     
	unsigned int val = 0;     
	int marker = 0;     
	if (strlen(token) < 4)     
	{         
		return DECODE_ERROR;     
	}     
	for (i = 0; i < 4; i++)     
	{         
		val *= 64;        
		if (token[i] == '=')        
		{             
			marker++;        
		}         
		else        
		{            
			if (marker > 0)             
			{                 
				return DECODE_ERROR;             
			}            
			else             
			{                 
				val += pos(token[i]);             
			}         
		}                 
	}     
	if (marker > 2)     
	{         
		return DECODE_ERROR;     
	}     
	return (marker << 24) | val; 
} 

int base64_decode(const char *str, char *data) 
{     
	const char *p;     
	unsigned char *q;       
	q = (unsigned char *)data;     
	for (p = str; *p && (*p == '=' || strchr(base64_chars, *p)); p += 4)     
	{         
		unsigned int val = token_decode(p);         
		unsigned int marker = (val >> 24) & 0xff;         
		if (val == DECODE_ERROR)         
		{             
			return -1;         
		}         
		*q++ = (val >> 16) & 0xff;        
		if (marker < 2)         
		{             
			*q++ = (val >> 8) & 0xff;         
		}         
		if (marker < 1)        
		{             
			*q++ = val & 0xff;         
		}     
	}     
	return q - (unsigned char *) data; 
} 

CString Code_Encrypt(CString S, WORD Key)
{
	CString Result,str;
	int i,j;

	Result=S; // 初始化结果字符串
	for(i=0; i<S.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)Result.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	S=Result; // 保存结果
	Result.Empty(); // 清除结果
	for(i=0; i<S.GetLength(); i++) // 对加密结果进行转换
	{
		j=(BYTE)S.GetAt(i); // 提取字符
		// 将字符转换为两个字母保存
		str=L"12"; // 设置str长度为2
		str.SetAt(0, 65+j/26);//这里将65改大点的数例如256，密文就会变乱码，效果更好，相应的，解密处要改为相同的数
		str.SetAt(1, 65+j%26);
		Result += str;
	}
	return Result;
}

CString Code_Decrypt(CString S, WORD Key)
{
	CString Result,str;
	int i,j;

	Result.Empty(); // 清除结果
	for(i=0; i < S.GetLength()/2; i++) // 将字符串两个字母一组进行处理
	{
		j = ((BYTE)S.GetAt(2*i)-65)*26;//相应的，解密处要改为相同的数

		j += (BYTE)S.GetAt(2*i+1)-65;
		str=L"1"; // 设置str长度为1
		str.SetAt(0, j);
		Result+=str; // 追加字符，还原字符串
	}
	S=Result; // 保存中间结果
	for(i=0; i<S.GetLength(); i++) // 依次对字符串中各字符进行操作
	{
		Result.SetAt(i, (BYTE)S.GetAt(i)^(Key>>8)); // 将密钥移位后与字符异或
		Key = ((BYTE)S.GetAt(i)+Key)*C1+C2; // 产生下一个密钥
	}
	return Result;
}

bool Unicode2UTF8(const wchar_t *input_unicode,
									  char ** p_output_utf8,
									  unsigned long *length)
{
	if (input_unicode == NULL) {
		return true;
	}
	int size_d = 8;
	int buffer_size = 0;

	const wchar_t* p_unicode = input_unicode;
	// count for the space need to allocate
	wchar_t w_char;
	do {
		w_char = *p_unicode;
		if (w_char < 0x80) {
			// utf char size is 1
			buffer_size += 1;
		} else if (w_char < 0x800) {
			// utf char size is 2
			buffer_size += 2;
		} else if (w_char < 0x10000) {
			// utf char size is 3
			buffer_size += 3;
		} else if (w_char < 0x200000) {
			// utf char size is 4
			buffer_size += 4;
		} else if (w_char < 0x4000000) {
			// utf char size is 5
			buffer_size += 5;
		} else {
			// utf char size is 6
			buffer_size += 6;
		}
		p_unicode++;
	}
	while (w_char != static_cast<char>(0));
	// allocate the memory
	char* utf8 = new char[buffer_size];

	p_unicode = input_unicode;
	int index_buffer = 0;
	// do the conversion
	do {
		w_char = *input_unicode;  // the unicode char current being converted
		input_unicode++;

		if (w_char < 0x80) {
			// length = 1;
			utf8[index_buffer++] = static_cast<char>(w_char);
		} else if (w_char < 0x800) {
			// length = 2;
			utf8[index_buffer++] = 0xc0 | (w_char >> 6);
			utf8[index_buffer++] = 0x80 | (w_char & 0x3f);
		} else if (w_char < 0x10000) {
			// length = 3;
			utf8[index_buffer++] = 0xe0 | (w_char >> 12);
			utf8[index_buffer++] = 0x80 | ((w_char >> 6) & 0x3f);
			utf8[index_buffer++] = 0x80 | (w_char & 0x3f);
		} else if (w_char < 0x200000) {
			// length = 4;
			utf8[index_buffer++] = 0xf0 | (static_cast<int>(w_char) >> 18);
			utf8[index_buffer++] = 0x80 | ((w_char >> 12) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((w_char >> 6) & 0x3f);
			utf8[index_buffer++] = 0x80 | (w_char & 0x3f);
		} else if (w_char < 0x4000000) {
			// length = 5
			utf8[index_buffer++] = 0xf8| (static_cast<int>(w_char) >> 24);
			utf8[index_buffer++] = 0x80 | ((static_cast<int>(w_char) >> 18) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((w_char >> 12) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((w_char >> 6) & 0x3f);
			utf8[index_buffer++] = 0x80 | (w_char & 0x3f);
		} else {  // if(wchar >= 0x4000000)
			// all other cases length = 6
			utf8[index_buffer++] = 0xfc | (static_cast<int>(w_char) >> 30);
			utf8[index_buffer++] = 0x80 | ((static_cast<int>(w_char) >> 24) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((static_cast<int>(w_char) >> 18) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((w_char >> 12) & 0x3f);
			utf8[index_buffer++] = 0x80 | ((w_char >> 6) & 0x3f);
			utf8[index_buffer++] = 0x80 | (w_char & 0x3f);
		}
	}
	while (w_char !=  static_cast<char>(0));

	// set the output length
	*length = buffer_size - 1;  // ignore last 

	// set the output charset
	*p_output_utf8 = utf8;
	return false;
}

bool UTF82UrlEncode(const char* input_utf8,
										char ** p_url_encode,
										unsigned long *length) {
											unsigned long total_length = 0;
											// calculate output size
											const char* current_pos = input_utf8;
											do {
												// the char needs to be encode
												if (!((*current_pos) >= 'a' && (*current_pos) <= 'z') &&
													!((*current_pos) >= 'A' && (*current_pos) <= 'Z') &&
													!((*current_pos) >= '0' && (*current_pos) <= '9') &&
													!((*current_pos) == ' ') &&
													!((*current_pos) == '.') &&
													!((*current_pos) == '-') &&
													!((*current_pos) == '_') &&
													!((*current_pos) == '~') &&
													!((*current_pos) == '\'') &&
													!((*current_pos) == '(') &&
													!((*current_pos) == ')') &&
													!((*current_pos) == '*') &&
													!((*current_pos) == 0)
													) {
														total_length += 3;
												} else {
													total_length++;
												}
												if ((*current_pos) == 0) {  // the end of str
													break;
												}
												current_pos++;

											} while (true);
											char* output_buffer = new char[total_length];
											current_pos = input_utf8;
											char* target_current_pos = output_buffer;
											unsigned long left_size = total_length;
											// do conversion
											do {
												// the char needs to be encode
												if (!((*current_pos) >= 'a' && (*current_pos) <= 'z') &&
													!((*current_pos) >= 'A' && (*current_pos) <= 'Z') &&
													!((*current_pos) >= '0' && (*current_pos) <= '9') &&
													!((*current_pos) == ' ') &&
													!((*current_pos) == '.') &&
													!((*current_pos) == '-') &&
													!((*current_pos) == '_') &&
													!((*current_pos) == '~') &&
													!((*current_pos) == '\'') &&
													!((*current_pos) == '(') &&
													!((*current_pos) == ')') &&
													!((*current_pos) == '*') &&
													!((*current_pos) == 0)
													) {
														unsigned char temp_unsigned_char = *current_pos;
														// convert to unsigned to skip the sign bit

														::_snprintf_s(target_current_pos,
															left_size,
															_TRUNCATE ,
															"%%%02X",
															temp_unsigned_char);
														target_current_pos += 3;
														left_size -= 3;
												} else if ((*current_pos) ==  ' ') {
													// the char blank
													*target_current_pos = '+';
													target_current_pos++;
													left_size--;
												} else {
													// char that need not change
													*target_current_pos = *current_pos;
													target_current_pos++;
													left_size--;
												}
												if ((*current_pos) == 0) {
													break;
												}
												current_pos++;
											} while (true);

											*p_url_encode = output_buffer;
											*length = total_length - 1;  // ignore last 
											return true;
}

CString getUTF8URLEncode(CString str)
{
	char *bb;
	unsigned long n;
	Unicode2UTF8(str,&bb,&n);
	char *dd;
	unsigned long value;
	UTF82UrlEncode(bb,&dd,&value);
	delete [] bb;
	string valuett=dd;
	delete []dd;
	CString outstr=CString(valuett.c_str());
	return outstr;
}

inline char FromHex(const char x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}

CStringA Emrys_URLDecode( const CStringA& src )
{
	CStringA decodeURL;
	for(int i = 0; i < src.GetLength(); i++)
	{
		unsigned char ch = 0;
		if(src[i]=='%')
		{
			ATLASSERT(i + 2 < src.GetLength());

			unsigned char c1 = src[i + 1];
			unsigned char c2 = src[i + 2];
			ch = ((FromHex(c1) << 4) & 0xf0);
			ch |= (FromHex(c2) & 0x0f);
			i += 2;
		}
		else if(src[i] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = src[i];
		}
		decodeURL += (unsigned char)ch;
	}

	return decodeURL;
}

CStringW UTF82Unicode(const char* pChar)   
{   
	ATLASSERT(NULL != pChar);

	int targetLen = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pChar, -1, NULL, 0);   

	WCHAR* pTargetData = new WCHAR[targetLen+1];   
	memset(pTargetData, 0, (targetLen+1) * sizeof(WCHAR));   
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)pChar, -1, (WCHAR*)pTargetData, targetLen);   

	CStringW strRet(pTargetData);
	delete pTargetData;   
	return strRet;   
}

CStringW getUTF8URLDecode(const wchar_t* pStr)
{
	return UTF82Unicode(Emrys_URLDecode(CStringA(pStr)));
}

char* Code_UnicodeToUtf8(CString unicode)
{
	std::string std_str;
	XLUETranscode::Unicode_to_UTF8(unicode,::wcslen(unicode),std_str);
	return strdup(std_str.c_str());
}

char* Code_UnicodeToAnsi(CString unicode)
{
	std::string std_str;
	XLUETranscode::Unicode_to_ANSI(unicode,::wcslen(unicode),std_str);
	return strdup(std_str.c_str());
}

CString Code_UTF8ToUnicode(const char* str)
{
	std::wstring temp;
	XLUETranscode::UTF8_to_Unicode(str, ::strlen(str), temp);
	return temp.c_str(); 
}

void CString2Char(CString cstr, char* &saRet){
	std::wstring str(cstr.GetBuffer());
	int nLength = str.length() + 1;
	TCHAR *c_str = new TCHAR[nLength];
	//char *saRet = new char[nLength];
	wcscpy_s(c_str, nLength, str.c_str());
	WideCharToMultiByte(CP_ACP, 0, cstr, nLength, saRet,
		nLength * 2 + 1, NULL, NULL);
	cstr.ReleaseBuffer();
	delete[]c_str;
	c_str = NULL;
}

string UrlEmrysGB2312(char * str)
{
	string dd;
	size_t len = strlen(str);
	for (size_t i = 0; i < len; i++)
	{
		if (isalnum((BYTE)str[i]))
		{
			char tempbuff[2];
			sprintf_s(tempbuff, 2, "%c", str[i]);
			dd.append(tempbuff);
		}
		else if (isspace((BYTE)str[i]))
		{
			dd.append("+");
		}
		else
		{
			char tempbuff[4];
			sprintf_s(tempbuff, 4, "%%%X%X", ((BYTE*)str)[i] >> 4, ((BYTE*)str)[i] % 16);
			dd.append(tempbuff);
		}

	}
	return dd;
}
//做为解Url使用
char CharToInt(char ch)
{
	if (ch >= '0' && ch <= '9') return (char)(ch - '0');
	if (ch >= 'a' && ch <= 'f') return (char)(ch - 'a' + 10);
	if (ch >= 'A' && ch <= 'F') return (char)(ch - 'A' + 10);
	return -1;
}
char StrToBin(char *str)
{
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0  -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}
//把url GB2312解码
string UrlEmrysGB2312Decode(string str)
{
	string output = "";
	char tmp[2];
	int i = 0, idx = 0, len = str.length();

	while (i < len)
	{
		if (str[i] == '%')
		{
			tmp[0] = str[i + 1];
			tmp[1] = str[i + 2];
			output += StrToBin(tmp);
			i = i + 3;
		}
		else if (str[i] == '+')
		{
			output += ' ';
			i++;
		}
		else{
			output += str[i];
			i++;
		}
	}

	return output;
}

CString  getGB2312URLEncode(CString str)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char *temp = new char[dwNum + 1];
	memset(temp, 0, dwNum + 1);
	CString2Char(str, temp);
	string result = UrlEmrysGB2312(temp);
	str = result.c_str();
	delete[] temp;
	return str;
}
CString  getGB2312URLDecode(CString str)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, str, -1, NULL, 0, NULL, FALSE);
	char *temp = new char[dwNum + 1];
	memset(temp, 0, dwNum + 1);
	CString2Char(str, temp);
	string tempData = temp;
	delete[] temp;
	temp = NULL;
	tempData = UrlEmrysGB2312Decode(tempData);
	str = tempData.c_str();
	return str;
}