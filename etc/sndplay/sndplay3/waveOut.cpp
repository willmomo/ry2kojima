
#include <windows.h>
#include <mmsystem.h>
#include "waveout.h"

#pragma comment(lib, "winmm.lib")

/*
var
  WavOut: TWavOut;

implementation

{$R *.DFM}
*/

//  WAVEOUT デバイスが投げるメッセージへのハンドラ
//  このハンドラ内では使用できる関数が限られる。
// WAVEの再生状況を受け取るコールバック関数

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	//このコールバック関数はあまりにも単純なのでアプリにあわせた改良
	//を する必要がある。

	if (uMsg == WOM_DONE) {
		//PostMessage(WavOut.Handle, WM_END_WAVEOUT , 0, dwParam1);
	}
}
/*
procedure TWavOut.WMEndWaveout(var Msg: TMessage);
begin
   CloseWaveOut;
   ShowMessage('再生終了');
end;
*/

//再生する
void TWavOut::FileWaveOut()
{
	MMRESULT mmres;

	// mmres := waveOutOpen(@FhWavOut, WAVE_MAPPER, PWaveFormatEx(FpSubchunk),DWORD(Handle), 0, CALLBACK_WINDOW);
	mmres = waveOutOpen(&FhWavOut, WAVE_MAPPER, (LPWAVEFORMATEX)FpSubchunk,   DWORD(waveOutProc), 0, CALLBACK_FUNCTION);
	if (mmres != MMSYSERR_NOERROR)
	{
		free(FpSubchunk);
		free(FpData);
		///MessageDlg('ERROR "waveOutOpen" ', mtError, [mbOK], 0);
		return;
	}

	// WAVEHDRバッファの割り当て
	FpWavEhdr = (PWAVEHDR)malloc(sizeof(WAVEHDR));

	FpWavEhdr->lpData = FpData;
	FpWavEhdr->dwBufferLength = FDataSize;
	FpWavEhdr->dwFlags = 0;
	FpWavEhdr->dwLoops = 0;

	waveOutPrepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutWrite(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutUnprepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));

	//WAVEデバイスを閉じる
	//   CloseWaveOut;
}

//WAVEデバイスを閉じる
void TWavOut::CloseWaveOut()
{
	waveOutReset(FhWavOut);
	waveOutUnprepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutClose(FhWavOut);
}

/*
void TWavOut::HeapFreeProc()
{
	//メモリの開放
	if FpSubchunk <> nil then
	{
//   ShowMessage('FpSubchunk HeapFree');
		HeapFree(GetProcessHeap, 0, FpSubchunk);
	}
	if FpData <> nil then
	{
//   ShowMessage('FpData HeapFree');
		HeapFree(GetProcessHeap, 0, FpData);
	}

	if FpWavEhdr <> nil then
	{
 //  ShowMessage('FpWavEhdr HeapFree');
		HeapFree(GetProcessHeap, 0, FpWavEhdr);
	}
}
*/

/*
procedure TWavOut.Button1Click(Sender: TObject);
begin
   if OpenDialog1.Execute then
      if LoadFromFile(OpenDialog1.FileName) then
         FileWaveOut;
end;

procedure TWavOut.FormCloseQuery(Sender: TObject; var CanClose:
Boolean);
begin
   HeapFreeProc; //メモリの開放
end;
*/

bool TWavOut::LoadFromFile(char* FileName)
{
	MMRESULT mmres;

	Fhmmio = mmioOpen(FileName, NULL, MMIO_READ);
	if (Fhmmio == 0)
	{
		//MessageDlg('ERROR "not WAVEFile Open"',mtError, [mbOK], 0);
		return false;
	}

	// ファイル先頭へシーク
	mmioSeek(Fhmmio, 0, SEEK_SET);
	FmmckinfoParent.fccType = mmioStringToFOURCC("WAVE", 0);
	mmres = mmioDescend(Fhmmio, &FmmckinfoParent, NULL, MMIO_FINDRIFF);
	if (mmres != MMSYSERR_NOERROR)
	{
		//MessageDlg('ERROR! "not WAVE format" ', mtError, [mbOK], 0);
		return false;
	}

	// fmtチャンク
	FmmckinfoSubchunk.ckid = mmioStringToFOURCC("fmt ", 0);
	mmres = mmioDescend(Fhmmio, &FmmckinfoSubchunk, &FmmckinfoParent,MMIO_FINDCHUNK);
	if (mmres != MMSYSERR_NOERROR)
	{
		//MessageDlg('ERROR! "not fmt format" ', mtError, [mbOK], 0);
		return false;
	}

	FSubchunkSize = FmmckinfoSubchunk.cksize;
	FpSubchunk = (char*)malloc(FSubchunkSize);
	if (mmioRead(Fhmmio, FpSubchunk, FSubchunkSize) != FSubchunkSize)
	{
		//MessageDlg('ERROR! "Reading fmt chunk" ', mtError, [mbOK], 0);
		free(FpSubchunk);
		return false;
	}

	mmioAscend(Fhmmio, &FmmckinfoSubchunk, 0);
	// dataチャンク
	FmmckinfoSubchunk.ckid = mmioStringToFOURCC("data", 0);
	mmres = mmioDescend(Fhmmio, &FmmckinfoSubchunk, &FmmckinfoParent,MMIO_FINDCHUNK);
	if (mmres != MMSYSERR_NOERROR)
	{
		//MessageDlg('ERROR "not data format" ', mtError, [mbOK], 0);
		free(FpSubchunk);
		return false;
	}

	// dataチャンク用のバッファの割り当て
	FDataSize = FmmckinfoSubchunk.cksize;
	FpData = (char*)malloc(FDataSize);
	if (mmioRead(Fhmmio, FpData, FDataSize) != FDataSize)
	{
		//MessageDlg('ERROR! "Reading data chunk" ', mtError, [mbOK], 0);
		free(FpSubchunk);
		free(FpData);
		return false;
	}

	// WAVファイルのクローズ
	mmioClose(Fhmmio, 0);
}

/*
procedure TWavOut.Button2Click(Sender: TObject);
begin
   FileWaveOut;
end;
*/