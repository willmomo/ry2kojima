#if 0
#include <windows.h>
#include <tchar.h>
#include <setupapi.h>
#include <iostream>
#include "smart.h"


#if defined(UNICODE)
#define _tcout	wcout
#else
#define _tcout	cout
#endif



int identifyDevice(HANDLE hFile, IDENTIFYDATA* identifyData) {

	SENDCMDINPARAMS 		scip;
	IDENTIFYCMDOUTPARAMS	idCmdOut;
	DWORD       			bytesReturned;
    BOOL					status;
	int						ret;

	/*------コマンド設定を行う---------------------------------------------*/
	memset(&idCmdOut, 0, sizeof(idCmdOut));
	memset(&scip, 0, sizeof(scip));
	scip.irDriveRegs.bCommandReg		= ID_CMD;
	
	/*------コマンド実行---------------------------------------------------*/
	status = ::DeviceIoControl(hFile,	SMART_RCV_DRIVE_DATA,
             				(LPVOID)&scip,	sizeof(SENDCMDINPARAMS) - 1,
               				(LPVOID)&idCmdOut,	sizeof(idCmdOut),
               				&bytesReturned, NULL);
	if(FALSE != status) {
		memcpy(identifyData, &idCmdOut.identifyData, sizeof(IDENTIFYDATA));
		ret = 0;
	} else {
		memset(identifyData, 0, sizeof(IDENTIFYDATA));
		ret = -1;
	}

	return ret;
}

void listDevInfo() {

	const GUID* pGuid = &DiskClassGuid;

	HDEVINFO hDevInfo;

	hDevInfo = SetupDiGetClassDevs(
		pGuid,
		NULL,
		NULL,
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if (hDevInfo == INVALID_HANDLE_VALUE) {
		std::_tcout << _T("SetupDiGetClassDevs error: ") << GetLastError() << std::endl;
		return;
	}

	SP_DEVICE_INTERFACE_DATA deviceInterfaceData = {sizeof(deviceInterfaceData)};

	SP_DEVINFO_DATA devInfoData = {sizeof(devInfoData)};
	for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++) {

		DWORD dwRegType;
		DWORD dwUiNum;
		DWORD dwSize = 0;

		DWORD dwProperty = SPDRP_UI_NUMBER;
		BOOL ret = SetupDiGetDeviceRegistryProperty(
			hDevInfo,
			&devInfoData,
			dwProperty,
			&dwRegType,
			NULL,
			0,
			&dwSize);

		if (dwRegType == REG_DWORD) {
			ret = SetupDiGetDeviceRegistryProperty(
				hDevInfo,
				&devInfoData,
				dwProperty,
				&dwRegType,
				(LPBYTE)&dwUiNum,
				dwSize,
				&dwSize);

			{
				TCHAR devName[MAX_PATH];
				wsprintf(devName, _T("\\\\.\\PhysicalDrive%d"), dwUiNum);

				std::_tcout << _T("'") << devName << _T("' を開く\n");

				HANDLE hFile = CreateFile(devName,
										GENERIC_READ | GENERIC_WRITE,
										FILE_SHARE_READ | FILE_SHARE_WRITE,
										NULL, OPEN_EXISTING, 0, NULL);

				if (hFile != INVALID_HANDLE_VALUE) {

					std::_tcout << _T("オープン完了\n");

					IDENTIFYDATA data;
					identifyDevice(hFile, &data);

					std::_tcout << _T("シリアル: ") << data.serialNumber << _T("\n");
					std::_tcout << _T("model   : ") << data.modelNumber << _T("\n");
					CloseHandle(hFile);
				}
			}
		} else {
			std::_tcout << _T("表示できないREGタイプ: ") << dwRegType << _T("\n");
		}
	}


	for (DWORD dwIndex = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, pGuid, dwIndex, &deviceInterfaceData) != FALSE; dwIndex++) {

		BOOL ret;
		DWORD requiredSize = 0;

		ret = SetupDiGetDeviceInterfaceDetail(
			hDevInfo,
			&deviceInterfaceData,
			NULL,
			0,
			&requiredSize,
			NULL);

		std::_tcout << _T("1 ret = ") << ret << _T(" (") << GetLastError() << _T(")") << std::endl;

		PSP_DEVICE_INTERFACE_DETAIL_DATA pDeviceInterfaceDetailData = (SP_DEVICE_INTERFACE_DETAIL_DATA*)LocalAlloc(LPTR, requiredSize);
		pDeviceInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

		SP_DEVINFO_DATA devinfoData = {sizeof(devinfoData)};

		ret = SetupDiGetDeviceInterfaceDetail(
			hDevInfo,
			&deviceInterfaceData,
			pDeviceInterfaceDetailData,
			requiredSize,
			&requiredSize,
			&devinfoData);

		std::_tcout << _T("2 ret = ") << ret << _T(" (") << GetLastError() << _T(")") << std::endl;

		std::_tcout << _T("device path: ") << pDeviceInterfaceDetailData->DevicePath << std::endl;

		{
			HANDLE hFile = CreateFile(pDeviceInterfaceDetailData->DevicePath,
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE,
									NULL, OPEN_EXISTING, 0, NULL);

			if (hFile != INVALID_HANDLE_VALUE) {

				std::_tcout << _T("オープン完了\n");

				CloseHandle(hFile);
			}
		}

		LocalFree(pDeviceInterfaceDetailData);
	}

	if (!SetupDiDestroyDeviceInfoList(hDevInfo)) {
		std::_tcout << _T("SetupDiDestroyDeviceInfoList error: ") << GetLastError() << std::endl;
	}
}


