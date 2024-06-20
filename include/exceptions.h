#include <exception>
#include <string>

class NotImplementedException : public std::exception {
public:
    NotImplementedException(const std::string& message = "Function not yet implemented")
        : msg_(message) {}

    virtual const char* what() const noexcept override {
        return msg_.c_str();
    }

private:
    std::string msg_;
};