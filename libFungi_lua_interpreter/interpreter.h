#pragma once
#define SOL_ALL_SAFETIES_ON 1
// #define SOL_NO_EXCEPTIONS 1
#include <sol/sol.hpp> // or #include "sol.hpp", whichever suits your needs
#include <string>
#include <iostream>
#include <libAlgae_dsp/dsp.h>
#include <libFungi_lua_interpreter/src/signal_wrapper.h>
#include <libFungi_lua_interpreter/src/algae_bindings.h>
#include "src/either.h"
#include "src/error.h"

sol::state configureScriptingEnvironment();

Either<Error, Signal> eval(sol::state* lua, std::string code);