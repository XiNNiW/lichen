#pragma once
#include <iostream>

template<typename L, typename R>
class Either {
    private:
        L _left;
        R _right;
        
        bool _isRight;

        
    public:
        Either<L,R>(const L& left) :_left(left),_isRight(false){}
        Either<L,R>(const R& right):_right(right),_isRight(true){}
        // Either<L,R>(const Either<L,R>& e):_isRight(e._isRight){
        //     std::cout<<"in either copy cons...\n";
        //     if(_isRight){
        //         _right =e.getRight();
        //     }else{
        //         _left =e.getLeft();
        //     }
        // }

        static Either<L,R> rightOf(const R& r){
            return Either<L,R>(r);
        }
        static Either<L,R> leftOf(const L& l){
            return Either<L,R>(l);
        }
        const R& getRight() const {return _right;}
        const L& getLeft() const {return _left;}
        bool isRight() const {return _isRight;}
        bool isLeft() const {return !_isRight;}
        
        template<typename F, typename EV=decltype(std::declval<F>()(std::declval<R>()))>
        EV flatMap(const F& f){
            return this->isRight()?f(_right):EV::leftOf(_left);
        }

        template<typename R2>
        Either<L,R2> flatMap(std::function<Either<L,R2>(R)> f){
            return this->isRight()?f(_right):Either<L,R2>::leftOf(_left);
        }




    
 
};

template<typename L, typename R>
Either<L,R> Right(const R& r){return Either<L,R>::rightOf(r);}

template<typename L, typename R>
Either<L,R> Left(const L& l){return Either<L,R>::leftOf(l);}

template<typename L,typename R, typename F, typename EV=decltype(std::declval<F>()(std::declval<R>()))>
EV flatMap(Either<L,R> e, F f){
    return e.flatMap(f);
}

template<typename L, typename R, typename R2>
Either<L,R2> flatMap(Either<L,R> e,std::function<Either<L,R2>(R)> f){
    return e.flatMap(f);
}

template<typename L, typename R>
const R& getRight(Either<L,R> e) {return e.getRight();}

template<typename L, typename R>
const L& getLeft(Either<L,R> e) {return e.getLeft();}

template<typename L, typename R>
const bool isRight(Either<L,R> e) {return e.isRight();}

template<typename L, typename R>
const bool isLeft(Either<L,R> e) {return e.isLeft();}
