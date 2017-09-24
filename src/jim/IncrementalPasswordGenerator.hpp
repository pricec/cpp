#include <limits>
#include <vector>
#include "IPasswordGenerator.hpp"

namespace pj
{

// [0-9a-zA-z]
    static const std::vector<unsigned int> DEFAULT_CHARSET = {47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122};
static const size_t DEFAULT_MAXLENGTH = std::numeric_limits<size_t>::max() - 1;

class IncrementalPasswordGenerator : public IPasswordGenerator
{
public:
    IncrementalPasswordGenerator();
    IncrementalPasswordGenerator(size_t maxLength);
    IncrementalPasswordGenerator(std::vector<unsigned int> charset);
    IncrementalPasswordGenerator(
        size_t maxLength, std::vector<unsigned int> charset);
    ~IncrementalPasswordGenerator();

    bool hasNext();
    std::string next();

private:
    void increaseLength();
    bool isLast();

private:
    size_t                     m_maxLength;
    std::vector<unsigned int>  m_charset;
    size_t                     m_curLength;
    char                      *m_curPass;
};

} // namespace pj
