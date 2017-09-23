#include <unistd.h>
#include <iostream>

namespace crypto
{

static const char *MD5 = "$1$";
static const char *SHA256 = "$5$";
static const char *SHA512 = "$6$";

// Compute the UNIX password hash
int ComputePasswordHash(
    const char        *type,
    const std::string &input,
    const std::string &salt,
    std::string       &hash
) {
    std::string command(type);
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
