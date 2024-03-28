#include "pch.h"
#include "utils.h"

char* wcharToChar(const wchar_t* wstr) {
    int len = wcslen(wstr) + 1;
    if (len < 0) {
        // Error occurred
        return NULL;
    }

    char* str = LocalAlloc(LPTR, (len + 1) * sizeof(char));
    if (!str) {
        // Memory allocation failed
        return NULL;
    }
    size_t converted;
    wcstombs_s(&converted, str, len + 1, wstr, len + 1);
    return str;
}

void xorWithKey(BYTE* data, size_t dataLen, BYTE* key, size_t keyLen) {
    for (size_t i = 0; i < dataLen; i++) {
        data[i] ^= key[i % keyLen];
    }
    return;
}

char* generateKey(size_t keyLen) {
    BYTE* key = LocalAlloc(LPTR, keyLen + 1);
    DWORD val;

    if (!key) {
        return NULL;
    }

    char hex[] = "0123456789abcdef";
    for (size_t i = 0; i < keyLen; i++) {
        RtlGenRandom(&val, sizeof(val));
        key[i] = hex[val % 16];
    }
    key[keyLen] = '\0';
    return key;
}

char* charToHex(char* str, size_t sizestr) {
    size_t size = sizestr * 2 + 1;
    char* ret = LocalAlloc(LPTR, (size) * sizeof(char));
    if (!ret) {
        return NULL;
    }

    ZeroMemory(ret, size);

    for (size_t i = 0; i < sizestr; i++) {
        snprintf(ret + 2 * i, size - 2 * i, "%02x", str[i]);
    }
    return ret;
}
