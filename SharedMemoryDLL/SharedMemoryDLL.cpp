/* Microsoft's page on Creating Named Shared Memory
*	https://msdn.microsoft.com/en-us/library/windows/desktop/aa366551%28v=vs.85%29.aspx
*/

#include "SharedMemoryDLL.h"
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <tchar.h>
#include <stdio.h>

using namespace std;

#pragma comment(lib, "user32.lib")

TCHAR imageName[] = TEXT("Local\\Image");
TCHAR dimName[] = TEXT("Local\\Dimensions");

int* pixBuf;


int* GetPtrToSharedMemory(char* objName, int width, int height) {
	void* ptrToSharedMem = NULL;	//the pointer to the shared memory space
	HANDLE hMapFile;
	int imageBufSize = 4 * width * height + 4;


	//CreateFileMapping
	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		imageBufSize,			 // memory space size
		objName);                 // name of mapping object

	if (hMapFile == NULL)
		return 0;


	//MapViewOfFile
	ptrToSharedMem = MapViewOfFile(
		hMapFile,				// handle to map object
		FILE_MAP_ALL_ACCESS,	// read/write permission
		0,
		0,
		imageBufSize);

	if (ptrToSharedMem == NULL) {
		CloseHandle(hMapFile);
		return 0;
	}

	//Add our dimensions to shared memory space
	char* ptrTmp = (char*)ptrToSharedMem;
	char* p = ptrTmp + 4;

	return (int*)(p); 
}



MYSHARED_API void* ReadSharedMemorySpace(int& width, int& height) {
	void* ptrToDim = OpenFileMappingToSharedMemory(dimName, 2, 1);
	int* dim = (int*)ptrToDim;
	dim += 1;
	width = *dim;
	dim += 1;
	height = *dim; 
	dim = NULL;
	UnmapPointerToSharedMemory((int*)ptrToDim);
	void* ptrToImage = OpenFileMappingToSharedMemory(imageName, width, height);
	return ptrToImage;
}



MYSHARED_API void UnmapPointerToSharedMemory(int* ptrToSharedMem) {
	UnmapViewOfFile(ptrToSharedMem);
}


MYSHARED_API int* CreateImagePointerToSharedMemory(int width, int height) {
	return GetPtrToSharedMemory(imageName, width, height);		//pass image shared memory pointer back to Unity
}


MYSHARED_API int* CreateDimensionPointerToSharedMemory(int width, int height) {
	return GetPtrToSharedMemory(dimName, width, height);		//pass dimension shared memory pointer back to Unity
}




void* OpenFileMappingToSharedMemory(char* objName, int width, int height) {
	int imageBufSize = 4 * width * height + 12;
	HANDLE hMapFile;
	void* ptrToSharedMem = NULL;

	//OpenFileMapping
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,	// read/write access
		FALSE,					// do not inherit the name
		objName					// name of mapping object
	);

	if (hMapFile == NULL) {
		cout << "Could not open file mapping object!" << endl;
		return 0;
	}

	//MapViewOfFile
	ptrToSharedMem = MapViewOfFile(
		hMapFile,				// handle to mapping object
		FILE_MAP_ALL_ACCESS,	// read/write access
		0,
		0,
		imageBufSize
	);

	if (ptrToSharedMem == NULL) {
		cout << "Could not map view of file!" << endl;
		return 0;
	}


	return ptrToSharedMem;
}


SharedMemoryDLL::SharedMemoryDLL()
{
}


SharedMemoryDLL::~SharedMemoryDLL()
{
}
