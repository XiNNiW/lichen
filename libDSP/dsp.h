#ifndef DSP_H
#define DSP_H

#include <math.h>
#include <vector>

template<typename F, typename G, typename A>
struct compose{
    F f;
    G g;
    compose(F _f, G _g):f(_f),g(_g){}
    auto operator()(A args){
        return f(g(args));
    }
};
namespace signals {
    namespace math {

        template<typename A>
        struct constant {
            A _value;
            constant(const A & value):_value(value){}
            A operator()(const int & t){
                return _value;
            };
        };

        template<typename A, typename B>
        struct multiply {
            A signal_a = A();
            B signal_b = B();
            multiply(const A & a, const B & b):signal_a(a),signal_b(b){}
            auto operator()(const int & t){
                return signal_a(t)*signal_b(t);
            }
        };

        template<typename A, typename B>
        struct divide {
            A signal_a = A();
            B signal_b = B();
            
            divide(const A & a, const B & b):signal_a(a),signal_b(b){}
            auto operator()(const int & t){
                return signal_a(t)/signal_b(t);
            }
        };

        template<typename A, typename B>
        struct add {
            A signal_a = A();
            B signal_b = B();
            
            add(const A & a, const B & b):signal_a(a),signal_b(b){}
            auto operator()(const int & t){
                return signal_a(t)+signal_b(t);
            }
        };

        template<typename A, typename B>
        struct subtract {
            A signal_a = A();
            B signal_b = B();
            
            subtract(const A & a, const B & b):signal_a(a),signal_b(b){}
            auto operator()(const int & t){
                return signal_a(t)-signal_b(t);
            }
        };

        template<typename A>
        struct cosine {
            A signal_a = A();

            cosine(const A & a):signal_a(a){}
            auto operator()(const int & t){
                return cos(signal_a(t));
            }
        };

        template<typename A>
        struct sine {
            A signal_a;
            sine(const A & a):signal_a(a){}
            auto operator()(const int & t){
                return sin(signal_a(t));
            }
        };

        // auto sum = [](auto signals...){
        //     auto amount = constant(0);
        //     for (auto sig : signals)
        //     {
        //         amount = add<decltype(amount),decltype(sig)>(amount,sig);
        //     }
            
        //     auto _summer = [](auto fst, auto rest...){
        //         auto _recurse = _summer(rest)
        //         return add<decltype(fst),decltype(sig)>(amount,sig);
        //     };

        //     return amount;
        // };

    } 
    namespace control{
  
        template<typename sample, typename BPM_sig, typename Num_sig>
        struct beats { 
            const sample SAMPLE_RATE; 
            const sample SECONDS_PER_MINUTE = 60.0;
            BPM_sig _bpm_s;
            Num_sig _numBeats_s;
            beats(const int & sr, const BPM_sig & bpm_s, const Num_sig & numBeats_s)
                 :SAMPLE_RATE(sr),_bpm_s(bpm_s),_numBeats_s(numBeats_s){}

            auto operator()(const int & t){
                return (_numBeats_s(t)/_bpm_s(t))*SECONDS_PER_MINUTE*SAMPLE_RATE;
            };
        };

        template<typename sample, typename Note_sig>
        struct mtof { 
            const sample TWO = 2.0;
            const sample TWELVE = 2.0;
            const sample REF_FREQ = 440.0;
            const sample REF_NOTE = 69;
            Note_sig _n;
            mtof(const Note_sig & n):_n(n){}
            auto operator()(int t){
                return  REF_FREQ * pow (TWO, (_n(t) - REF_NOTE) / TWELVE);
            };
        };

        template<typename A, typename duration_signal>
        struct sequence{
            duration_signal _duration;
            std::vector<A> _events;
            sequence(const duration_signal & duration, const std::vector<A> & events):_duration(duration),_events(events){}
            auto operator()(int t) {
                int idx = ((int)(t/_duration(t)))%_events.size();
                return _events[idx];
            }
        };

        template<typename sample, typename Start_sig, typename End_sig, typename RampTime_sig>
        struct ramp { 
            Start_sig _start;
            End_sig _end;
            RampTime_sig _rampTime;
            ramp(const Start_sig & start, const End_sig & end, const RampTime_sig & rampTimeInSamples):_start(start),_end(end),_rampTime(rampTimeInSamples){}
            auto operator()(int t){
                sample rt = _rampTime(t);
                sample phase = (((sample)t)/((sample)rt));
                return (sample) (t<rt)?_start(t)*(1.0-phase)+_end(t)*phase:_end(t);
            };
        };

