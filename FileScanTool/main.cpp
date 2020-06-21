#include <iostream>
//#include <io.h>
#include <string>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <climits>
#include <sys/timeb.h>
#include "FileInfo.h"


#ifdef _WIN32
#include <winsock2.h>
#include <time.h>
#else
#include <sys/time.h>
#endif

using namespace std;

#define MAXPATHLEN 4096

void ScanCatalogue(const string& path, vector<_finddata_t>& fileInfos)
{
	//vector<_finddata_t>* files = new vector<_finddata_t>;
	long handleFile = 0;
	_finddata_t fileInfo;
	string pathName;

	if ((handleFile = _findfirst(pathName.assign(path).append("/*").c_str(), &fileInfo)) == -1L) {
		return;
	}
	do {
		if (strcmp(fileInfo.name, ".") ^ strcmp(fileInfo.name, "..")) {
			continue;
		}
		if ((fileInfo.attrib & _A_SUBDIR)) {
			string subPath;
			ScanCatalogue(
				subPath.assign(path).append("/").append(fileInfo.name),
				fileInfos);
		}
		fileInfos.push_back(fileInfo);
		//files->push_back(fileInfo);
	} while (_findnext(handleFile, &fileInfo) == 0);

	_findclose(handleFile);
}

void printFileInfos(vector<_finddata_t>& fileInfos)
{
	//printf("Listing of .c files\n\n");
	printf("CLA  DATE %25c SIZE  FILENMAE\n", ' ');
	printf("---  ---- %25c ----  --------\n", ' ');
	for (vector<_finddata_t>::iterator it = fileInfos.begin(); it < fileInfos.end(); ++it) {
		char buffer[30];
		printf((it->attrib & _A_SUBDIR) ? " D  " : " F  ");
		ctime_s(buffer, sizeof(buffer), &it->time_write);
		printf(" %.24s %10ld  %-12s\n",
			buffer, it->size, it->name);
	}
}

void run()
{
	string path = "./TestDir";
	vector<_finddata_t>* fileInfos = new vector<_finddata_t>;
	ScanCatalogue("./TestDir", *fileInfos);
	printFileInfos(*fileInfos);

	delete fileInfos;
}

void test01()
{
	struct _finddata_t c_file;
	intptr_t hFile;

	// Find first .c file in current directory
	if ((hFile = _findfirst("./TestDir/*", &c_file)) == -1L)
		printf("No *.c files in current directory!\n");
	else
	{
		printf("Listing of .c files\n\n");
		printf("RDO HID SYS ARC DIR  FILE         DATE %25c SIZE\n", ' ');
		printf("--- --- --- --- ---  ----         ---- %25c ----\n", ' ');
		do {
			char buffer[30];
			printf((c_file.attrib & _A_RDONLY) ? " Y  " : " N  ");
			printf((c_file.attrib & _A_HIDDEN) ? " Y  " : " N  ");
			printf((c_file.attrib & _A_SYSTEM) ? " Y  " : " N  ");
			printf((c_file.attrib & _A_ARCH) ? " Y  " : " N  ");
			printf((c_file.attrib & _A_SUBDIR) ? " Y  " : " N  ");
			ctime_s(buffer, _countof(buffer), &c_file.time_write);
			printf(" %-12s %.24s  %9ld\n",
				c_file.name, buffer, c_file.size);
		} while (_findnext(hFile, &c_file) == 0);
		_findclose(hFile);
	}
}

void test02()
{
	char c[1024] = "hello";
	cout << strcmp(c, "hello") << endl;
	cout << strcmp(c, "hell") << endl;
}

void test03()
{
	char buffer[1024];
	string oldPath = "./TestDir/";
	//_fullpath(buffer, oldPath.c_str(), sizeof(buffer));
	time_t currentTime = time((time_t*)NULL);
	cout << "current time: " << currentTime << endl;
	char buffer_t[30] = { 0 };
	ctime_s(buffer_t, sizeof(buffer_t), &currentTime);
	//strcpy_s(buffer_t, sizeof(buffer_t), ctime(&currentTime));
	//strcpy(buffer_t, ctime(&currentTime));
	cout << buffer_t << endl;
}

int ScanCatalogue2(const string& path, vector<FileInfo>& fileInfos, unsigned long &fileCount, unsigned long &dirCount)
{
	long handleFile = 0;
	_finddata_t file;
	string pathName;

	if ((handleFile = _findfirst(pathName.assign(path).append("/*").c_str(), &file)) == -1L) {
		return -1;
	}
	do {
		if (strcmp(file.name, ".") ^ strcmp(file.name, "..")) {
			continue;
		}
		if ((file.attrib & _A_SUBDIR)) {
			++dirCount;
			string subPath;
			ScanCatalogue2(
				subPath.assign(path).append("/").append(file.name),
				fileInfos, fileCount, dirCount);
		}
		else {
			++fileCount;
		}
		char buffer[MAXPATHLEN] = { 0 };
		_fullpath(buffer, path.c_str(), sizeof(buffer));
		FileInfo fInfo(file.name, buffer, file.time_write, file.time_create, file.attrib, file.size);
		fileInfos.push_back(fInfo);
	} while (_findnext(handleFile, &file) == 0);

	_findclose(handleFile);
	return 0;
}

