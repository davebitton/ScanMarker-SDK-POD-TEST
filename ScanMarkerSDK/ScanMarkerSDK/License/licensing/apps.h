/* apps/apps.h */


#ifndef HEADER_APPS_H
# define HEADER_APPS_H

//# include "e_os.h"

# include <openssl/bio.h>
# include <openssl/x509.h>
# include <openssl/lhash.h>
# include <openssl/conf.h>
# include <openssl/txt_db.h>
# ifndef OPENSSL_NO_ENGINE
#  include <openssl/engine.h>
# endif

CONF *config = NULL;
BIO *bio_err = NULL;


#  ifndef OPENSSL_NO_ENGINE
#   define apps_startup() \
                        do { ERR_load_crypto_strings(); OpenSSL_add_all_algorithms(); \
                        ENGINE_load_builtin_engines();  } while(0)
#   define apps_shutdown() \
                        do { CONF_modules_unload(1);  \
                        OBJ_cleanup(); EVP_cleanup(); ENGINE_cleanup(); \
                        CRYPTO_cleanup_all_ex_data(); ERR_remove_thread_state(NULL); \
                        RAND_cleanup(); \
                        ERR_free_strings(); } while(0)
#  else
#   define apps_startup() \
                        do { ERR_load_crypto_strings(); OpenSSL_add_all_algorithms(); \
                         } while(0)
#   define apps_shutdown() \
                        do { CONF_modules_unload(1);  \
                        OBJ_cleanup(); EVP_cleanup(); \
                        CRYPTO_cleanup_all_ex_data(); ERR_remove_thread_state(NULL); \
                        RAND_cleanup(); \
                        ERR_free_strings(); } while(0)
#  endif


# if defined(OPENSSL_SYSNAME_WIN32) || defined(OPENSSL_SYSNAME_WINCE)
#  define openssl_fdset(a,b) FD_SET((unsigned int)a, b)
# else
#  define openssl_fdset(a,b) FD_SET(a, b)
# endif

typedef struct args_st {
    char **data;
    int count;
} ARGS;

# define PW_MIN_LENGTH 4
typedef struct pw_cb_data {
    const void *password;
    const char *prompt_info;
} PW_CB_DATA;

int password_callback(char *buf, int bufsiz, int verify, PW_CB_DATA *cb_data);



X509 *load_cert(BIO *err, const char *file, int format,
                const char *pass, ENGINE *e, const char *cert_descrip);

STACK_OF(X509) *load_certs(BIO *err, const char *file, int format,
                           const char *pass, ENGINE *e,
                           const char *cert_descrip);

X509_STORE *setup_verify(BIO *bp, char *CAfile, char *CApath);

ENGINE *setup_engine(BIO *err, const char *engine, int debug);
void release_engine(ENGINE *e);

int load_config(BIO *err, CONF *cnf);

#define FORMAT_PEM      3

# define OPENSSL_NO_SSL_INTERN

#endif
