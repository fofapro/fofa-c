#ifndef FOFA_API_BASE64_H_
#define FOFA_API_BASE64_H_

char GetCharIndex(char c);

int Base64Encode(char *lpBuffer, const char *lpString, int sLen);

int Base64Decode(char *lpString, const char *lpSrc, int sLen);

#endif