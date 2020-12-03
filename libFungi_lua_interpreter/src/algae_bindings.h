#pragma once
#include <sol/sol.hpp>
#include <libAlgae_dsp/dsp.h>
#include "signal_wrapper.h"
#include <map>

template<typename dsp>
void bindSignalWrapperType(sol::state* lua){
    lua->new_usertype<Signal>("Signal", 
		sol::constructors<Signal(), Signal(std::function<double(int)>)>(),
		
		sol::meta_function::multiplication, 
			sol::overload(
				sol::resolve<Signal(const Signal&)>(&Signal::operator*),
				sol::resolve<Signal(double)>(&Signal::operator*),
				sol::resolve<Signal(int)>(&Signal::operator*)
			),
		sol::meta_function::division, 
			sol::overload(
				sol::resolve<Signal(const Signal&)>(&Signal::operator+),
				sol::resolve<Signal(double)>(&Signal::operator+),
				sol::resolve<Signal(int)>(&Signal::operator+)
			),
		sol::meta_function::addition, 
			sol::overload(
				sol::resolve<Signal(const Signal&)>(&Signal::operator+),
				sol::resolve<Signal(double)>(&Signal::operator+),
				sol::resolve<Signal(int)>(&Signal::operator+)
			),
		sol::meta_function::subtraction, 
			sol::overload(
				sol::resolve<Signal(const Signal&)>(&Signal::operator-),
				sol::resolve<Signal(double)>(&Signal::operator-),
				sol::resolve<Signal(int)>(&Signal::operator-)
			)
	);
}

template<typename dsp>
void bindOscillators(sol::state* lua){
    lua->set_function(
		"osc", 
		sol::overload(
			[](double freq)->Signal {
				return Signal(
					dsp::osc(
						dsp::constant(freq)
					)
				);
			},
			[](Signal freq)->Signal {
				return Signal(
					dsp::osc(
						freq._sig
					)
				);
			}
		)
	);

	lua->set_function(
		"phasor", 
		sol::overload(
			[](double freq)->Signal {
				return Signal(
					dsp::phasor(
						dsp::constant(freq)
					)
				);
			},
			[](Signal freq)->Signal {
				return Signal(
					dsp::phasor(
						freq._sig
					)
				);
			}
		)
	);

}

template<typename dsp>
void bindEnvelopes(sol::state* lua){
    lua->set_function(
		"adsr", 
		sol::overload(
			[](int attack, int decay, double sustainLevel, int sustainTime, int release)->Signal {
				return Signal(
					dsp::adsr(
						dsp::constant(attack),
						dsp::constant(decay),
						dsp::constant(sustainLevel),
						dsp::constant(sustainTime),
						dsp::constant(release)
					)
				);
			},
			[](Signal attack, Signal decay, Signal sustainLevel, Signal sustainTime, Signal release)->Signal {
				return Signal(
					dsp::adsr(
						attack._sig,
						decay._sig,
						sustainLevel._sig,
						sustainTime._sig,
						release._sig
					)
				);
			}
		)
	);

}

template<typename dsp>
void bindControlSignals(sol::state* lua){
    lua->set_function(
		"ramp", 
		sol::overload(
			[](double start, double stop, int ramptime)->Signal {
				return Signal(
					dsp::ramp(
						dsp::constant(start),
						dsp::constant(stop),
						dsp::constant(ramptime)
					)
				);
			},
			[](Signal start, Signal stop, Signal ramptime)->Signal {
				return Signal(
					dsp::ramp(
						start._sig,
						stop._sig,
						ramptime._sig
					)
				);
			}
		)
	);
    lua->set_function(
		"loop", 
		sol::overload(
			[](double start, double stop, Signal signalToLoop)->Signal {
				return Signal(
					dsp::loop(
						dsp::constant(start),
						dsp::constant(stop),
						signalToLoop
					)
				);
			},
			[](Signal start, Signal stop, Signal signalToLoop)->Signal {
				return Signal(
					dsp::loop(
						start._sig,
						stop._sig,
						signalToLoop._sig
					)
				);
			}
		)
	);
    lua->set_function(
		"seq", 
		sol::overload(
			[](Signal duration, std::vector<double> events)->Signal {
				return Signal(
					dsp::seq(
                        duration._sig,
						events
					)
				);
			},
            [](Signal duration, std::vector<int> events)->Signal {
				return Signal(
					dsp::seq(
                        duration._sig,
						events
					)
				);
			}

		)
	);

}