        template<typename Signal, typename TimeModulator>
        struct _timeTransform { 
            Signal _sig;
            TimeModulator _mod;
            _timeTransform(const Signal & sig, const TimeModulator & modulator)
                         :_sig(sig),_mod(modulator){}
            auto operator () (const int & t){
                return _sig(_mod(t));
            };
        };

        template<typename Signal, typename TimeModulator>
        auto timeTransform(const Signal & signal, const TimeModulator & modulator){
            return _timeTransform<Signal, TimeModulator>(signal, modulator);
        }

        template<typename TimeSignal>
        struct _shift { 
            TimeSignal _n;
            _shift(const TimeSignal & n):_n(n){}
            auto operator()(const int & t){
                return t+_n(t);
            };
        };

        template<typename TimeSignal>
        auto shift(const TimeSignal & time_s){
            return _shift<TimeSignal>(time_s);
        }

        template<typename Signal>
        struct _invert {
            Signal _signal;
            _invert(const Signal & signal):_signal(signal){}
            auto operator()(const int & t){
                return -_signal(t);
            };
        };

        template<typename Signal>
        auto invert(const Signal & signal){
            return _invert<Signal>(signal);
        }


        template<typename sample, typename AttackTime_sig, typename DecayTime_sig, typename SustainValue_sig, typename SustainTime_sig, typename ReleaseTime_sig>
        struct adsr { 
            math::constant<sample> ZERO = math::constant<sample>(0.0);
            math::constant<sample> ONE = math::constant<sample>(1.0);
            AttackTime_sig _attack;
            DecayTime_sig _decay;
            SustainValue_sig _sustainValue;
            SustainTime_sig _sustainTime;
            ReleaseTime_sig _release;
            
            adsr(const AttackTime_sig & attack, const DecayTime_sig & decay, const SustainValue_sig & sustainValue, const SustainTime_sig & sustainTime, const ReleaseTime_sig & release)
            :_attack(attack),_decay(decay),_sustainValue(sustainValue),_sustainTime(sustainTime),_release(release){}
            auto operator()(const int & t){
                auto releaseOffset = [=](int t){
                    return _attack(t)+_decay(t)+_sustainTime(t);
                };

                auto a = ramp<sample,decltype(ZERO),decltype(ONE),decltype(_attack)>(ZERO,ONE,_attack);
                auto d = timeTransform(
                    ramp<sample,decltype(ONE),decltype(_sustainValue),decltype(_decay)>(ONE, _sustainValue, _decay),
                    shift(invert(_attack))
                );
                auto s = _sustainValue;
                auto r = timeTransform(
                    ramp<sample,decltype(_sustainValue),decltype(ZERO),decltype(_release)>(_sustainValue, ZERO, _release),
                    shift(invert(releaseOffset))
                );
                return 
                (t<_attack(t))?
                    a(t)
                :(t<_attack(t)+_decay(t))?
                    d(t)
                :(t<_attack(t)+_decay(t)+_sustainTime(t))?
                    s(t)
                :
                    r(t);
            };
        };

        auto _loop = [](int loopStartInSamples, int loopStopInSamples){
            return [=](int t){
                return t%(loopStopInSamples-loopStartInSamples)+ loopStartInSamples;
            };
        };

        template<typename StartSample_signal, typename EndTime_signal, typename SignalToLoop>
        struct loop{
            SignalToLoop s;
            StartSample_signal start;
            EndTime_signal end;
            loop(const StartSample_signal & startSampleNum, const EndTime_signal & endSampleNum, const SignalToLoop & signal)
                :start(startSampleNum),end(endSampleNum),s(signal){}
            auto operator()(const int & t){
                int loopLength = end(t)-start(t);
                return s(start(t)+t%(loopLength));
            }
        };

    }
    namespace oscillator {

        template<typename Effect, typename Signal, typename Sample, typename Gain>
        class regenerate{
            Sample previous=0;
            Effect _effect;
            Gain _feedbackAmount;
            regenerate(const Effect & effect,const Gain & feedbackAmount):_effect(effect),_feedbackAmount(feedbackAmount){}
            auto operator()(Signal input){

                return [&](const int & t){
                    auto withfeedback = [=](int t){return input(t)+_feedbackAmount*previous;};
                    previous = _effect(withfeedback)(t);
                    return previous;
                };
            }
        };

        template<typename sample, typename frequency_signal,typename frequency=decltype( std::declval<frequency_signal>()(std::declval<int>()))>
        struct phasor{
            frequency sampleRate=44100;
            frequency phase=0;
            frequency_signal freq = frequency_signal();

