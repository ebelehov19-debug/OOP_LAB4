#include "Deposit.h"

#include <cmath>
#include <istream>
#include <ostream>
#include <stdexcept>

Bank_Optional* Deposit::clone() const
{
    return new Deposit(*this);
}

Deposit::Deposit()
    : sum_(0.0), rate_(0.0), years_(1), cumulative_(false)
{}

Deposit::Deposit(double sum, double rate, int years, bool cumulative)
    : sum_(sum), rate_(rate), years_(years), cumulative_(cumulative)
{
    if (!std::isfinite(sum) || !std::isfinite(rate) ||
        sum < 0 || rate < 0 || years <= 0) {
        throw std::invalid_argument("Invalid deposit parameters");
    }
}

double Deposit::metric() const
{
    if (cumulative_) {
        return sum_ * (std::pow(1.0 + rate_, years_) - 1.0) / years_;
    }

    return sum_ * rate_;
}

bool Deposit::isCumulative() const
{
    return cumulative_;
}

void Deposit::read(std::istream& in)
{
    double sum;
    double rate;
    int years;
    int cumulative;

    // Accept both plain field values and the labelled output of print().
    in >> std::ws;
    if (in.peek() == 'D')
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
        if (!expect("Deposit:") || !expect("sum=") || !(in >> sum) ||
            !expect("rate=") || !(in >> rate) ||
            !expect("years=") || !(in >> years) ||
            !expect("cumulative=") || !(in >> cumulative) ||
            !expect("metric=") || !(in >> printedMetric)) {
            return;
        }
    }
    else if (!(in >> sum >> rate >> years >> cumulative))
    {
        return;
    }

    if (cumulative != 0 && cumulative != 1) 
    {
        in.setstate(std::ios::failbit);
        return;
    }

    try 
    {
        Deposit temp(sum, rate, years, cumulative == 1);
        *this = temp;
    }
    catch (const std::invalid_argument&) 
    {
        in.setstate(std::ios::failbit);
    }
}

void Deposit::print(std::ostream& out) const
{
    out << "Deposit: sum=" << sum_
        << " rate=" << rate_
        << " years=" << years_
        << " cumulative=" << (cumulative_ ? 1 : 0)
        << " metric=" << metric();
}



    
