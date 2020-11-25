#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include "signal_wrapper.h"
#include <iostream>
#include <algorithm>
#include "either.h"
#include "error.h"

// struct FakeHash{
//     template<typename A>
//     size_t operator()(const A& f){
//         size_t res = 17;
//         res *= 31+std::hash<int>()(f.type);
//         switch (f.type)
//         {
//         case A::t_int:
//             res *= 31+std::hash<int>()(f.as_integer);
//             break;
//         case A::t_float:
//             res *= 31+std::hash<float>()(f.as_float);
//             break;
//         case A::t_bool:
//             res *= 31+std::hash<bool>()(f.as_bool);
//             break;
//         case A::t_string:
//             res *= 31+std::hash<std::string>()(f.as_string);
//             break;
//         case A::t_list:
//             res *= 31+std::hash<std::vector<A>>()(f.as_list);
//             break;
//         // case A::t_map:
//         //     res *= 31+std::hash<std::unordered_map<A,A,FakeHash>>()(f.as_map);
//         //     break;      
//         default:
//             break;
//         }
//         return res;
//     }
// };



struct SporeDataVariant{
    enum {t_int, t_float, t_bool, t_string, t_list, t_map, t_signal, t_lambda, t_unknown} type;
    union{
        int as_integer;
        double as_float;
        bool as_bool;
        std::string as_string;
        std::vector<SporeDataVariant> as_list;
        Signal<double,int> as_signal;
        std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)> as_lambda;
    };
    ~SporeDataVariant(){
        if(type==t_string){
            using std::string;
            as_string.~string();
        }
        if(type == t_list){
            using std::vector;
            as_list.~vector<SporeDataVariant>();
        }
        if(type == t_lambda){
            using std::function;
            as_lambda.~function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>();
        }
        if(type == t_signal){
            as_signal.~Signal<double,int>();
        }
    }
    SporeDataVariant():type(t_unknown){}
    SporeDataVariant(const SporeDataVariant& f)
    :SporeDataVariant(){
        type = f.type;
        switch (type)
        {
        case t_int:
            as_integer=int(f.as_integer);
            break;
         case t_float:
            as_float=double(f.as_float);
            break;
        case t_bool:
            as_bool=bool(f.as_bool);
            break;
        case t_string:
            // new (&as_string) std::string(f.as_string);
            new (&as_string) std::string();
            as_string = std::string(f.as_string);
            break;
        case t_list:
            new (&as_list) std::vector<SporeDataVariant>();
            // for(auto el:f.as_list){as_list.push_back(el);}
            std::copy(f.as_list.begin(), f.as_list.end(), back_inserter(as_list) );
            break;
        case t_lambda:
            new (&as_lambda) std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>();
            as_lambda = f.as_lambda;
            break;
        case t_signal:
            new (&as_signal) Signal<double,int>(f.as_signal);
            break;

        default:
            break;
        }
    }
    // SporeDataVariant(SporeDataVariant&& s){
    //     swapVariant(*this,s);
    // }

    SporeDataVariant& operator =(SporeDataVariant f){

        // swapVariant(*this, f);
        // return *this;
        if((*this)!=f){
             type = f.type;
        switch (type)
            {
            case t_int:
                as_integer = f.as_integer;
                break;
            case t_float:
                as_float = f.as_float;
                break;
            case t_bool:
                as_bool = f.as_bool;
                break;
            case t_string:
                new (&as_string) std::string(f.as_string);
                break;
            case t_list:
                new (&as_list) std::vector<SporeDataVariant>(f.as_list);
                break;
            case t_lambda:
                new (&as_lambda) std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>(f.as_lambda);
                break;
            case t_signal:
                new (&as_signal) std::function<double(int)>(f.as_signal);
                break;            
            default:
                break;
            }
        }
       
        return *this;
    }
    friend void swapVariant(SporeDataVariant& s1, SporeDataVariant& s2){
        std::swap(s1.type,s2.type);
        switch (s1.type)
        {
        case t_int:
            std::swap(s1.as_integer,s2.as_integer);
            break;
        case t_bool:
            std::swap(s1.as_bool,s2.as_bool);
            break;
        case t_float:
            std::swap(s1.as_float,s2.as_float);
            break;
        case t_list:
            std::swap(s1.as_list,s2.as_list);
            break;
        case t_lambda:
            std::swap(s1.as_lambda,s2.as_lambda);
            break;
        case t_signal:
            std::swap(s1.as_signal,s2.as_signal);
            break;
        default:
            break;
        }
    }

    SporeDataVariant(int v):type(t_int),as_integer(v){}
    SporeDataVariant(double v):type(t_float),as_float(v){}
    SporeDataVariant(bool v):type(t_bool),as_bool(v){}
    SporeDataVariant(std::string v)
    :type(t_string){
        new (&as_string)std::string(v);
    }
    SporeDataVariant(const char v[] )
    :type(t_string){
        new (&as_string) std::string(v);
    }
    SporeDataVariant(const std::vector<SporeDataVariant> v)
    :type(t_list){
        new (&as_list) std::vector<SporeDataVariant>();
        for(auto el:v){
            as_list.push_back(el);
        }
    }
    SporeDataVariant(const Signal<double,int>& v):type(t_signal){
        new (&as_signal) Signal<double,int>(v);

    }
    SporeDataVariant(const std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>& v)
    :type(t_lambda){
        new (&as_lambda) std::function<Either<SporeError,SporeDataVariant>(SporeDataVariant)>(v);
    }

    bool operator == (const SporeDataVariant& f) const {
        bool areEqual = type==f.type;
        if(areEqual){
            switch (type)
            {
            case t_int:
                areEqual= as_integer == f.as_integer;
                break;
            case t_float:
                areEqual= as_float == f.as_float;
                break;
            case t_bool:
                areEqual= as_bool == f.as_bool;
                break;
            case t_string:
                areEqual= as_string == f.as_string;
                break;
            case t_list:
                areEqual= as_list == f.as_list;
                break;
            case t_lambda:
                areEqual= false;
                break;
            case t_signal:
                areEqual = false;
            case t_unknown:
                areEqual = false;
            default:
                break;
            }
        }
        return areEqual;

    }
    bool operator != (const SporeDataVariant& s){
        return !(*this==s);
    }

    Either<SporeError, SporeDataVariant> operator * (const SporeDataVariant& s){
        return multiply(*this, s);
    }

    friend Either<SporeError, SporeDataVariant> multiply(const SporeDataVariant& left,const SporeDataVariant& right){
        using E = Either<SporeError, SporeDataVariant>;
        auto intByX = [&](){
            int a = left.as_integer;
            switch (right.type)
            {
            case t_int:
                return E(SporeDataVariant(a*right.as_integer));
            case t_float:
                return E(SporeDataVariant(a*right.as_float));
            case t_bool:
                return E(SporeDataVariant(a*right.as_bool));
            default:
                break;
            }
            return E(SporeError("unsupported operation: int * "+typeToString(right)));

        };
        auto floatByX = [&](){
            int a = left.as_float;
            switch (right.type)
            {
            case t_int:
                return E(SporeDataVariant(a*right.as_integer));
            case t_float:
                return E(SporeDataVariant(a*right.as_float));
            case t_bool:
                return E(SporeDataVariant(a*right.as_bool));
            default:
                break;
            }
            return E(SporeError("unsupported operation: float * "+typeToString(right)));

        };
        auto boolByX = [&](){
            bool a = left.as_bool;
            switch (right.type)
            {
            case t_int:
                return E(SporeDataVariant(a*right.as_integer));
            case t_float:
                return E(SporeDataVariant(a*right.as_float));
            case t_bool:
                return E(SporeDataVariant(a*right.as_bool));

            default:
                break;
            }
            return E(SporeError("unsupported operation: bool * "+typeToString(right)));

        };
        // auto stringByX = [&](){
        //    return E(SporeError("unsupported operation"));
        // };
        // auto listByX = [&](){
        //     // auto result = left.as_list;
        //     // for_each(result.begin(), result.end(), [&](auto el){return multiply(el,right);});
        //     // return result;
        //     return E(SporeError("unsupported operation"));
        // };
        switch (left.type)
        {
        case t_int:
            return intByX();
        case t_float:
            return floatByX();
        case t_bool:
            return boolByX();
 
        default:
            break;
        }

        return E(SporeError("unsupported operation"));
    }

    friend std::string typeToString(SporeDataVariant d){
        switch (d.type)
        {
        case SporeDataVariant::t_int:
            return "INT";
            break;
        case SporeDataVariant::t_float:
            return "FLOAT";
            break;
        case SporeDataVariant::t_bool:
            return "FLOAT";
            break;
        case SporeDataVariant::t_list:
            return "LIST";
            break;
        case SporeDataVariant::t_string:
            return "STRING";
            break;
        case SporeDataVariant::t_signal:
            return "SIGNAL";
            break;
        case SporeDataVariant::t_lambda:
            return "FUNCTION";
            break;
        case SporeDataVariant::t_unknown:
            return "UNKNOWN";
            break;
        
        default:
            break;
        }

        return "UNKNOWN";
}

};


