#pragma once

#include <string>
#include <sstream>

// کلاس پایه برای خطاهای Signup
class SignupException {
private:
    std::string error_message; // پیام خطا

public:
    // سازنده
    SignupException(const std::string& message)
        : error_message(message) {}

    // دریافت پیام خطا
    std::string get_message() const {
        return error_message;
    }
};

// استثناء برای نام کاربری تکراری
class UsernameAlreadyExists : public SignupException {
public:
    UsernameAlreadyExists()
        : SignupException("Bad Request: Username already exists.") {}
};

// استثناء برای کاربر لاگین‌شده
class UserAlreadyLoggedIn : public SignupException {
public:
    UserAlreadyLoggedIn()
        : SignupException("Permission Denied: User already logged in.") {}
};