void PrintFileInfos2(vector<FileInfo>& fileInfos)
{
	//printf("Listing of .c files\n\n");
	printf("CLA  DATE %25c SIZE  FILENMAE %6c PATH\n", ' ', ' ');
	printf("---  ---- %25c ----  -------- %6c ------------\n", ' ', ' ');
	for (vector<FileInfo>::iterator it = fileInfos.begin(); it < fileInfos.end(); ++it) {
		char buffer[30];
		printf((it->GetAttrib() & _A_SUBDIR) ? " D  " : " F  ");
		time_t ttime = it->GetTimeWrite();
		ctime_s(buffer, sizeof(buffer), &ttime);
		printf(" %.24s %10ld  %-16s%s\n",
			buffer, it->GetSize(), (it->GetName()).c_str(), it->GetPath().c_str());
	}
}

void SeekFileDirByTimeOfWrite(vector<FileInfo>& fileInfos, int dayNum, vector<unsigned long> &seekFileIndex)
{
	//vector<unsigned long>* seekFileIndex = new vector<unsigned long>;
	//int secondDiffer = 60;
	int secondDiffer = dayNum * 24 * 60 * 60;
	time_t currentTime = time((time_t *)NULL);
	time_t timeDiffer = currentTime - (time_t)secondDiffer;
	for (unsigned long i = 0; i < fileInfos.size(); ++i) {
		if (fileInfos[i].GetTimeWrite() >= timeDiffer) {
			seekFileIndex.push_back(i);
		}
	}
}

void printFileInfoBySeek(vector<FileInfo>& fileInfos, vector<unsigned long>& seekFileIndex)
{
	printf("CLA  DATE %25c SIZE  FILENMAE %16c PATH\n", ' ', ' ');
	printf("---  ---- %25c ----  -------- %16c ------------\n", ' ', ' ');
	for (unsigned long i = 0L; i < seekFileIndex.size(); ++i) {
		char buffer[30];
		printf((fileInfos[seekFileIndex[i]].GetAttrib() & _A_SUBDIR) ? " D  " : " F  ");
		time_t ttime = fileInfos[seekFileIndex[i]].GetTimeWrite();
		ctime_s(buffer, sizeof(buffer), &ttime);
		printf(" %.24s %10ld  %-26s%s\n",
			buffer, fileInfos[seekFileIndex[i]].GetSize(), (fileInfos[seekFileIndex[i]].GetName()).c_str(), fileInfos[seekFileIndex[i]].GetPath().c_str());
	}
}

unsigned long long GetCurrentTimeMsec()
{
#ifdef _WIN32
	struct timeval tv;
	time_t clock;
	struct tm tm;
	SYSTEMTIME wtm;

	GetLocalTime(&wtm);
	tm.tm_year = wtm.wYear - 1900;
	tm.tm_mon = wtm.wMonth - 1;
	tm.tm_mday = wtm.wDay;
	tm.tm_hour = wtm.wHour;
	tm.tm_min = wtm.wMinute;
	tm.tm_sec = wtm.wSecond;
	tm.tm_isdst = -1;
	clock = mktime(&tm);
	tv.tv_sec = (long)clock;
	tv.tv_usec = wtm.wMilliseconds * 1000;
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return ((unsigned long long)tv.tv_sec * 1000 + (unsigned long long)tv.tv_usec / 1000);
#endif
}

void run2()
{
	//string path = "D:/";
	//string path = "E:/";
	//string path = "./TestDir/";
	string path;
	cout << "输入需要扫描的文件路径：";
	cin >> path;
	cout << "##########################开始扫描##########################" << endl;
	unsigned long fileCount = 0L;
	unsigned long dirCount = 0L;
	vector<FileInfo>* fileInfos = new vector<FileInfo>;
	// 扫描指定目录下的所有文件与目录
	time_t startTime = GetCurrentTimeMsec();
	if (-1 == ScanCatalogue2(path, *fileInfos, fileCount, dirCount)) {
		cout << "未找到该路径或该路径下为空！" << endl;
		return;
	}
	time_t endTime = GetCurrentTimeMsec();
	// 打印扫描出的文件与目录信息
	//PrintFileInfos2(*fileInfos);
	// 打印扫描到的文件数和目录树
	cout << "文件数：" << fileCount << endl;
	cout << "目录数：" << dirCount << endl;
	cout << "##########################扫描完成##########################" << endl;
	cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>用时：" << (endTime - startTime) << "ms<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << "输入天数，查找近几天内修改的文件与目录：";
	int dayNum;
	cin >> dayNum;
	// 挑选出指定时间内修改的文件和目录
	vector<unsigned long>* seekFileIndex = new vector<unsigned long>;
	SeekFileDirByTimeOfWrite(*fileInfos, dayNum, *seekFileIndex);
	// 打印出挑选出的文件与目录信息
	printFileInfoBySeek(*fileInfos, *seekFileIndex);

	delete seekFileIndex;
	delete fileInfos;
}

int main()
{
	//run();
	run2();
	//test03();

	//system("pause");
	return 0;
}