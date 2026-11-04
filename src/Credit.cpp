#include "Credit.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Credit::Credit():sum_(0.0),rate_(0.0),years_(1),annuity_(false)
{}

Credit::Credit(double sum, double rate, int years, bool annuity)
    : sum_(sum), rate_(rate), years_(years), annuity_(annuity)
{
    if (!std::isfinite(sum) || !std::isfinite(rate) ||
        sum < 0 || rate < 0 || years <= 0) {
        throw std::invalid_argument("Invalid credit parameters");
    }
}


double Credit::metric() const
{
    if (!annuity_) {
        return sum_ * rate_;
    }

    if (rate_ == 0.0) {
        return sum_ / years_;
    }

    const double r = rate_ / 12.0;
    const double n = years_ * 12.0;
    const double payment =
        sum_ * r / (1.0 - std::pow(1.0 + r, -n));

    return 12.0 * payment;
}

bool Credit::isAnnuity() const
{
    return annuity_;
}void Credit::read(std::istream& in)
{
    double sum;
    double rate;
    int years;
    int annuity;
    in >> std::ws;
    if (in.peek() == 'C')
    {
        const auto expect = [&in](const char* text) {
            in >> std::ws;
            for (; *text; ++text) {
                char actual;
                if (!in.get(actual) || actual != *text) {
                    in.setstate(std::ios::failbit);
                    return false;
                }
            }
            return true;
        };

        double printedMetric;
        if (!expect("Credit:") || !expect("sum=") || !(in >> sum) ||
            !expect("rate=") || !(in >> rate) ||
            !expect("years=") || !(in >> years) ||
            !expect("annuity=") || !(in >> annuity) ||
            !expect("metric=") || !(in >> printedMetric)) {
            return;
        }
    }
    else if (!(in >> sum >> rate >> years >> annuity))
    {
        return;
    }

    if (annuity != 0 && annuity != 1) 
    {
        in.setstate(std::ios::failbit);
        return;
    }

    try 
    {
        Credit temp(sum, rate, years, annuity == 1);
        *this = temp;
    }
    catch (const std::invalid_argument&) 
    {
        in.setstate(std::ios::failbit);
    }
}

void Credit::print(std::ostream& out) const
{
    out << "Credit: sum=" << sum_
        << " rate=" << rate_
        << " years=" << years_
        << " annuity=" << (annuity_ ? 1 : 0)
        << " metric=" << metric();
}



    