            phasor(const int & sr,const frequency_signal & f):sampleRate(sr),freq(f){}
            sample operator ()(const int & t) {
                frequency phi = freq(t)/sampleRate;
                phase = fmod(phase+phi,1.0);
                return phase;
            }
            
        };

        template<typename sample, typename frequency_signal> //typename frequency=decltype( std::declval<frequency_signal>()(std::declval< int >()))>
        struct sinOsc{
            const sample TWO_PI =2*M_PI;
            sample phase = 0;
            sample sampleRate = 44100;
            frequency_signal freq; 

            sinOsc(const int & sr, const frequency_signal & f):sampleRate(sr), freq(f){}
            sample operator()(const int & t){
                sample increment = TWO_PI*freq(t)/sampleRate;
                phase = fmod(phase+increment,TWO_PI);
                return sin(phase);

            }
        };



        template<typename Value_sig>
        struct accumulator{
            long phase=0;
            Value_sig v;
            accumulator(const Value_sig & _v):v(_v){}
            auto operator()(int t){
                phase+=v(t);
                return phase;
            }
        };




    }
    namespace sampling{
        template<typename sample, typename frequency, int SAMPLE_RATE>
        auto Sampler = [](std::vector<sample> buffer){
            return [=](int t) {
                int bufferSize = buffer.size();
                return (t<bufferSize)?buffer[t]:0;
            };
        };
    }

    template<typename S>
    struct SignalIterator{
        int frame=0;
        S s;
        SignalIterator(const S & signal):s(signal){}
        auto operator()(){
            return s(frame++);
        }
    };
    

    template <typename sample>
    struct environment{
        const int SAMPLE_RATE;
        environment(int sr):SAMPLE_RATE(sr){}

        template<typename S>
        auto iterator(const S & signal){  
            return SignalIterator<S>(signal);
        }

        template<typename A>
        const auto constant(const A & a){
            return math::constant<A>(a);
        }
        template<typename A, typename B>
        auto mult(const A & a, const B & b){
            return math::multiply<A,B>(a,b);
        }

        template<typename A, typename B>
        auto div(const A & a,const B & b){
            return math::divide<A,B>(a,b);
        }

        template<typename A, typename B>
        auto add(const A & a,const B & b){
            return math::add<A,B>(a,b);
        }

        template<typename A, typename B>
        auto sub(const A & a,const B & b){
            return math::subtract<A,B>(a,b);
        }

        template<typename StartSample_signal, typename EndTime_signal, typename SignalToLoop>
        auto loop(const StartSample_signal & startSampleNum, const EndTime_signal & endSampleNum, const SignalToLoop & signal){
            return control::loop<StartSample_signal, EndTime_signal, SignalToLoop>(startSampleNum, endSampleNum, signal);
        }

        template<typename BPM_sig, typename Num_sig>
        auto beats(const BPM_sig & bpm,const Num_sig & num){
            return control::beats<sample, BPM_sig, Num_sig>(SAMPLE_RATE,bpm,num);
        }

        template<typename Note_sig>
        auto mtof(const Note_sig & note){
            return control::mtof<sample, Note_sig>(note);
        }

        template<typename A, typename duration_sig>
        auto seq(const duration_sig & duration,const std::vector<A> & events){
            return control::sequence<A, duration_sig>(duration, events);
        }

        template<typename Start_sig, typename End_sig, typename RampTime_sig>
        auto ramp(const Start_sig & start,const End_sig & stop,const RampTime_sig & numberOfSamples){
            return control::ramp<sample,Start_sig, End_sig, RampTime_sig>(start, stop, numberOfSamples);
        }

        template<typename AttackTime_sig, typename DecayTime_sig, typename SustainValue_sig, typename SustainTime_sig, typename ReleaseTime_sig>
        auto adsr(const AttackTime_sig & attack,const DecayTime_sig & decay,const SustainValue_sig & sustainValue,const SustainTime_sig & sustainTime,const ReleaseTime_sig & release){
            return control::adsr
                            <sample, AttackTime_sig, DecayTime_sig, SustainValue_sig, SustainTime_sig, ReleaseTime_sig>
                            (attack, decay, sustainValue, sustainTime, release);
        }

        template<typename Freq_sig>
        auto osc(const Freq_sig & freq){
            return oscillator::sinOsc<sample, Freq_sig>(SAMPLE_RATE,freq);
        }

        template<typename Freq_sig>
        auto phasor(const Freq_sig & freq){
            return oscillator::phasor<sample,Freq_sig>(SAMPLE_RATE,freq);
        }


    };


}


#endif

