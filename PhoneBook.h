#pragma once
#include "Subscriber.h"
#include <cstddef>
#include <string>
#include <vector>

class PhoneBook {
private:
    std::vector<Subscriber> subscribers_;
    std::size_t checkedIndex(int number) const;

public:
    PhoneBook() = default;
    PhoneBook(const PhoneBook& other) = default;
    PhoneBook& operator=(const PhoneBook& other) = default;

    // Повертає копію книги з доданим абонентом.
    PhoneBook operator+(const Subscriber& subscriber) const;

    Subscriber& at(int number); // Нумерація контактів починається з 1.
    void remove(int number);
    const std::vector<Subscriber>& getSubscribers() const;

    std::vector<Subscriber> findPhone(const std::string& surname) const;
    std::vector<Subscriber> findPhone(const std::string& surname,
                                     bool partialMatch) const;
};
