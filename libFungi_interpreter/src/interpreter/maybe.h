#pragma once
#include <functional>

    template<typename T>
    class Maybe {
        private:
            std::unique_ptr<T> _value;
            Maybe<T> (const T& value):_value(new T(value)){}
            Maybe<T> ():_value(nullptr){}
        public:
            static Maybe<T> of(const T& value) { return Maybe<T>(value);}
            static Maybe<T> nothing() { return Maybe<T>();}

            const T& getValue() const {
                return *(_value);
            }

            template<typename F, typename R=decltype( std::declval<F>()(std::declval<T>()) ) >
            Maybe<R> map(const F& f) const {
                return this->isSomthing()? Maybe<T>::of(f(this->getValue())): Maybe<T>::nothing();
            }

            template<typename F, typename R >
            Maybe<R> map(const Maybe<T>& m, const std::function<R(T)>& f) const {
                return this->isSomthing()? Maybe<T>::of(f(this->getValue())): Maybe<T>::nothing();
            }

            template<typename F, typename MR=decltype( std::declval<F>()(std::declval<T>()) ) >
            MR flatMap( const F& f) const {
                return this->isSomthing()? f(this->getValue()): MR();
            }

            template<typename F, typename R >
            Maybe<R> flatMap( const std::function<Maybe<R>(T)>& f) const {
                return this->isSomthing()? f(this->getValue()): Maybe<R>::nothing();
            }

            const T& getOrElse(const T& elseValue) const {
                return this->isSomthing()? this->getValue(): elseValue;
            }

            const bool isNothing() const {
                return _value?false:true;
            }

            const bool isSomthing() const {
                return _value?true:false;
            }
    };

    template<typename T>
    Maybe<T> Just(const T& value){ 
        return Maybe<T>::of(value);
    }

    template<typename T>
    Maybe<T> Nothing(){
        return Maybe<T>::nothing();
    }

    template<typename T>
    const T& getValue(const Maybe<T>& m){
        return m.getValue();
    }

    template<typename F, typename T, typename R=decltype( std::declval<F>()(std::declval<T>()) ) >
    Maybe<R> map(const Maybe<T>& m, const F& f){
        return m.map(f);
    }

    template<typename F, typename T, typename R >
    Maybe<R> map(const Maybe<T>& m, const std::function<R(T)>& f){
        return m.map(f);
    }

    template<typename F, typename T, typename MR=decltype( std::declval<F>()(std::declval<T>()) ) >
    MR flatMap( const Maybe<T>& m, const F& f){
        return m.flatMap(f);
    }

    template<typename T, typename R >
    Maybe<R> flatMap( const Maybe<T>& m, const std::function<Maybe<R>(T)>& f){
        return m.flatMap(f);
    }

    template<typename T>
    const T& getOrElse(const Maybe<T>& m, const T& elseValue){
        return m.getOrElse(elseValue);
    }

    template<typename T>
    const bool isNothing(const Maybe<T>& m){
        return m.isNothing();
    }

    template<typename T>
    const bool isSomthing(const Maybe<T>& m){
        return m.isSomthing();
    }




