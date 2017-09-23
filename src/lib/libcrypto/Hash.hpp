#include <openssl/evp.h>
#include <iostream>

namespace crypto
{

static const char *MD5 = "md5";
static const char *SHA256 = "sha256";
static const char *SHA512 = "sha512";

int ComputeHash(const char *type, const std::string& input, std::string& hash)
{
    EVP_MD_CTX *ctx;
    const EVP_MD *md;
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int dlen, i;

    OpenSSL_add_all_digests();

    md = EVP_get_digestbyname(type);
    if (!md)
    {
        return 10;
    }

    ctx = EVP_MD_CTX_create();
    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, input.c_str(), input.length());
    EVP_DigestFinal_ex(ctx, digest, &dlen);
    EVP_MD_CTX_destroy(ctx);

    hash.clear();
    char s[dlen*2];
    for (i = 0; i < dlen; ++i)
    {
        sprintf(&s[i*2], "%02x", digest[i]);
    }
    hash = std::string(s);

    EVP_cleanup();
    return 0;
}

} // namespace crypto
