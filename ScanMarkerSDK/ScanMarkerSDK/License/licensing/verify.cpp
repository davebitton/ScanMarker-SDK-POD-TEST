#ifdef _WIN32
#include <Winsock2.h>
#endif

#include "verify.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "apps.h"
#include <openssl/bio.h>
#include <openssl/err.h>

#include <openssl/x509v3.h>
#include <openssl/pem.h>

#include "RootCA.h"
//#include "RootCA_Obfuscated.h"
#include "LicensingLog.h"
#include "LicensingMemory.h"

#include <string>
#include <sstream>

static int load_certs_from_memory(BIO *err, unsigned char *buf, size_t length, int format,
	const char *desc, STACK_OF(X509) **pcerts);

static int callback(int ok, X509_STORE_CTX *ctx);
static int check(X509_STORE *ctx, X509 *cert,
                 STACK_OF(X509) *uchain, STACK_OF(X509) *tchain,
                 STACK_OF(X509_CRL) *crls, ENGINE *e);
static int v_verbose = 0, vflags = 0;

int logCallback (const char *str, size_t len, void *u)
{
    log(Error, "str");
    return 1;//go on printing
}

#ifndef OPENSSL_NO_ENGINE
/* Try to load an engine in a shareable library */
static ENGINE *try_load_engine(BIO *err, const char *engine, int debug)
{
    ENGINE *e = ENGINE_by_id("dynamic");
    if (e) {
        if (!ENGINE_ctrl_cmd_string(e, "SO_PATH", engine, 0)
            || !ENGINE_ctrl_cmd_string(e, "LOAD", NULL, 0)) {
            ENGINE_free(e);
            e = NULL;
        }
    }
    return e;
}
#endif

ENGINE *setup_engine(BIO *err, const char *engine, int debug)
{
    ENGINE *e = NULL;
    
#ifndef OPENSSL_NO_ENGINE
    if (engine) {
        if (strcmp(engine, "auto") == 0) {
            log(Debug, "enabling auto ENGINE support\n");
            ENGINE_register_all_complete();
            return NULL;
        }
        if ((e = ENGINE_by_id(engine)) == NULL
            && (e = try_load_engine(err, engine, debug)) == NULL)
        {
            log(Error, "invalid engine\n");
            ERR_print_errors_cb(logCallback, 0);
            return NULL;
        }
        if (debug) {
            ENGINE_ctrl(e, ENGINE_CTRL_SET_LOGSTREAM, 0, err, 0);
        }
        //ENGINE_ctrl_cmd(e, "SET_USER_INTERFACE", 0, ui_method, 0, 1);
        if (!ENGINE_set_default(e, ENGINE_METHOD_ALL)) {
            log(Error, "can't use that engine\n");
            ERR_print_errors_cb(logCallback, 0);
            ENGINE_free(e);
            return NULL;
        }
        
        log(Debug, "engine is set.\n");
    }
#endif
    return e;
}

void release_engine(ENGINE *e)
{
#ifndef OPENSSL_NO_ENGINE
    if (e != NULL)
    /* Free our "structural" reference. */
        ENGINE_free(e);
#endif
}

int password_callback(char *buf, int bufsiz, int verify, PW_CB_DATA *cb_tmp)
{
    return 0;
}

int check_certificate(X509 *cert)
{
	ENGINE *e = NULL;
	int i, ret = 0;

	STACK_OF(X509) *trusted = NULL;

	X509_STORE *cert_ctx = NULL;
	X509_LOOKUP *lookup = NULL;
	X509_VERIFY_PARAM *vpm = NULL;
		
	char *engine = NULL;
	do
	{
		cert_ctx = X509_STORE_new();
		if (cert_ctx == NULL)
		{
			log(Error, "failed to create a new store");
			break;
		}

		X509_STORE_set_verify_cb(cert_ctx, callback);

		ERR_load_crypto_strings();

		//apps_startup();

		if (bio_err == NULL)
			if ((bio_err = BIO_new(BIO_s_file())) != NULL)
				BIO_set_fp(bio_err, stderr, BIO_NOCLOSE | BIO_FP_TEXT);

		if (!load_config(bio_err, NULL))
		{
			log(Error, "failed to load config");
			break;
		}
		e = setup_engine(bio_err, engine, 0);

		if (vpm)
			X509_STORE_set1_param(cert_ctx, vpm);

		lookup = X509_STORE_add_lookup(cert_ctx, X509_LOOKUP_file());
		if (lookup == NULL)
		{
			log(Error, "X509_STORE_add_lookup failed");
			break;
		}
		
		X509_STORE_add_cert(cert_ctx, cert);

		lookup = X509_STORE_add_lookup(cert_ctx, X509_LOOKUP_hash_dir());
		if (lookup == NULL)
		{
			log(Error, "X509_STORE_add_lookup failed");
			break;
		}
		X509_LOOKUP_add_dir(lookup, NULL, X509_FILETYPE_DEFAULT);

		ERR_clear_error();

		if (!load_certs_from_memory(bio_err, (unsigned char *)ca, sizeof(ca), FORMAT_PEM, "trusted certificates", &trusted))
		{
			break;
		}

		ret = 0;

		ret = check(cert_ctx, cert, NULL, trusted, NULL, e);

	}while (0);
	
    if (vpm)
        X509_VERIFY_PARAM_free(vpm);

    if (cert_ctx != NULL)
        X509_STORE_free(cert_ctx);

    sk_X509_pop_free(trusted, X509_free);
    release_engine(e);
    //apps_shutdown();
	return ret;
}

