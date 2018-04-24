#pragma once

#define MYSHARED_API __declspec(dllexport)


extern "C" {
	MYSHARED_API int* CreateImagePointerToSharedMemory(int width, int height);
	MYSHARED_API int* CreateDimensionPointerToSharedMemory(int width, int height);
	MYSHARED_API void UnmapPointerToSharedMemory(int* ptrToSharedMem);
	MYSHARED_API void* ReadSharedMemorySpace(int& width, int& height);
}

int* GetPtrToSharedMemory(char* objName, int width, int height);
void* OpenFileMappingToSharedMemory(char* objName, int width, int height);

class SharedMemoryDLL
{
public:
	SharedMemoryDLL();
	~SharedMemoryDLL();
};

