#pragma once
#include <string>
struct SyntaxError{
    enum {t_no_error, t_null_ref, t_name_not_found} type;
    std::string message;
    SyntaxError(std::string message):message(message){}
    SyntaxError(const SyntaxError& s):type(s.type),message(s.message){}
    SyntaxError(){}
};

