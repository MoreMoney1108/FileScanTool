#include "FileInfo.h"

FileInfo::FileInfo(
	const string name, const string path, 
	const time_t time_write, const time_t time_creat, 
	const unsigned attrib, _fsize_t size)
	:_name(name), _path(path), _time_write(time_write), _time_creat(time_creat), _attrib(attrib), _size(size)
{
}

FileInfo::~FileInfo()
{
}

string FileInfo::GetName()
{
	return this->_name;
}

string FileInfo::GetPath()
{
	return this->_path;
}

unsigned FileInfo::GetAttrib()
{
	return this->_attrib;
}

time_t FileInfo::GetTimeWrite()
{
	return _time_write;
}

time_t FileInfo::GetTimeCreat()
{
	return _time_creat;
}

_fsize_t FileInfo::GetSize()
{
	return this->_size;
}
