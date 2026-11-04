#include "Bank_Optional.h"

#include <cmath>
#include <istream>
#include <ostream>

Bank_Optional::operator double() const
{
    return metric();
}

bool Bank_Optional::operator==(const Bank_Optional& other) const
{
    constexpr double eps = 1e-6;
    return std::abs(metric() - other.metric()) < eps;
}

std::ostream& operator<<(std::ostream& out, const Bank_Optional& object)
{
    object.print(out);
    return out;
}

std::istream& operator>>(std::istream& in, Bank_Optional& object)
{
    object.read(in);
    return in;
}
