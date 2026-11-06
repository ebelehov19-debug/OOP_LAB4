#pragma once

#include "Bank_Optional.h"
#include <cstddef>

class BankCollection
{
private:
    Bank_Optional** data_;
    std::size_t size_;
    std::size_t capacity_;
    void reserve(std::size_t capacity);
public:
    BankCollection();
    ~BankCollection();
    BankCollection(const BankCollection& other);
    BankCollection& operator=(const BankCollection& other);
    BankCollection(BankCollection&& other) noexcept;
    BankCollection& operator=(BankCollection&& other) noexcept;

    void add(Bank_Optional* object);
    void remove(std::size_t index);

    std::size_t size() const;
    Bank_Optional& at(std::size_t index);
    const Bank_Optional& at(std::size_t index) const;

    double totalMetric() const;
    void sortByMetric();
};
