#pragma once

#include <functional>
#include <libAlgae_dsp/dsp.h>


struct Signal{
    std::function<double(int)> _sig;

    Signal():_sig(std::function<double(int)>([](int t){return 0;})){}

    Signal(const std::function<double(int)>& f):_sig(f){
    }
    friend Signal add(const Signal& a, const Signal& b){
        return Signal(signals::math::add(a._sig,b._sig));
    }
    Signal operator +(const Signal& a){
        return add(*this,a);
    }
    Signal operator +(double a){
        return add(*this,Signal(signals::math::constant(a)));
    }
    Signal operator +(int a){
        return add(*this,Signal(signals::math::constant(a)));
    }
    friend Signal subtract(const Signal& a, const Signal& b){
        return Signal(signals::math::subtract(a._sig,b._sig));
    }
    Signal operator -(const Signal& a){
        return subtract(*this,a);
    }
    Signal operator -(double a){
        return subtract(*this,Signal(signals::math::constant(a)));
    }
    Signal operator -(int a){
        return subtract(*this,Signal(signals::math::constant(a)));
    }
    friend Signal mult(const Signal& a, const Signal& b){
        return Signal(signals::math::multiply(a._sig,b._sig));
    }
    Signal operator *(const Signal& a){
        return mult(*this,a);
    }
    Signal operator *(double a){
        return mult(*this,Signal(signals::math::constant(a)));
    }
    Signal operator *(int a){
        return mult(*this,Signal(signals::math::constant(a)));
    }
    friend Signal divide(const Signal& a, const Signal& b){
        return Signal(signals::math::divide(a._sig,b._sig));
    }
    Signal operator /(const Signal& a){
        return divide(*this,a);
    }
    Signal operator /(double a){
        return divide(*this,Signal(signals::math::constant(a)));
    }
    Signal operator /(int a){
        return divide(*this,Signal(signals::math::constant(a)));
    }

    double operator()(int t){
        return _sig(t);
    }


};