template<typename dsp>
void bindUnitConverters(sol::state* lua){
    lua->set_function(
		"mtof", 
		sol::overload(
			[](double note)->Signal {
				return Signal(
					dsp::mtof(
						dsp::constant(note)
					)
				);
			},
			[](Signal note)->Signal {
				return Signal(
					dsp::mtof(
						note._sig
					)
				);
			}
		)
	);
    lua->set_function(
		"beats", 
		sol::overload(
			[](double bpm,double beats)->Signal {
				return Signal(
					dsp::beats(
						dsp::constant(bpm),
						dsp::constant(beats)
					)
				);
			},
			[](Signal bpm, Signal beats)->Signal {
				return Signal(
					dsp::beats(
                        bpm._sig,
						beats._sig
					)
				);
			}
		)
	);
}

template<typename dsp>
void bindMathFunctions(sol::state* lua){
    lua->set_function(
		"constant", 
		sol::overload(
			[](double a)->Signal {
				return Signal(
                    dsp::constant(a)
				);
			}
		)
	);
    lua->set_function(
		"mult", 
		sol::overload(
			[](double a,double b)->Signal {
				return Signal(
                    dsp::mult(
                        dsp::constant(a),
                        dsp::constant(b)
                    )
				);
			},
            [](Signal a,double b)->Signal {
				return Signal(
                    dsp::mult(
                        a._sig,
                        dsp::constant(b)
                    )
				);
			},
            [](double a,Signal b)->Signal {
				return Signal(
                    dsp::mult(
                        dsp::constant(a),
                        b._sig
                    )
				);
			},
            [](Signal a,Signal b)->Signal {
				return Signal(
                    dsp::mult(
                        a._sig,
                        b._sig
                    )
				);
			}
		)
	);
    lua->set_function(
		"div", 
		sol::overload(
			[](double a,double b)->Signal {
				return Signal(
                    dsp::div(
                        dsp::constant(a),
                        dsp::constant(b)
                    )
				);
			},
            [](Signal a,double b)->Signal {
				return Signal(
                    dsp::div(
                        a._sig,
                        dsp::constant(b)
                    )
				);
			},
            [](double a,Signal b)->Signal {
				return Signal(
                    dsp::div(
                        dsp::constant(a),
                        b._sig
                    )
				);
			},
            [](Signal a,Signal b)->Signal {
				return Signal(
                    dsp::div(
                        a._sig,
                        b._sig
                    )
				);
			}
		)
	);
    lua->set_function(
		"add", 
		sol::overload(
			[](double a,double b)->Signal {
				return Signal(
                    dsp::add(
                        dsp::constant(a),
                        dsp::constant(b)
                    )
				);
			},
            [](Signal a,double b)->Signal {
				return Signal(
                    dsp::add(
                        a._sig,
                        dsp::constant(b)
                    )
				);
			},
            [](double a,Signal b)->Signal {
				return Signal(
                    dsp::add(
                        dsp::constant(a),
                        b._sig
                    )
				);
			},
            [](Signal a,Signal b)->Signal {
				return Signal(
                    dsp::add(
                        a._sig,
                        b._sig
                    )
				);
			}
		)
	);
    lua->set_function(
		"sub", 
		sol::overload(
			[](double a,double b)->Signal {
				return Signal(
                    dsp::sub(
                        dsp::constant(a),
                        dsp::constant(b)
                    )
				);
			},
            [](Signal a,double b)->Signal {
				return Signal(
                    dsp::sub(
                        a._sig,
                        dsp::constant(b)
                    )
				);
			},
            [](double a,Signal b)->Signal {
				return Signal(
                    dsp::sub(
                        dsp::constant(a),
                        b._sig
                    )
				);
			},
            [](Signal a,Signal b)->Signal {
				return Signal(
                    dsp::sub(
                        a._sig,
                        b._sig
                    )
				);
			}
		)
	);
    lua->set_function(
		"cos", 
		sol::overload(
			[](double a)->Signal {
				return Signal(
                    dsp::cos(
                        dsp::constant(a)
                    )
				);
			},
            [](Signal a)->Signal {
				return Signal(
                    dsp::cos(
                        a._sig
                    )
				);
			}
		)
	);
    lua->set_function(
		"sin", 
		sol::overload(
			[](double a)->Signal {
				return Signal(
                    dsp::sin(
                        dsp::constant(a)
                    )
				);
			},
            [](Signal a)->Signal {
				return Signal(
                    dsp::sin(
                        a._sig
                    )
				);
			}
		)
	);
}



template<typename dsp>
void createAlgaeBindings(sol::state* lua){
    
    bindSignalWrapperType<dsp>(lua);
    bindMathFunctions<dsp>(lua);
    bindUnitConverters<dsp>(lua);
    bindControlSignals<dsp>(lua);
    bindEnvelopes<dsp>(lua);
    bindOscillators<dsp>(lua);
	
	
}

	


