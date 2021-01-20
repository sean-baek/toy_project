#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/bio.h> /* basic input output stream */
#include <openssl/err.h>
#include <openssl/ssl.h>
 
#define BUFFSIZE 1024
 
void init_ssl() {
        SSL_load_error_strings();
        SSL_library_init();
}
 
void report_and_exit(const char* msg) {
        perror(msg);
        ERR_print_errors_fp(stderr);
        exit(-1);
}
 
void cleanup(SSL_CTX* ctx, BIO* bio) {
        SSL_CTX_free(ctx);
        BIO_free_all(bio);
}
 
void secure_connect(const char* host);
 
int main() {
        init_ssl();
 
        const char* hostname = "www.google.com";
        fprintf(stderr, "Trying an HTTPS connection to %s...\n", hostname);
        secure_connect(hostname);
 
        return 0;
}
 
 
void secure_connect(const char* host) {
        char name[BUFFSIZE];
        char request[BUFFSIZE];
        char response[BUFFSIZE];
 
 
        const SSL_METHOD* method = TLS_client_method();
        if ( NULL == method ) report_and_exit("TLS_client_method...");
 
        SSL_CTX* ctx = SSL_CTX_new(method);
        if ( NULL == ctx ) report_and_exit("SSL_CTX_new...");
 
        SSL_CTX_set_options(ctx, SSL_OP_NO_SSLv2 | SSL_OP_NO_COMPRESSION);
        SSL_CTX_set_ciphersuites(ctx, "TLS_AES_256_GCM_SHA384");
 
        BIO* bio = BIO_new_ssl_connect(ctx);
        if ( NULL == bio ) report_and_exit("BIO_new_ssl_connect...");
 
        SSL* ssl = NULL;
 
        /* 기본 입출력 채널, SSL 세션, 서버 endpoint 를 연결한다. */
        /* link bio chaanel, SSL Session, and Server endpoint */
 
        sprintf(name, "%s:%s", host, "https");
        BIO_get_ssl(bio, &ssl);
        SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
        BIO_set_conn_hostname(bio, name);
 
        if ( BIO_do_connect(bio) <= 0 ) {
                cleanup( ctx, bio );
                report_and_exit("BIO_do_connect...");
        }
 
        if ( !SSL_CTX_load_verify_locations(ctx,
                "/etc/ssl/certs/ca-certificates.crt", /* CA 인증키 */
                "/etc/ssl/certs/"))
        {
                report_and_exit("SSL_CTX_load_verify_locations...");
        }
 
        long verify_flag = SSL_get_verify_result(ssl);
        if ( verify_flag != X509_V_OK )
                fprintf( stderr, 
                                "#### Certificate verification error (%i) but continuing...\n",
                                (int) verify_flag);
 
        /* x43 0100 0011 ?c */
        sprintf( request, "GET / HTTP/1.1\x0D\x0AHost: %s\x0D\x0A\x43onnection: Close\x0D\x0A\x0D\x0A",
                        host);
        BIO_puts(bio, request);
 
        while (1) {
                memset(response, '\0', sizeof(response));
                int n = BIO_read(bio, response, BUFFSIZE);
                if ( n <= 0 ) break;
                puts(response);
        }
 
        cleanup( ctx, bio );
}