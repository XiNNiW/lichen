#pragma once
#include <string>
#include "interpreter/interpreter.h"
#include <sporelang_bison.hpp>
#include "lang/sporelang_syntaxtree.h"
#include <sstream>


extern std::istream * yyin;

int eval(std::string program);



