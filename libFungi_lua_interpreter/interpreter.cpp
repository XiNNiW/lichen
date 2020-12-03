#include "interpreter.h"

sol::state configureScriptingEnvironment(){
	using dsp = signals::DSPLib<double, 48000>;

	sol::state lua;
	lua.open_libraries(sol::lib::base);

	createAlgaeBindings<dsp>(&lua);

	return lua;
}

Either<Error, Signal> eval(sol::state* lua, std::string code){
	
    sol::protected_function_result result = lua->script(code,[](lua_State*, sol::protected_function_result pfr) {
		// pfr will contain things that went wrong, for either loading or executing the script
		// Can throw your own custom error
		// You can also just return it, and let the call-site handle the error if necessary.
		return pfr;
	});

	if(result.valid()){
		return Either<Error, Signal>::rightOf(result.get<Signal>()._sig);
	}

	return Either<Error,Signal>(Error("BAD"));
}