static int check(X509_STORE *ctx, X509 *x,
                 STACK_OF(X509) *uchain, STACK_OF(X509) *tchain,
                 STACK_OF(X509_CRL) *crls, ENGINE *e)
{
    int i = 0, ret = 0;
    X509_STORE_CTX *csc;

	const EVP_MD *fprint_type = EVP_md5();
	unsigned int fprint_size = 0;
	unsigned char fprint[EVP_MAX_MD_SIZE];
	BIO *outbio = outbio = BIO_new_fp(stdout, BIO_NOCLOSE);

	if (!X509_digest(x, fprint_type, fprint, &fprint_size))
		BIO_printf(outbio, "Error creating the certificate fingerprint.\n");

	/* OpenSSL fingerprint-style: uppercase hex bytes with colon */
	for (int j = 0; j<fprint_size; j++) 
	{
	  BIO_printf(outbio,"%02X%c", fprint[j], (j+1 == fprint_size) ?'\n':':');
	}

	BIO_free_all(outbio);

    csc = X509_STORE_CTX_new();
    if (csc == NULL) {
        ERR_print_errors_cb(logCallback, 0);
        return 0;
    }
    X509_STORE_set_flags(ctx, vflags);
    if (!X509_STORE_CTX_init(csc, ctx, x, uchain)) {
        ERR_print_errors_cb(logCallback, 0);
        if (x != NULL)
            X509_free(x);
        
        return 0;
    }
    if (tchain)
        X509_STORE_CTX_trusted_stack(csc, tchain);
    if (crls)
        X509_STORE_CTX_set0_crls(csc, crls);
    i = X509_verify_cert(csc);
    X509_STORE_CTX_free(csc);

    ret = 0;

    if (i > 0)
    {
        log(Debug, "OK\n");
        ret = 1;
    }
    else
        ERR_print_errors_cb(logCallback, 0);

    return (ret);
}


static int callback(int ok, X509_STORE_CTX *ctx)
{
    int cert_error = X509_STORE_CTX_get_error(ctx);
    X509 *current_cert = X509_STORE_CTX_get_current_cert(ctx);

    if (!ok) {
        if (current_cert) {
            //X509_NAME_print_ex_fp(stdout,
            //                      X509_get_subject_name(current_cert),
            //                      0, XN_FLAG_ONELINE);
            //printf("\n");
            log(Error, "failed to get certificate from store");
        }
        
        std::stringstream logStream;
        
        logStream << (X509_STORE_CTX_get0_parent_ctx(ctx) ? "[CRL path]" : "")
        << " error " << cert_error << " at " << X509_STORE_CTX_get_error_depth(ctx)
        << " depth lookup:" << X509_verify_cert_error_string(cert_error);
        
        log(Error, logStream.str().c_str());
        
        switch (cert_error)
        {
            case X509_V_ERR_NO_EXPLICIT_POLICY:
                log(Error, "X509_V_ERR_NO_EXPLICIT_POLICY");
            case X509_V_ERR_CERT_HAS_EXPIRED:

            /*
             * since we are just checking the certificates, it is ok if they
             * are self signed. But we should still warn the user.
             */

            case X509_V_ERR_DEPTH_ZERO_SELF_SIGNED_CERT:
            /* Continue after extension errors too */
            case X509_V_ERR_INVALID_CA:
            case X509_V_ERR_INVALID_NON_CA:
            case X509_V_ERR_PATH_LENGTH_EXCEEDED:
            case X509_V_ERR_INVALID_PURPOSE:
            case X509_V_ERR_CRL_HAS_EXPIRED:
            case X509_V_ERR_CRL_NOT_YET_VALID:
            case X509_V_ERR_UNHANDLED_CRITICAL_EXTENSION:
                ok = 1;

        }

        return ok;

    }
    if (cert_error == X509_V_OK && ok == 2)
        log(Error, "Policies error");
    if (!v_verbose)
        ERR_clear_error();
    return (ok);
}

