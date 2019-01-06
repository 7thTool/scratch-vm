#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <bthdef.h>
#include <bthledef.h>
#include <Bluetoothleapis.h>
#pragma comment(lib, "SetupAPI")
#pragma comment(lib, "BluetoothApis.lib")
 
#define TO_SEARCH_DEVICE_UUID L"{0000180d-0000-1000-8000-00805f9b34fb}" 
 
HANDLE GetBLEHandle(__in GUID svcGuid)
{
	HDEVINFO hDevInfo;
	SP_DEVICE_INTERFACE_DATA devIntfData;
	SP_DEVINFO_DATA devInfoData;
	GUID deviceGUID = svcGuid;
	HANDLE hFind = NULL;
 
	hDevInfo = SetupDiGetClassDevs(&deviceGUID, NULL, NULL, DIGCF_DEVICEINTERFACE | DIGCF_PRESENT);
 
	if (hDevInfo == INVALID_HANDLE_VALUE) return NULL;
 
	devIntfData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
 
	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &deviceGUID, i, &devIntfData); i++)
	{
		SP_DEVICE_INTERFACE_DETAIL_DATA DeviceInterfaceDetailData;
 
		DeviceInterfaceDetailData.cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
 
		DWORD size = 0;
 
		if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devIntfData, NULL, 0, &size, 0))
		{
			int err = GetLastError();
 
			if (err == ERROR_NO_MORE_ITEMS) break;
 
			PSP_DEVICE_INTERFACE_DETAIL_DATA pInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)GlobalAlloc(GPTR, size);
 
			pInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
 
			if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &devIntfData, pInterfaceDetailData, size, &size, &devInfoData))
				break;
 
			hFind = CreateFile(
				pInterfaceDetailData->DevicePath,
				GENERIC_WRITE | GENERIC_READ,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
 
			GlobalFree(pInterfaceDetailData);
		}
	}
 
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return hFind;
}
 
 
int main()
{
	GUID svcGuid;
	CLSIDFromString(TO_SEARCH_DEVICE_UUID, &svcGuid);
 
	HANDLE hLEDevice = GetBLEHandle(svcGuid);
 
	printf("Find Handle: 0x%x \r\n", hLEDevice);
 
	//-------------------------------------------
	USHORT serviceBufferCount;
 
	HRESULT hr = BluetoothGATTGetServices(
		hLEDevice,
		0,
		NULL,
		&serviceBufferCount,
		BLUETOOTH_GATT_FLAG_NONE);
 
	if (hr != HRESULT_FROM_WIN32(ERROR_MORE_DATA)) {
		printf("BluetoothGATTGetServices - Need Alloc Buffer Size: %d \r\n", serviceBufferCount);
	}
 
	PBTH_LE_GATT_SERVICE pServiceBuffer = (PBTH_LE_GATT_SERVICE)
		malloc(sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
 
	if (pServiceBuffer == NULL) {
		printf("pServiceBuffer out of memory\r\n");
	}
	else {
		RtlZeroMemory(pServiceBuffer,
			sizeof(BTH_LE_GATT_SERVICE) * serviceBufferCount);
	}
	
	USHORT numServices;
	hr = BluetoothGATTGetServices(
		hLEDevice,
		serviceBufferCount,
		pServiceBuffer,
		&numServices,
		BLUETOOTH_GATT_FLAG_NONE);
 
	if (hr == S_OK) {
		printf("BluetoothGATTGetServices - Service Count %d \r\n", numServices);
	}
	//-------------------------------------------
	USHORT charBufferSize;
	hr = BluetoothGATTGetCharacteristics(
		hLEDevice,
		pServiceBuffer,
		0,
		NULL,
		&charBufferSize,
		BLUETOOTH_GATT_FLAG_NONE);
	if (HRESULT_FROM_WIN32(ERROR_MORE_DATA) == hr) {
		printf("BluetoothGATTGetCharacteristics - Buffer Size %d \r\n", charBufferSize);
	}
 
	PBTH_LE_GATT_CHARACTERISTIC pCharBuffer;
	if (charBufferSize > 0) 
	{
		pCharBuffer = (PBTH_LE_GATT_CHARACTERISTIC)
			malloc(charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
 
		if (pCharBuffer == NULL) {
			printf("pCharBuffer out of memory\r\n");
		}
		else {
			RtlZeroMemory(pCharBuffer,
				charBufferSize * sizeof(BTH_LE_GATT_CHARACTERISTIC));
		}
 
		USHORT numChars;
		hr = BluetoothGATTGetCharacteristics(
			hLEDevice,
			pServiceBuffer,
			charBufferSize,
			pCharBuffer,
			&numChars,
			BLUETOOTH_GATT_FLAG_NONE);
 
		if (hr == S_OK) {
			printf("BluetoothGATTGetCharacteristics - Actual Data %d \r\n", numChars);
		}
 
		if (numChars != charBufferSize) {
			printf("buffer size and buffer size actual size mismatch\r\n");
		}
	}
	//-------------------------------------------
 
 
	//-------------------------------------------
 
	system("pause");
	return 0;
}
