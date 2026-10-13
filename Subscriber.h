#pragma once
#include <cstddef>
#include <string>

class Subscriber {
private:
    std::string surname_;
    std::string firstName_;
    std::string phone_;

    static void validatePhone(const std::string& phone);
    static void validateName(const std::string& name);

public:
    static constexpr std::size_t MAX_NAME_LENGTH = 50;

    Subscriber(const std::string& surname, const std::string& firstName,
               const std::string& phone);
    Subscriber(const Subscriber& other) = default;
    Subscriber& operator=(const Subscriber& other) = default;

    // Присвоєння рядка змінює лише номер телефону.
    Subscriber& operator=(const std::string& phone);

    const std::string& getSurname() const;
    const std::string& getFirstName() const;
    const std::string& getPhone() const;
};
