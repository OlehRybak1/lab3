#include "Subscriber.h"
#include "Exceptions.h"
#include <algorithm>

void Subscriber::validatePhone(const std::string& phone) {
    if (phone.size() != 10 ||
        !std::all_of(phone.begin(), phone.end(), [](char c) {
            return c >= '0' && c <= '9';
        })) {
        throw InvalidPhoneException();
    }
}

Subscriber::Subscriber(const std::string& surname,
                       const std::string& firstName,
                       const std::string& phone)
    : surname_(surname), firstName_(firstName) {
    if (surname.find_first_not_of(" \t\r\n") == std::string::npos ||
        firstName.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw InvalidInputException("Прізвище та ім'я не можуть бути порожніми!");
    }
    *this = phone;
}

Subscriber& Subscriber::operator=(const std::string& phone) {
    validatePhone(phone); // Помилкове введення не змінює попередній номер.
    phone_ = phone;
    return *this;
}

const std::string& Subscriber::getSurname() const { return surname_; }
const std::string& Subscriber::getFirstName() const { return firstName_; }
const std::string& Subscriber::getPhone() const { return phone_; }
