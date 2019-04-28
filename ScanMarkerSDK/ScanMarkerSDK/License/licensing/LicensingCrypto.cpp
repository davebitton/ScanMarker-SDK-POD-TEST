#include <string>
#include "LicensingCrypto.h"
#include "LicenseSettings.h"

#ifdef __ANDROID__//Java

int checkSignature(const std::string &signature, const char *msg, int msgLen, const std::string &encodedCert)
{
    return LicenseSettings::instance()->checkSignature(signature, msg, msgLen, encodedCert);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#else//OpenSSL

#include "openssl/bio.h"
#include <openssl/err.h>
#include <openssl/pem.h>

#include "PlatformDefine.h"




#include "licensing.h"
#include "LicensingLog.h"
#include "LicensingMemory.h"

#include "b64.h"

#include "verify.h"


int checkSignature(const std::string &signature, const char *msg, int msgLen, const std::string &encodedCert)
{
	EVP_PKEY *pkey = NULL;
	BIO              *certbio = NULL;
	BIO               *outbio = NULL;
	X509                *cert = NULL;
	int ret = Success;

	/* ---------------------------------------------------------- *
	* These function calls initialize openssl for correct work.  *
	* ---------------------------------------------------------- */
	OpenSSL_add_all_algorithms();
	ERR_load_BIO_strings();
	ERR_load_crypto_strings();

	/* ---------------------------------------------------------- *
	* Create the Input/Output BIO's.                             *
	* ---------------------------------------------------------- */
	//certbio = BIO_new(BIO_s_file());
	outbio = BIO_new_fp(stdout, BIO_NOCLOSE);

	/* ---------------------------------------------------------- *
	* Load the certificate from file (PEM).                      *
	* ---------------------------------------------------------- */
	
	size_t outLen = 0;
	ByteSmartPtr certDataDecoded((char *)b64_decode_ex(encodedCert.c_str(), encodedCert.size(), &outLen), free);
	certbio = BIO_new_mem_buf((void*)certDataDecoded.get(), outLen);

	cert = PEM_read_bio_X509(certbio, NULL, 0, NULL);
	if (cert == 0)
	{
		log(Error, "failed to decode and load cert from the license");
		ret = CertificateMissing;
	}
#ifndef __ANDROID__
	if (!check_certificate(cert)) 
	{
		log(Error, "invalid certificate");
		ret = BadCertificate;
	}
#endif//__ANDROID__
	/* ---------------------------------------------------------- *
	* Extract the certificate's public key data.                 *
	* ---------------------------------------------------------- */
	if ((pkey = X509_get_pubkey(cert)) == NULL)
	{
		ret = PublicKeyMissing;
		log(Error, "Error getting public key from certificate");
		BIO_free_all(certbio);
		BIO_free_all(outbio);
		return ret;
	}

	EVP_MD_CTX *mdctx = NULL;

	int rvI;
	if (ret == Success && 0 != pkey && 0 != (mdctx = EVP_MD_CTX_create()))
	{
		/* Initialize `key` with a public key */
		//For now, switch to SHA1, because of the cert
		if (1 == EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256()/*EVP_sha1()*/, NULL, pkey))
		{
			if (1 == EVP_DigestVerifyUpdate(mdctx, msg, msgLen))
			{
				size_t outLen = 0;
				ByteSmartPtr signatureDecoded((char *)b64_decode_ex(signature.c_str(), signature.size(), &outLen), free);

				rvI = EVP_DigestVerifyFinal(mdctx, (const unsigned char *)signatureDecoded.get(), outLen);
				if (1 != rvI)
				{
					ret = SignatureMismatch;
				}
			}
			else
			{
				ret = SignatureMismatch;
			}
		}
	}
	if (0 != pkey)
		EVP_PKEY_free(pkey);
	if (0 != cert)
		X509_free(cert);

	BIO_free_all(certbio);
	BIO_free_all(outbio);
	EVP_MD_CTX_destroy(mdctx);
	return ret;
}


#endif//__ANDROID__