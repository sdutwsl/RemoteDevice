#pragma once
#include <audioclient.h>
#include <mmdeviceapi.h>
#include <stdio.h>

#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }
#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

extern BOOL bAudioDone;
class MyAudioSink;
HRESULT RecordAudioStream(MyAudioSink* pMySink);
DWORD WINAPI PCMThread(LPVOID pAddress);