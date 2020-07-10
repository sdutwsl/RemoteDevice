// CaptureTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//-----------------------------------------------------------
// Record an audio stream from the default audio capture
// device. The RecordAudioStream function allocates a shared
// buffer big enough to hold one second of PCM audio data.
// The function uses this buffer to stream data from the
// capture device. The main loop runs every 1/2 second.
//-----------------------------------------------------------

// REFERENCE_TIME time units per second and per millisecond
#include "PostPCMThread.h"



const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioCaptureClient = __uuidof(IAudioCaptureClient);


class MyAudioSink {
public:
    VOID (*SendData)(BYTE*pData,UINT iLength);
private:
    UINT iFramesize;
public:
    HRESULT SetFormat(WAVEFORMATEX* pwfx) {
        /*pwfx->wFormatTag = WAVE_FORMAT_PCM;
        pwfx->nChannels = 2;
        pwfx->nSamplesPerSec = 11025;
        pwfx->nAvgBytesPerSec = 22050;
        pwfx->nBlockAlign = 4;
        pwfx->wBitsPerSample = 16;
        pwfx->cbSize = 0;*/
        iFramesize = (pwfx->wBitsPerSample / 8) * pwfx->nChannels;
        return 0;
    }
    HRESULT CopyData(BYTE* pData, UINT32 numFramesAvailable, BOOL* pDone) {
        //if there is no sounds data,just return
        if (pData == NULL) {
            return 0;
        }
        //auto temp = fwrite(pData, iFramesize, numFramesAvailable, 0);
        SendData(pData, iFramesize * numFramesAvailable);
        //auto t=fwrite(pData,1, iFramesize * numFramesAvailable, f);
        //printf("写入PCM %d\n", t);
        *pDone = bAudioDone;
        return 0;
    }
};


HRESULT RecordAudioStream(MyAudioSink* pMySink)
{
    HRESULT hr;
    REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
    REFERENCE_TIME hnsActualDuration;
    UINT32 bufferFrameCount;
    UINT32 numFramesAvailable;
    IMMDeviceEnumerator* pEnumerator = NULL;
    IMMDevice* pDevice = NULL;
    IAudioClient* pAudioClient = NULL;
    IAudioCaptureClient* pCaptureClient = NULL;
    WAVEFORMATEX* pwfx = NULL;
    UINT32 packetLength = 0;
    BOOL bDone = FALSE;
    BYTE* pData;
    DWORD flags;

    hr = CoInitialize(0);
    EXIT_ON_ERROR(hr);

    hr = CoCreateInstance(
        CLSID_MMDeviceEnumerator, NULL,
        CLSCTX_ALL, IID_IMMDeviceEnumerator,
        (void**)&pEnumerator);
    EXIT_ON_ERROR(hr)

        hr = pEnumerator->GetDefaultAudioEndpoint(
            eRender, eConsole, &pDevice);
    EXIT_ON_ERROR(hr)

        hr = pDevice->Activate(
            IID_IAudioClient, CLSCTX_ALL,
            NULL, (void**)&pAudioClient);
    EXIT_ON_ERROR(hr)

        hr = pAudioClient->GetMixFormat(&pwfx);
    printf("采样率:%d\n", pwfx->nSamplesPerSec);
    printf("声道:%d\n", pwfx->nChannels);
    printf("码率:%d\n", pwfx->nAvgBytesPerSec);
    printf("对齐:%d\n", pwfx->nBlockAlign);
    printf("样本位数:%d\n", pwfx->wBitsPerSample);
    EXIT_ON_ERROR(hr)

        hr = pAudioClient->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_LOOPBACK,
            hnsRequestedDuration,
            0,
            pwfx,
            NULL);
    EXIT_ON_ERROR(hr)

        // Get the size of the allocated buffer.
        hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    EXIT_ON_ERROR(hr)

        hr = pAudioClient->GetService(
            IID_IAudioCaptureClient,
            (void**)&pCaptureClient);
    EXIT_ON_ERROR(hr)

        // Notify the audio sink which format to use.
        hr = pMySink->SetFormat(pwfx);
    EXIT_ON_ERROR(hr)

        // Calculate the actual duration of the allocated buffer.
        hnsActualDuration = (double)REFTIMES_PER_SEC *
        bufferFrameCount / pwfx->nSamplesPerSec;

    hr = pAudioClient->Start();  // Start recording.
    EXIT_ON_ERROR(hr)

        // Each loop fills about half of the shared buffer.
        while (bDone == FALSE)
        {
            // Sleep for half the buffer duration.
            //Sleep(hnsActualDuration / REFTIMES_PER_MILLISEC / 2);

            hr = pCaptureClient->GetNextPacketSize(&packetLength);
            EXIT_ON_ERROR(hr)

                while (packetLength != 0)
                {
                    // Get the available data in the shared buffer.
                    hr = pCaptureClient->GetBuffer(
                        &pData,
                        &numFramesAvailable,
                        &flags, NULL, NULL);
                    EXIT_ON_ERROR(hr)

                        if (flags & AUDCLNT_BUFFERFLAGS_SILENT)
                        {
                            pData = NULL;  // Tell CopyData to write silence.
                        }

                    // Copy the available capture data to the audio sink.
                    hr = pMySink->CopyData(
                        pData, numFramesAvailable, &bDone);
                    EXIT_ON_ERROR(hr)
                        hr = pCaptureClient->ReleaseBuffer(numFramesAvailable);
                    EXIT_ON_ERROR(hr)

                        hr = pCaptureClient->GetNextPacketSize(&packetLength);
                    EXIT_ON_ERROR(hr)
                }
        }

    hr = pAudioClient->Stop();  // Stop recording.
    EXIT_ON_ERROR(hr)

        Exit:
    CoTaskMemFree(pwfx);
    SAFE_RELEASE(pEnumerator)
        SAFE_RELEASE(pDevice)
        SAFE_RELEASE(pAudioClient)
        SAFE_RELEASE(pCaptureClient)
        return hr;
}
SOCKET sockPCM;
sockaddr_in addrPCM;
int time = 0;
VOID SendData(BYTE* pData, UINT iLength) {
    //int l=send(sockPCM,(char*)pData,iLength,0);
    int l = sendto(sockPCM, (char*)pData, iLength, 0, (sockaddr*)&addrPCM, sizeof(sockaddr));
    //printf("第%d次发送音频帧 长度%d 目标%s\n:", time++, l,inet_ntoa(addrPCM.sin_addr));
    //printf("Error %d\t", WSAGetLastError());
}

DWORD WINAPI PCMThread(LPVOID pAddress) {
    //防止由于某些意外启动多个线程
    CreateMutexA(0, true, "PCMThread");
    if (ERROR_ALREADY_EXISTS == GetLastError()) {
        return 0;
    }

    //sockPCM = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockPCM = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addrPCM.sin_family = AF_INET;
    addrPCM.sin_port = htons(11600);
    addrPCM.sin_addr.S_un.S_addr = inet_addr((char*)pAddress);
    //auto err=connect(sockPCM, (sockaddr*)&addrPCM, sizeof(addrPCM));
    //if (SOCKET_ERROR == err) {
    //    printf("connect error %d\n", WSAGetLastError());
    //    return 1;
    //}
    MyAudioSink ms;
    ms.SendData = SendData;
    RecordAudioStream(&ms);
    closesocket(sockPCM);
    return 0;
}