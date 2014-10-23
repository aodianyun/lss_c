#include <string>
#include <atlbase.h>
#include <atlapp.h>
#include<atlcom.h>
#include<atlhost.h>
#include <atlmisc.h>

#include <atlstr.h>
//#include "stdatl.h"

using std::string;
using std::wstring;

class ConvertString
{
private:
	static std::string WChar2Ansi(LPCWSTR pwszSrc);
	static std::wstring Ansi2WChar(LPCSTR pszSrc, int nLen);
public:
	static std::string ws2s(wstring& inputws);
	static std::wstring s2ws(const string& s);
};