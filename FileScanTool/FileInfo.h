#ifndef _FILEINFO_H
#define _FILEINFO_H
#pragma once

#include <string>
#include <ctime>
#include <io.h>
using namespace std;

class FileInfo
{
public:
	FileInfo(const string name, const string path, const time_t time_write, const time_t time_creat, const unsigned attrib, _fsize_t size);
	~FileInfo();
	string GetName();
	string GetPath();
	unsigned GetAttrib();
	time_t GetTimeWrite();
	time_t GetTimeCreat();
	_fsize_t GetSize();

private:
	string _name;
	string _path;
	time_t _time_write;
	time_t _time_creat;
	unsigned _attrib;
	_fsize_t _size;
};


#endif // !_FILEINFO_H

