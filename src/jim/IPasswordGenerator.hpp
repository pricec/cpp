#include <string>

class IPasswordGenerator
{
public:
    virtual bool hasNext() = 0;
    virtual std::string next() = 0;
};
