#include "interpreter.h"

sol::state configureScriptingEnvironment(){
	using dsp = signals::DSPLib<double, 48000>;

	sol::state lua;
	lua.open_libraries(sol::lib::base);

	createAlgaeBindings<dsp>(&lua);

	return lua;
}

auto solErrorCallBack = [](lua_State*, sol::protected_function_result pfr) {
	return pfr;
};

Either<Error,Signal> handleResult(const sol::protected_function_result& result){
	if(result.valid()){
		return Either<Error, Signal>::rightOf(result.get<Signal>());
	} else {
		sol::error err = result;
		return Either<Error,Signal>(Error(err.what()));
	}
}

Either<Error, Signal> eval(sol::state* lua, std::string code){
	
    sol::protected_function_result result = lua->script(code, solErrorCallBack);

	return handleResult(result);


}

Either<Error, Signal> evalFile(sol::state* lua, std::string filePath){
	
    sol::protected_function_result result = lua->script_file(filePath,solErrorCallBack);

	return handleResult(result);

}