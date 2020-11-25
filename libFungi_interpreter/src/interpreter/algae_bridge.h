#pragma once
#include <map>
#include <functional>
#include <string>
#include "libAlgae_dsp/dsp.h"
#include "either.h"
#include "error.h"
#include "signal_wrapper.h"
#include "data_varient.h"

template<typename Sample, typename Frequency, typename Time>
std::map<std::string, SporeDataVariant> makeAlgaeBindings(int sampleRate){
    return 
    {
        {
            "osc", 
            SporeDataVariant(std::function<Either<SporeError, SporeDataVariant>>([](SporeDataVariant args){ 
                if(args.type != SporeDataVariant::t_list && args.as_list.size()== 1)
                    Signal freq = Signal(args.as_list[0]);
                    
                    return Either<SporeError, SporeDataVariant>(
                        SporeDataVariant(Signal<Sample,Time>(signals::oscillator::sinOsc<Sample,Frequency>(freq)))
                    );
                else {
                    return Either<SporeError, SporeDataVariant>(
                        SporeError("improper args for signal osc")
                    );
                }
            }))
        }
    };
}
