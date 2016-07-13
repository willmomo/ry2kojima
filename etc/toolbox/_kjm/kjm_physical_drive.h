//---------------------------------------------------------------------
// kjm::physical_drive class
//---------------------------------------------------------------------

#pragma once


#include <windows.h>
#include <string>
#include "kjm_smart.h"


namespace kjm {

	//-----------------------------------------------------------------
	// 物理ドライブ(SMART)を処理するクラス
	//-----------------------------------------------------------------
	ATTRVALUEANDNAME attrValueAndNames[];
	#define	NUM_ATTRIBUTE_STRUCTS	30

	class physical_drive {
	private:
		HANDLE m_disk_handle;

	public:
		// アクセサ
		HANDLE getHandle() { return m_disk_handle; };
		int getTemperature();
		int getDrivePowerCycleCount();
		int getPowerOnHours();
		int getStartStopCount();

	public:
		physical_drive() : m_disk_handle( INVALID_HANDLE_VALUE ) {};
		virtual ~physical_drive() { CloseDrive(); };

		BOOL OpenDrive( LPCTSTR driveName );
		BOOL CloseDrive();

		bool isSupportedSMART();
		int identifyDevice(IDENTIFYDATA* identifyData);
		int smartDisable();
		int smartEnable();
		int smartEnableDisableAutoSave(char enable);
		int smartReturnStatus();
		int smartReadAttributeValues(SMARTATTRIBUTESDATA* attrData);
		int smartReadAttributeThresholds(SMARTHRESHOLDSDATA* thData);

		std::string toCSVString();
	};

}
