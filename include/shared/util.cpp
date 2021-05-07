#include "stdafx.h"
#include "util.h"
#include "shared/log.h"
#include "shellapi.h"

//
std::string QString::GetGUID()
{
	return nim::Tool::GetUuid();
}

std::string QString::GetMd5(const std::string& input)
{
	return nim::Tool::GetMd5(input);
}

void QString::NIMFreeBuf(void *data)
{
	return nim::Global::FreeBuf(data);
}

//
std::wstring QPath::GetAppPath()
{
	return nbase::win32::GetCurrentModuleDirectory();
}

std::wstring QPath::GetUserAppDataDir(const std::string& app_account)
{
	return nim::Tool::GetUserAppdataDir(app_account);
}

std::wstring QPath::GetLocalAppDataDir()
{
	return nim::Tool::GetLocalAppdataDir();
}

std::wstring QPath::GetNimAppDataDir()
{
	std::wstring dir = QPath::GetLocalAppDataDir();
#ifdef _DEBUG
	dir.append(L"Netease\\NIM_Debug\\");
#else
	dir.append(L"Netease\\NIM\\");
#endif

	return dir;
}

std::list<std::wstring> Split(const std::wstring& input, const std::wstring& delimitor)
{
	std::list<std::wstring> output;
	std::wstring input2(input);

	if (input2.empty())
		return output;

	wchar_t *token = wcstok(&input2[0], delimitor.c_str());
	while (token != NULL)
	{
		output.push_back(token);
		token = wcstok(NULL, delimitor.c_str());
	}


	return output;
}

std::list<std::string> Split(const std::string& input, const std::string& delimitor)
{
	std::list<std::string> output;
	std::string input2(input);

	if (input2.empty())
		return output;

	char *token = strtok(&input2[0], delimitor.c_str());
	while (token != NULL)
	{
		output.push_back(token);
		token = strtok(NULL, delimitor.c_str());
	}

	return output;
}

std::map<std::wstring,std::wstring> QCommand::key_value_;

void QCommand::ParseCommand( const std::wstring &cmd )
{
	//这里的 ui::StringHelper::Split是云信duilib里面的函数，你的项目没有用云信duilib，所以这里编译错误，要替换一下

	std::list<std::wstring> arrays = Split(cmd, L"/");
	for(std::list<std::wstring>::const_iterator i = arrays.begin(); i != arrays.end(); i++)
	{
		std::list<std::wstring> object = Split(*i, L" ");
		assert(object.size() == 2);
		key_value_[ *object.begin() ] = *object.rbegin();
	}
}

std::wstring QCommand::Get( const std::wstring &key )
{
	std::map<std::wstring,std::wstring>::const_iterator i = key_value_.find(key);
	if(i == key_value_.end())
		return L"";
	else
		return i->second;
}

void QCommand::Set( const std::wstring &key, const std::wstring &value )
{
	key_value_[key] = value;
}

void QCommand::Erase(const std::wstring &key)
{
	key_value_.erase(key);
}

bool QCommand::AppStartWidthCommand( const std::wstring &app, const std::wstring &cmd )
{
	HINSTANCE hInst = ::ShellExecuteW(NULL, L"open", app.c_str(), cmd.c_str(), NULL, SW_SHOWNORMAL);
	return (int)hInst > 32;
}

bool QCommand::RestartApp(const std::wstring &cmd)
{
	wchar_t app[1024] = { 0 };
	GetModuleFileName(NULL, app, 1024);
	HINSTANCE hInst = ::ShellExecuteW(NULL, L"open", app, cmd.c_str(), NULL, SW_SHOWNORMAL);
	return (int)hInst > 32;
}