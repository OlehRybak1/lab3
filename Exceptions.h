#pragma once
#include <stdexcept>
#include <string>

class InvalidPhoneException : public std::runtime_error {
public:
    InvalidPhoneException()
        : std::runtime_error("Некоректний номер телефону (має бути 10 цифр)!") {}
};

class ContactNotFoundException : public std::runtime_error {
public:
    ContactNotFoundException()
        : std::runtime_error("Абонента не знайдено!") {}
};

class InvalidInputException : public std::runtime_error {
public:
    explicit InvalidInputException(const std::string& message)
        : std::runtime_error(message) {}
};

class InputClosedException : public std::runtime_error {
public:
    InputClosedException() : std::runtime_error("Введення завершено.") {}
};
