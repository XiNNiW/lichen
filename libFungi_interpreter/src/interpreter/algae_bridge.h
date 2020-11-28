#pragma once
#include <map>
#include <functional>
#include <string>
#include "libAlgae_dsp/dsp.h"
#include "either.h"
#include "error.h"
#include "signal_wrapper.h"
#include "data_varient.h"
#include "symbol.h"

template<typename Sample, typename Frequency, typename Time>
SporeDataVariant createOscBinding(int sampleRate){
    return SporeDataVariant(std::function<Either<SporeError, SporeDataVariant>(SporeDataVariant)>(
        [=](SporeDataVariant args){ 
            if(args.type == SporeDataVariant::t_list && args.as_list.size()== 1){
                auto e = signals::environment<Sample>(sampleRate);
                auto freqVariant = args.as_list[0];
                switch (freqVariant.type)
                {
                case SporeDataVariant::t_int:{
                    auto constantInt = signals::math::constant<Frequency>(Frequency(freqVariant.as_integer));
                    return Either<SporeError, SporeDataVariant>(
                        SporeDataVariant(Signal<Sample,Time>(e.osc(constantInt)))
                    );
                    break;
                }
                case SporeDataVariant::t_float:{

                    // auto constantFloat = ;
                    auto _sig = e.osc(e.constant(freqVariant.as_float));
                    std::cout << "in binding!\n";

                    auto sig = Either<SporeError, SporeDataVariant>(
                        SporeDataVariant(Signal<Sample,Time>(_sig))
                    );
                    std::cout << "out binding!\n";

                    return sig;
                    break;
                }
                case SporeDataVariant::t_bool:{
                    auto constantBool = signals::math::constant<Frequency>(Frequency(freqVariant.as_bool));
                    return Either<SporeError, SporeDataVariant>(
                        SporeDataVariant(Signal<Sample,Time>(e.osc(constantBool)))
                    );
                    break;
                }
                case SporeDataVariant::t_signal:{
                    auto freq = freqVariant.as_signal;
                    return Either<SporeError, SporeDataVariant>(
                        SporeDataVariant(Signal<Sample,Time>(e.osc(freq)))
                    );
                    break;
                }
                
                default:
                    break;
                }

                return Either<SporeError, SporeDataVariant>(
                    SporeError("improper args for signal osc")
                ); 
                
            }else {
                return Either<SporeError, SporeDataVariant>(
                    SporeError("improper args for signal osc")
                );
            }
        }));
}

template<typename Sample, typename Frequency, typename Time>
std::map<std::string, SporeDataVariant> makeAlgaeBindings(int sampleRate){
    return {
        {
            "osc", 
            createOscBinding<Sample, Frequency, Time>(sampleRate)
        }
    };
}

std::vector<SymbolVariant> getAlgaeSemantics(){
    return {
        SymbolVariant(FunctionVariableSymbol("osc", 1, {"freq"}))
    };
}


