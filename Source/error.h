#ifndef ERROR_H
#define ERROR_H

#include <string>

using std::string;

class ErrorBase : public std::exception
{
public:
    ErrorBase();
    explicit ErrorBase(const char* msg) : msg(msg) {}
    explicit ErrorBase(const std::string& msg) : msg(msg) {}

    virtual const char* what() const noexcept override
    {
        return msg.c_str();
    }
protected:
    std::string msg;
};

class MemoryError : public ErrorBase
{
private:
    static constexpr const char* ErrorMsg = "Allocation error";
public:
    explicit MemoryError() : ErrorBase(ErrorMsg) {}
    explicit MemoryError(const std::string& msg) : ErrorBase(ErrorMsg + msg) {}
};

class RangeError : public ErrorBase
{
private:
    static constexpr const char* ErrorMsg = "Index is out of range";
public:
    explicit RangeError() : ErrorBase(ErrorMsg) {}
    explicit RangeError(const std::string& msg) : ErrorBase(ErrorMsg + msg) {}
};

#endif // ERROR_H
