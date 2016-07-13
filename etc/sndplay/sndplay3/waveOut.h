
#pragma once


class TWavOut
{
public:
	// OpenDialog1: TOpenDialog;
	// Button1: TButton;
	// Button2: TButton;
	// procedure Button1Click(Sender: TObject);
	// procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
	// procedure Button2Click(Sender: TObject);
	
	HMMIO Fhmmio;
	DWORD FSubchunkSize;
	DWORD FDataSize;
	HWAVEOUT FhWavOut;

	MMCKINFO FmmckinfoParent;
	MMCKINFO FmmckinfoSubchunk;

	PWAVEHDR FpWavEhdr;
	char* FpSubchunk;
	char* FpData;
	// FWaveFmt: tWAVEFORMATEX;  //データ情報

	bool LoadFromFile(char* FileName);
	void FileWaveOut();
	void HeapFreeProc();
	void CloseWaveOut();
	// procedure WMEndWaveout(var Msg: TMessage); Message WM_END_WAVEOUT;
	// { Private 宣言 }

public:
	// { Public 宣言 }
};
