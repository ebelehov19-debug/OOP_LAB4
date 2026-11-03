#include "Bank_Optional.h"

#include <cmath>
#include <istream>
#include <ostream>

Bank_Optional::operator double()
{
    return metric();
}

bool Bank_Optinal :: operator == (condt Bank_Optional& other)
{
    constexpr double eps = 1e-6;
    return std::abs(metric() - other.metric()) < eps;
}

std::ofstream& operator<<(std::ofstream& out,const Bank_Optional& object)
{
    object.print(out);
    return out;
}

std::ifstream& operator>>(std::ifstream& in, const Banr_Optional& object)
{
    object.read(in);
    return in;
}