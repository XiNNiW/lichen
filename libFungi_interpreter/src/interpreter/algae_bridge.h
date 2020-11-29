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


Either<SporeError, SporeDataVariant> castAsSignal(SporeDataVariant arg){
    using E = Either<SporeError, SporeDataVariant>;
    using S = Signal<double, int>;
    switch (arg.type)
    {
    case SporeDataType::t_int:
        return E::rightOf( SporeDataVariant((S(signals::math::constant<double>(arg.as_integer)))));
        break;
    case SporeDataType::t_float:
        return E::rightOf( SporeDataVariant((S(signals::math::constant<double>(arg.as_float)))));
        break;
    case SporeDataType::t_bool:
        return E::rightOf( SporeDataVariant((S(signals::math::constant<double>(arg.as_bool)))));
        break;
    case SporeDataType::t_signal:
        return E::rightOf( arg);
        break;
    
    default:
        break;
    }
    std::cout << "cast failed!!!! ;(...";
    return Either<SporeError, SporeDataVariant>::leftOf(SporeError("invalid cast to signal"));
}

// template<typename T>
// Either<SporeError, std::vector<T>> castAsVector(SporeDataVariant arg){
//     using E = Either<SporeError, std::vector<T>>;
//     using S = Signal<double, int>;
//     if(arg.type==t_list){
//         std::vector<E> casted;
//         std::transform(arg.as_list.begin(), arg.as_list.end(), casted.begin(), [](SporeDataVariant el){
//             switch(el.type){

//             }
//         });
//     }


//     return Either<SporeError, SporeDataVariant>::leftOf(SporeError("invalid cast to signal"));
// }




template<typename T>
std::vector<T> vconcat(std::vector<T> lhs, std::vector<T> rhs){
    std::vector<T> result;
    result.reserve( lhs.size() + rhs.size() ); // preallocate memory
    result.insert( result.end(), lhs.begin(), rhs.end() );
    result.insert( result.end(), lhs.begin(), rhs.end() );
    return result;
}

// template<typename T=Signal<double,int>, typename V>
// Either<SporeError,SporeDataVariant> castAs(V data){
   
//     return castAsSignal(data);

// }

// template<typename T, typename D>
// Either<SporeError,std::vector<SporeDataVariant>> castArgs(D data){
//     return castAs<T>(data).flatMap([&](SporeDataVariant casted){
//         return std::vector<SporeDataVariant>(casted);
//     });
// }

// template<typename T, typename ...Rest, typename D, typename... Drest>
// Either<SporeError,std::vector<SporeDataVariant>> castArgs(D fst, Drest... rst){
//     return castAs<T>(fst).flatMap([&](SporeDataVariant fstCast){
//         castArgs(rst...).flatMap([&](std::vector<SporeDataVariant> restCast){
//             return vconcat(std::vector<SporeDataVariant>(fstCast),restCast);
//         });
//     });
// }

// template <std::size_t... Indices>
// struct indices {
//     using next = indices<Indices..., sizeof...(Indices)>;
// };

// template <std::size_t N>
// struct build_indices {
//     using type = typename build_indices<N-1>::type::next;
// };
// template <>
// struct build_indices<0> {
//     using type = indices<>;
// };
// template <std::size_t N>
// using BuildIndices = typename build_indices<N>::type;



// template <size_t num_args>
// struct unpack_caller
// {
// private:
//     template <typename FuncType, size_t... I>
//     void call(FuncType &f, std::vector<SporeDataVariant> &args, indices<I...>){
//         f(args[I]...);
//     }

// public:
//     template <typename FuncType>
//     void operator () (FuncType &f, std::vector<SporeDataVariant> &args){
//         assert(args.size() == num_args); // just to be sure
//         call(f, args, BuildIndices<num_args>{});
//     }
// };


// template<typename T, typename ...Rest>
// Either<SporeError,std::vector<SporeDataVariant>> castAllArgs(SporeDataVariant args){
//     bool valid = args.type==SporeDataType::t_list ? args.as_list.size() == 1+sizeof...(Rest) : false;

//     if(valid){
//         return unpack_caller<1+sizeof...(Rest)>(
//             // std::function<Either<SporeError,std::vector<SporeDataVariant>>(SporeDataVariant...)>(
//                 [](SporeDataVariant data...){return castArgs<T,Rest...>(data...); },
//             // ),
//             args.as_list
//         );
//     } else {
//         return Either<SporeError,std::vector<SporeDataVariant>>::leftOf(SporeError("wrong number of args"));
//     }
// }


template<typename T>
Either<SporeError,SporeDataVariant> bind(T rawSignal){
    using E = Either<SporeError,SporeDataVariant>;
    return E::rightOf(SporeDataVariant(Signal<double,int>(rawSignal)));
}

template<typename T>
Either<SporeError, std::vector<SporeDataVariant>> concatEither(T lst){
    using E = Either<SporeError, std::vector<SporeDataVariant>>;
    if(lst.isLeft())
        return E::leftOf(lst.getLeft());

    return E::rightOf(std::vector<SporeDataVariant>({lst.getRight()}));

}

