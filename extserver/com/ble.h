#pragma once

#ifndef _H_BLE_H_
#define _H_BLE_H_

#ifdef _WINDOWS
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <bthledef.h>
#include <Bluetoothleapis.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
//https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/bad452cb-4fc2-4a86-9b60-070b43577cc9/is-there-a-simple-example-desktop-programming-c-for-bluetooth-low-energy-devices?forum=wdk
#endif

class ble
{
  public:
	
const char *sservice_uuid = "0000ffe0-0000-1000-8000-00805f9b34fb";
const char *scharacteristic_uuid = "0000ffe1-0000-1000-8000-00805f9b34fb";

#ifdef _WINDOWS
	static void scan()
	{
		HDEVINFO hDevInfo = NULL;
		hDevInfo = SetupDiGetClassDevs(&GUID_BLUETOOTHLE_DEVICE_INTERFACE, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
		if (hDevInfo == INVALID_HANDLE_VALUE)
			return;

		SP_DEVINFO_LIST_DETAIL_DATA devListData = {0};
		devListData.cbSize = sizeof(SP_DEVINFO_LIST_DETAIL_DATA);
		if(!SetupDiGetDeviceInfoListDetail(hDevInfo, &devListData))
		{
			DWORD dwErr = GetLastError();
			if (dwErr == ERROR_INVALID_USER_BUFFER)
			{
				int a = 1;
				int b = a + 1;
					/*DWORD size = 0;
					PSP_DEVINFO_LIST_DETAIL_DATA pDevListData = (PSP_DEVINFO_LIST_DETAIL_DATA)GlobalAlloc(GPTR, size);
					pDevListData->cbSize = sizeof(SP_DEVINFO_LIST_DETAIL_DATA);
					if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devListData, pDevListData, size, &size, &devInfoData))
						break;
					PRINTFL("%s\n", pInterfaceDetailData->DevicePath);

					GlobalFree(pInterfaceDetailData);*/
			}
		}

		SP_DEVICE_INTERFACE_DATA devIntfData = {0};
		SP_DEVINFO_DATA devInfoData = {0};
		devIntfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
		devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_BLUETOOTHLE_DEVICE_INTERFACE, i, &devIntfData); i++)
		{
			SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData = {0};
			DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
			DWORD size = 0;
			if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devIntfData, NULL, 0, &size, 0))
			{
				int err = GetLastError();
				if (err == ERROR_NO_MORE_ITEMS)
					break;

				PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);
				pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
				if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devIntfData, pInterfaceDetailData, size, &size, &devInfoData))
					break;
				PRINTFL("%s\n", pInterfaceDetailData->DevicePath);
				/*HANDLE hLEDevice = CreateFile(
					pInterfaceDetailData->DevicePath,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					0,
					NULL);*/

				GlobalFree(pInterfaceDetailData);
			}
		}
		SetupDiDestroyDeviceInfoList(hDevInfo);
	}
#endif
};

#endif //_H_BLE_H_
