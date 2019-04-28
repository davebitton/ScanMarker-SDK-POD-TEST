#ifndef __LICENSING_CRYPTO_H__
#define __LICENSING_CRYPTO_H__

#include <string>

int checkSignature(const std::string &signatureEncoded, const char *msg, int msgLen, const std::string &encodedCert);

#endif//__LICENSING_CRYPTO_H__
