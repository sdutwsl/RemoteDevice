#pragma once

#include <winsock2.h>
#include<winsock.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

DWORD WINAPI BroadPCThread(LPVOID param);