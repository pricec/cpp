#include <unistd.h>
#include <iostream>

namespace crypto
{

enum HashFunction
{
    MD5 = 0,
    SHA256,
    SHA512,
};

static const char *MD5_STR    = "$1$";
static const char *SHA256_STR = "$5$";
static const char *SHA512_STR = "$6$";
static const char *NOFUNC_STR = "NONE";

const char* HashFunctionToCStr(const HashFunction hashfn)
{
    switch (hashfn)
    {
    case MD5:
        return MD5_STR;
    case SHA256:
        return SHA256_STR;
    case SHA512:
        return SHA512_STR;
    default:
        return NOFUNC_STR;
    }
}

// Compute the UNIX password hash
int ComputePasswordHash(
    const HashFunction type,
    const std::string  &input,
    const std::string  &salt,
    std::string        &hash
) {
    std::string command(HashFunctionToCStr(type));
    command += salt;
    char *result = crypt(input.c_str(), command.c_str());
    if (result == NULL)
    {
        return 10;
    }
    hash = std::string(result);
    return 0;
}

} // namespace crypto
