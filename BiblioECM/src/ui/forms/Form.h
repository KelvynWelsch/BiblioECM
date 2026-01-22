#pragma once
#include <string>
#include <vector>
#include <functional>
#include "../../core/errors/errors.h"

inline ECM::Expected<std::string> accept_any(const std::string& s) {
    return s;
}

inline ECM::Expected<int> int_validator(const std::string& s) {
    if (s.empty()) {
        return std::unexpected(validation("Id vazio!"));
    }
    for (char c : s) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return std::unexpected(validation("Foram inseridos caracteres inválidos!"));
        }
    }
    return std::stoi(s);
}

struct IField {
    virtual ~IField() = default;
    virtual const std::string& label() const = 0;
    virtual const std::string& value() const = 0;
    virtual bool handleInput(const std::string& input, std::string& error) = 0;
};

template <class T>
struct Field : IField {
    std::string _label;
    std::string _value;

    std::function<ECM::Expected<T>(const std::string&)> validator;
    std::optional<T> parsed;

    Field(std::string label,
        std::function<ECM::Expected<T>(const std::string&)> validator)
        : _label(std::move(label)), validator(std::move(validator)) {
    }

    const std::string& label() const override { return _label; }
    const std::string& value() const override { return _value; }

    void set(const std::string& raw, const T& value) {
        _value = raw;
        parsed = value;
    }

    bool handleInput(const std::string& input, std::string& erro) override {
        if (input.empty() && parsed.has_value()) {
            return true;
        }

        auto res = validator(input);
        if (!res.has_value()) {
            erro = res.error().message;
            return false;
        }
        _value = input;
        parsed = res.value();
        return true;
    }

    const T& get() const { return parsed.value(); }
};


class Form {
public:
    Form(std::string title, std::vector<std::unique_ptr<IField>> fields);

    explicit Form(std::string title)
        : title(std::move(title)) {
    }

    // true = confirmado | false = cancelado
    bool run();

    const std::string& get(int i) const;

    void add(std::unique_ptr<IField> field);

private:
    std::string title;
    std::vector<std::unique_ptr<IField>> fields;
    int current = 0;

    void draw();
    void showError(const std::string& msg) const;
    bool confirm() const;
};

