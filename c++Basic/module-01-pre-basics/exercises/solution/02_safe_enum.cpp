#include <iostream>
#include <string>

enum class HttpStatus {
    OK = 200,
    NotFound = 404,
    InternalError = 500
};

int toCode(HttpStatus status) {
    return static_cast<int>(status);
}

std::string toMessage(HttpStatus status) {
    switch (status) {
        case HttpStatus::OK: return "OK";
        case HttpStatus::NotFound: return "Not Found";
        case HttpStatus::InternalError: return "Internal Server Error";
        default: return "Unknown";
    }
}

bool isSuccess(HttpStatus status) {
    int code = toCode(status);
    return code >= 200 && code < 300;
}

int main() {
    HttpStatus status = HttpStatus::OK;
    std::cout << "Code: " << toCode(status) << '\n';
    std::cout << "Message: " << toMessage(status) << '\n';
    std::cout << "Is success: " << (isSuccess(status) ? "yes" : "no") << '\n';

    HttpStatus error = HttpStatus::NotFound;
    std::cout << "\nCode: " << toCode(error) << '\n';
    std::cout << "Message: " << toMessage(error) << '\n';
    std::cout << "Is success: " << (isSuccess(error) ? "yes" : "no") << '\n';

    return 0;
}