void smartinfo() {


	HANDLE hFile = ::CreateFile(_T("\\\\.\\PhysicalDrive0"),
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {

		// 温度
		printf("温度: %d\n", drive.getTemperature());

		if (drive.isSupportedSMART()) {

			int ret = drive.smartReturnStatus();
		}

		// 正常のときは、個別の閾値をチェックする。

		SMARTATTRIBUTESDATA	attrData;
		SMARTHRESHOLDSDATA thData;

		memset( &attrData, 0, sizeof( attrData ) );
		memset( &thData, 0, sizeof( thData ) );

		drive.smartReadAttributeValues( &attrData );
		drive.smartReadAttributeThresholds( &thData );

		for ( int index = 0; index < NUM_ATTRIBUTE_STRUCTS; ++index ) {
			int attrID = attrData.AttributeData[ index ].attributeID;
			if ( 0 != attrID ) {
				char*	attrName;
				int		arrayIndex = 0;
				while ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
					if ( attrID == kjm::attrValueAndNames[arrayIndex].value ) {
						break;
					}
					++arrayIndex;
				}
				
				if ( 0 != kjm::attrValueAndNames[arrayIndex].value ) {
					attrName = kjm::attrValueAndNames[arrayIndex].name;
				} else {
					attrName = "Unknown Attribute";
				}

				printf("%3d: %41s: %3d %3d %3d $%04X", 
					attrID,
					attrName, 
					attrData.AttributeData[index].attributeValue,
					attrData.AttributeData[index].worstValue,
					thData.ThresholdData[index].attributeThreshold,
					*(WORD*)&attrData.AttributeData[index].vendorSpecific[0]);

				// 現在値が閾値を下回っているときは、警告を出す。
				if ( ( WORD )attrData.AttributeData[ index ].attributeValue
					< ( WORD )thData.ThresholdData[index].attributeThreshold ) {
				}

				puts("");
			}
		}

		drive.CloseDrive();
	}
}


int _tmain(int argc, TCHAR** argv) {

	std::locale loc("japanese");
	std::_tcout.imbue(loc);

//	listDevInfo();

	smartinfo();

	return 0;
}
#endif


#include <iostream>
#include "smart.h"


#if defined(UNICODE)
#define _tcout	wcout
#else
#define _tcout	cout
#endif


int _tmain(int argc, TCHAR** argv) {
	std::locale loc("japanese");
	std::_tcout.imbue(loc);

	kjm::smart_info si;
	if (si.open_device(0)) {

		std::_tcout << _T("S.M.A.R.Tのサポート状態: ") << si.isSupportedSMART() << _T("\n");

		std::_tcout << _T("簡易診断: ") << si.return_smart_status() << _T("\n");
	}

	return 0;
}
