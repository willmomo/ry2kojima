// mute.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "mute.h"

#include <ObjBase.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	IAudioEndpointVolume *endpointVolume = NULL;
	hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL; 

	// -------------------------

	BOOL bMute = FALSE;
	endpointVolume->GetMute(&bMute);
	endpointVolume->SetMute(bMute ? FALSE : TRUE, NULL);
	endpointVolume->Release();

	return 0;
}
