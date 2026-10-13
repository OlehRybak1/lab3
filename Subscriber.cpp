#include "Subscriber.h"
#include "Exceptions.h"
#include <algorithm>

void Subscriber::validateName(const std::string& name) {
    // У UTF-8 рахуємо початкові байти символів, а не всі байти рядка.
    const auto length = static_cast<std::size_t>(std::count_if(
        name.begin(), name.end(), [](unsigned char c) {
            return (c & 0xC0) != 0x80;
        }));

    if (name.find_first_not_of(" \t\r\n") == std::string::npos) {
        throw InvalidInputException("Прізвище та ім'я не можуть бути порожніми!");
    }
    if (length > MAX_NAME_LENGTH || name.size() > 4 * MAX_NAME_LENGTH) {
        throw InvalidInputException(
            "Прізвище та ім'я можуть містити не більше 50 символів кожне!");
    }
}

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
    validateName(surname);
    validateName(firstName);
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