template<typename T, typename ... Rest>
Either<SporeError, std::vector<SporeDataVariant>> concatEither(T fst, Rest...rst){
    using E = Either<SporeError, std::vector<SporeDataVariant>>;
    if(fst.isLeft())
        return E::leftOf(fst.getLeft());
    std::vector<SporeDataVariant> fstVector({fst.getRight()});
    return concatEither<Rest...>(rst...).flatMap([&](std::vector<SporeDataVariant> theRest){
        return E::rightOf(vconcat(fstVector,theRest));
    });
    
}

template<typename F>
SporeDataVariant makeSporeFunction(F func){
    return SporeDataVariant(std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>(func));
}

template<typename Sample, typename Frequency, typename Time>
std::map<std::string, SporeDataVariant> makeAlgaeBindings(int sampleRate){
    using DSP = signals::DSPLib<double,48000>;
    return {
        {
            "osc", 
            
            makeSporeFunction([](SporeDataVariant args){
                return castAsSignal(args.as_list[0]).flatMap([](SporeDataVariant arg1){
                    return bind(
                        DSP::osc(arg1.as_signal._sig)
                    );
                });
            })
            

        },
        
        {
            "phasor", 
            makeSporeFunction([](SporeDataVariant args){
                return castAsSignal(args.as_list[0]).flatMap([](SporeDataVariant arg1){
                    return bind(
                        DSP::phasor(arg1.as_signal._sig)
                    );
                });
            })
        },
        {
            "mtof", 
            makeSporeFunction([](SporeDataVariant args){
                return castAsSignal(args.as_list[0]).flatMap([](SporeDataVariant arg1){
                    return bind(
                        DSP::mtof(arg1.as_signal._sig)
                    );
                });
            })
        },
        // {
        //     "seq", 
        //     makeSporeFunction([](SporeDataVariant args){
        //         switch (args.as_list[0].type)
        //         {
        //         case SporeDataType::t_int:
        //             return castAsVector<int>(args.as_list[0]).flatMap([](std::vector<int> arg1){
        //                 return bind(
        //                     DSP::seq(arg1.as_signal._sig)
        //                 );
        //             });
        //             break;
        //         case SporeDataType::t_float:
        //             return castAsVector<double>(args.as_list[0]).flatMap([](std::vector<double> arg1){
        //                 return bind(
        //                     DSP::seq(arg1.as_signal._sig)
        //                 );
        //             });
        //             break;
        //         case SporeDataType::t_bool:
        //             return castAsVector<bool>(args.as_list[0]).flatMap([](std::vector<bool> arg1){
        //                 return bind(
        //                     DSP::seq(arg1.as_signal._sig)
        //                 );
        //             });
        //             break;
                
        //         default:
        //             break;
        //         }

        //         return E::leftOf(SporeError("invalid args for seq"));
                
        //     })
        // },
        {
            "adsr",
            
                makeSporeFunction([](SporeDataVariant args){
                    return concatEither(
                        castAsSignal(args.as_list[0]),
                        castAsSignal(args.as_list[1]),
                        castAsSignal(args.as_list[2]),
                        castAsSignal(args.as_list[3]),
                        castAsSignal(args.as_list[4])
                    )
                    .flatMap([=](std::vector<SporeDataVariant> args){
                        return bind(
                            DSP::adsr(
                                args[0].as_signal._sig,
                                args[1].as_signal._sig,
                                args[2].as_signal._sig,
                                args[3].as_signal._sig,
                                args[4].as_signal._sig
                            )
                        );
                    });

                })
            
        }
        ,
        {
            "ramp",
                makeSporeFunction([](SporeDataVariant args){
                    return concatEither(
                        castAsSignal(args.as_list[0]),
                        castAsSignal(args.as_list[1]),
                        castAsSignal(args.as_list[2])
                    )
                    .flatMap([=](std::vector<SporeDataVariant> args){
                        return bind(
                            DSP::ramp(
                                args[0].as_signal._sig,
                                args[1].as_signal._sig,
                                args[2].as_signal._sig
                            )
                        );
                    });

                })
        },
        {
            "loop",
                makeSporeFunction([](SporeDataVariant args){
                    return concatEither(
                        castAsSignal(args.as_list[0]),
                        castAsSignal(args.as_list[1]),
                        castAsSignal(args.as_list[2])
                    )
                    .flatMap([=](std::vector<SporeDataVariant> args){
                        return bind(
                            DSP::loop(
                                args[0].as_signal._sig,
                                args[1].as_signal._sig,
                                args[2].as_signal._sig
                            )
                        );
                    });

                })
        }
    };
}

std::vector<SymbolVariant> getAlgaeSemantics(){
    return {
        SymbolVariant(FunctionVariableSymbol("osc", 1, {"freq"}))
    };
}