int load_certs_from_memory(BIO *err, unsigned char *buf, size_t length, int format,
	 const char *desc, STACK_OF(X509) **pcerts)
{
	int i;
	BIO *bio;
	STACK_OF(X509_INFO) *xis = NULL;
	X509_INFO *xi;
	int rv = 0;

	if (format != FORMAT_PEM) {
		log(Error, "load_certs_from_memory bad input format specified. Only PEM format is supported\n");
		return 0;
	}
	if (buf == NULL || length == 0) 
	{
        log(Error, "load_certs_from_memory(): Error got null or empty memory buffer \n");
		return 0;
	}
	bio = BIO_new_mem_buf((void*)buf, (int)length);
	
	if (bio == NULL)
    {
		log(Error, "load_certs_from_memory(): Error reading from memory\n");
		ERR_print_errors_cb(logCallback, 0);
		return 0;
	}

	xis = PEM_X509_INFO_read_bio(bio, NULL, NULL, NULL);

	BIO_free(bio);
    do
    {
        if (pcerts)
        {
            *pcerts = sk_X509_new_null();
            if (!*pcerts)
                break;
        }

        for (i = 0; i < sk_X509_INFO_num(xis); i++)
        {
            xi = sk_X509_INFO_value(xis, i);
            if (xi->x509 && pcerts)
            {
                if (!sk_X509_push(*pcerts, xi->x509))
                    break;
                xi->x509 = NULL;
            }
        }

        if (pcerts && sk_X509_num(*pcerts) > 0)
            rv = 1;
        
    }while(0);

	if (xis)
		sk_X509_INFO_pop_free(xis, X509_INFO_free);

	if (rv == 0) {
		if (pcerts) {
			sk_X509_pop_free(*pcerts, X509_free);
			*pcerts = NULL;
		}
		log(Error, "unable to load certificates\n");
		ERR_print_errors_cb(logCallback, 0);
	}
	return rv;
}

int load_config(BIO *err, CONF *cnf)
{
    static int load_config_called = 0;
    if (load_config_called)
        return 1;
    load_config_called = 1;
    if (!cnf)
        cnf = config;
    if (!cnf)
        return 1;
    
    OPENSSL_load_builtin_modules();
    
    if (CONF_modules_load(cnf, NULL, 0) <= 0) {
        log(Error, "Error configuring OpenSSL\n");
        ERR_print_errors_cb(logCallback, 0);
        return 0;
    }
    return 1;
}

X509 *load_cert(BIO *err, const char *file, int format,
                const char *pass, ENGINE *e, const char *cert_descrip)
{
    X509 *x = NULL;
    BIO *cert;
    
    if ((cert = BIO_new(BIO_s_file())) == NULL) {
        ERR_print_errors_cb(logCallback, 0);
        return 0;
    }
    
    if (file == NULL) {
#ifdef _IONBF
# ifndef OPENSSL_NO_SETVBUF_IONBF
        setvbuf(stdin, NULL, _IONBF, 0);
# endif                         /* ndef OPENSSL_NO_SETVBUF_IONBF */
#endif
        BIO_set_fp(cert, stdin, BIO_NOCLOSE);
    } else {
        if (BIO_read_filename(cert, file) <= 0)
        {
            log(Error, "Error opening cert file\n");
            ERR_print_errors_cb(logCallback, 0);
            if (cert != NULL)
                BIO_free(cert);
            return 0;
        }
    }
    

    if (format == FORMAT_PEM)
        x = PEM_read_bio_X509_AUX(cert, NULL,
                                  (pem_password_cb *)password_callback, NULL);
    else
    {
        log(Error, "bad input format specified for %s\n");
    }
    if (x == NULL) {
        log(Error, "unable to load certificate\n");
        ERR_print_errors_cb(logCallback, 0);
    }
    if (cert != NULL)
        BIO_free(cert);
    return (x);
}

