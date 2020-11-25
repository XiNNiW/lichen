#pragma once
#include <string>
struct SporeError{
    enum {t_no_error, t_null_ref, t_name_not_found} type;
    std::string message;
    SporeError(std::string message):message(message){}
    SporeError(const SporeError& s):type(s.type),message(s.message){}
    SporeError(){}
};

