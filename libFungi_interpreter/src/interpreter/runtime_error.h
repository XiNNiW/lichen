#pragma once
#include <string>

struct RuntimeError{
    enum {t_no_error, t_null_ref, t_name_not_found} type;
    std::string message;
    RuntimeError(std::string message):message(message){}
    RuntimeError(const RuntimeError& s):type(s.type),message(s.message){}
    RuntimeError(){}
};
