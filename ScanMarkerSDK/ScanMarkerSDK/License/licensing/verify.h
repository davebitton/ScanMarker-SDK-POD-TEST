#ifndef __VERIFY_H__
#define __VERIFY_H__
#include <string>
#include <openssl/x509.h>

int check_certificate(X509 *cert);

#endif
