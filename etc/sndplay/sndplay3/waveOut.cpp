
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

//  WAVEOUT �f�o�C�X�������郁�b�Z�[�W�ւ̃n���h��
//  ���̃n���h�����ł͎g�p�ł���֐���������B
// WAVE�̍Đ��󋵂��󂯎��R�[���o�b�N�֐�

void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	//���̃R�[���o�b�N�֐��͂��܂�ɂ��P���Ȃ̂ŃA�v���ɂ��킹������
	//�� ����K�v������B

	if (uMsg == WOM_DONE) {
		//PostMessage(WavOut.Handle, WM_END_WAVEOUT , 0, dwParam1);
	}
}
/*
procedure TWavOut.WMEndWaveout(var Msg: TMessage);
begin
   CloseWaveOut;
   ShowMessage('�Đ��I��');
end;
*/

//�Đ�����
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

	// WAVEHDR�o�b�t�@�̊��蓖��
	FpWavEhdr = (PWAVEHDR)malloc(sizeof(WAVEHDR));

	FpWavEhdr->lpData = FpData;
	FpWavEhdr->dwBufferLength = FDataSize;
	FpWavEhdr->dwFlags = 0;
	FpWavEhdr->dwLoops = 0;

	waveOutPrepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutWrite(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutUnprepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));

	//WAVE�f�o�C�X�����
	//   CloseWaveOut;
}

//WAVE�f�o�C�X�����
void TWavOut::CloseWaveOut()
{
	waveOutReset(FhWavOut);
	waveOutUnprepareHeader(FhWavOut, FpWavEhdr, sizeof(WAVEHDR));
	waveOutClose(FhWavOut);
}

/*
void TWavOut::HeapFreeProc()
{
	//�������̊J��
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
   HeapFreeProc; //�������̊J��
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

	// �t�@�C���擪�փV�[�N
	mmioSeek(Fhmmio, 0, SEEK_SET);
	FmmckinfoParent.fccType = mmioStringToFOURCC("WAVE", 0);
	mmres = mmioDescend(Fhmmio, &FmmckinfoParent, NULL, MMIO_FINDRIFF);
	if (mmres != MMSYSERR_NOERROR)
	{
		//MessageDlg('ERROR! "not WAVE format" ', mtError, [mbOK], 0);
		return false;
	}

	// fmt�`�����N
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
	// data�`�����N
	FmmckinfoSubchunk.ckid = mmioStringToFOURCC("data", 0);
	mmres = mmioDescend(Fhmmio, &FmmckinfoSubchunk, &FmmckinfoParent,MMIO_FINDCHUNK);
	if (mmres != MMSYSERR_NOERROR)
	{
		//MessageDlg('ERROR "not data format" ', mtError, [mbOK], 0);
		free(FpSubchunk);
		return false;
	}

	// data�`�����N�p�̃o�b�t�@�̊��蓖��
	FDataSize = FmmckinfoSubchunk.cksize;
	FpData = (char*)malloc(FDataSize);
	if (mmioRead(Fhmmio, FpData, FDataSize) != FDataSize)
	{
		//MessageDlg('ERROR! "Reading data chunk" ', mtError, [mbOK], 0);
		free(FpSubchunk);
		free(FpData);
		return false;
	}

	// WAV�t�@�C���̃N���[�Y
	mmioClose(Fhmmio, 0);
}

/*
procedure TWavOut.Button2Click(Sender: TObject);
begin
   FileWaveOut;
end;
*/