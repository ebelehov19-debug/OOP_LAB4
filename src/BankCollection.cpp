#include<BankCollection.h>

#include <istream>
#include <ostream>
#include <stdexcept>

BankCollection:: BankCollection():data_(nullptr),capacity_(0),size_(0) {}

BankCollection::~BankCollection()
{
    for (std::size_t i = 0; i < size_; ++i)
    {
        delete data_[i];
    }
    delete[] data_;
}

BankCollection:: BankCollection(const BankCollection& other):data_(nullptr), size_(0), capacity_(other.size_)
{
    if (capacity_ == 0)
        return;
    data_ = new Bank_Optional*[capacity_];

    for (; size_ < other.size_; ++size_)
            data_[size_] = other.data_[size_]->clone();
}
BankCollection:: BankCollection(BankCollection&& other) noexcept
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_)
{
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

void BankCollection::swap(BankCollection& other) noexcept
{
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}
BankCollection& BankCollection::operator=(const BankCollection& other)
{
    BankCollection tmp(other);
    swap(tmp);                   
    return *this;
}
BankCollection& BankCollection::operator=(BankCollection&& other) noexcept
{
    if (this != &other)
    {
        BankCollection tmp(std::move(other));
        swap(tmp);                             
    }
    return *this;
}