#pragma once
#include <functional>


template<typename Sample,typename Time>
struct Signal{
    std::function<Sample(Time)> _sig;
    // ~Signal(){
    //     using std::function;
    //     _sig.~function<Sample(Time)>();
    // }
    Signal():_sig(std::function<Sample(Time)>([](int t){return 0;})){}

    // Signal(const Signal& s):_sig(s._sig){}
    // Signal& operator = (const Signal& s){
    //     _sig = s._sig;
    //     return *this;
    // }
    Signal(std::function<Sample(Time)> f):_sig(f){}
    friend Signal add(Signal a, Signal b){
        return Signal(std::function<Sample(Time)>([=](Time t){return a(t)+b(t);}));
    }
    Signal operator +(Signal a){
        return add(*this,a);
    }
    friend Signal subtract(Signal a, Signal b){
        return Signal(std::function<Sample(Time)>([=](Time t){return a(t)-b(t);}));
    }
    Signal operator -(Signal a){
        return subtract(*this,a);
    }
    friend Signal mult(Signal a, Signal b){
        return Signal(std::function<Sample(Time)>([=](Time t){return a(t)*b(t);}));
    }
    Signal operator *(Signal a){
        return mult(*this,a);
    }
    friend Signal divide(Signal a, Signal b){
        return Signal(std::function<Sample(Time)>([=](Time t){return a(t)/b(t);}));
    }
    Signal operator /(Signal a){
        return divide(*this,a);
    }

    Sample operator()(Time t){
        return _sig(t);
    }


};
