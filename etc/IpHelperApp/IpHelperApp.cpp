
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

	FIXED_INFO *pFixedInfo;
	ULONG      ulOutBufLen;
	DWORD      dwRetVal;

	pFixedInfo = (FIXED_INFO *)malloc(sizeof(FIXED_INFO));
	ulOutBufLen = sizeof(FIXED_INFO);

	if (GetNetworkParams(pFixedInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pFixedInfo);
		pFixedInfo = (FIXED_INFO *)malloc(ulOutBufLen);
	}

	if ((dwRetVal = GetNetworkParams(pFixedInfo, &ulOutBufLen)) != NO_ERROR) {
		printf("GetNetworkParams call failed with %d\n", dwRetVal);
	}

	printf("Host name: %s\n", pFixedInfo->HostName);
	printf("Domain name: %s\n", pFixedInfo->DomainName);

	printf("DNS Servers:\n");
	printf("\t%s\n", pFixedInfo->DnsServerList.IpAddress.String);

	PIP_ADDR_STRING pIPAddr = pFixedInfo->DnsServerList.Next;
	while (pIPAddr) {
		printf("\t%s\n", pIPAddr->IpAddress.String);
		pIPAddr = pIPAddr->Next;
	}

	return 0;
}
