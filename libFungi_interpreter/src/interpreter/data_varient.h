#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <string>
#include <functional>
#include <iostream>
#include "signal_wrapper.h"



struct FakeHash{
    template<typename A>
    size_t operator()(const A& f){
        size_t res = 17;
        res *= 31+std::hash<int>()(f.type);
        switch (f.type)
        {
        case A::t_int:
            res *= 31+std::hash<int>()(f.as_integer);
            break;
        case A::t_float:
            res *= 31+std::hash<float>()(f.as_float);
            break;
        case A::t_bool:
            res *= 31+std::hash<bool>()(f.as_bool);
            break;
        case A::t_string:
            res *= 31+std::hash<std::string>()(f.as_string);
            break;
        case A::t_list:
            res *= 31+std::hash<std::vector<A>>()(f.as_list);
            break;
        // case A::t_map:
        //     res *= 31+std::hash<std::unordered_map<A,A,FakeHash>>()(f.as_map);
        //     break;
        
        default:
            break;
        }
        return res;
    }
};

struct SporeDataVariant{
    enum {t_int, t_float, t_bool, t_string, t_list, t_map, t_signal, t_lambda, t_unknown} type;
    union{
        int as_integer;
        double as_float;
        bool as_bool;
        std::string as_string;
        std::vector<SporeDataVariant> as_list;
        Signal<double,int> as_signal;
        // std::unordered_map<SporeDataVariant,SporeDataVariant,FakeHash> as_map;
        std::function<SporeDataVariant(SporeDataVariant)> as_lambda;
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
        // if(type== t_map){
        //     using std::unordered_map;
        //     as_map.~unordered_map<SporeDataVariant,SporeDataVariant,FakeHash>();
        // }
        if(type == t_lambda){
            using std::function;
            as_lambda.~function<SporeDataVariant(SporeDataVariant)>();
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
            as_integer=f.as_integer;
            break;
         case t_float:
            as_float=f.as_float;
            break;
        case t_bool:
            as_bool=f.as_bool;
            break;
        case t_string:
            new (&as_string) std::string(f.as_string);
            break;
        case t_list:
            new (&as_list) std::vector<SporeDataVariant>();
            for(auto el:f.as_list){as_list.push_back(el);}
            break;
        // case t_map:
        //     new (&as_map) std::unordered_map<SporeDataVariant,SporeDataVariant,FakeHash>();
        //     for(const auto& el: f.as_map){
        //         as_map.insert(el);
        //     }
        //     break;
        case t_lambda:
            new (&as_lambda) std::function<SporeDataVariant(SporeDataVariant)>(f.as_lambda);
            break;
        case t_signal:
            new (&as_signal) Signal<double,int>(f.as_signal);
            break;
        default:
            break;
        }
    }
    SporeDataVariant& operator =(SporeDataVariant f) noexcept{
        swapUnion(*this,f);
        return *this;
    }

    SporeDataVariant(int v):type(t_int),as_integer(v){std::cout<<"int\n";}
    SporeDataVariant(double v):type(t_float),as_float(v){std::cout<<"float\n";}
    SporeDataVariant(bool v):type(t_bool),as_bool(v){std::cout<<"bool\n";}
    SporeDataVariant(std::string v)
    :type(t_string){
        new (&as_string)std::string(v);
        std::cout<<"string\n";
    }
    SporeDataVariant(const char v[] )
    :type(t_string){
        new (&as_string) std::string(v);
        std::cout<<"char*\n";
    }
    SporeDataVariant(const std::vector<SporeDataVariant> v)
    :type(t_list){
        new (&as_list) std::vector<SporeDataVariant>();
        for(auto el:v){
            as_list.push_back(el);
        }
        std::cout<<"vector\n";
    }
    SporeDataVariant(const Signal<double,int>& v):type(t_signal){
        new (&as_signal) Signal<double,int>(v);
        std::cout<<"signal\n";

    }
    SporeDataVariant(const std::function<SporeDataVariant(SporeDataVariant)>& v)
    :type(t_lambda){
        new (&as_lambda) std::function<SporeDataVariant(SporeDataVariant)>(v);
        std::cout<<"lambda\n";
    }

    bool operator == (const SporeDataVariant& f) const {
        bool areEqual = false;
        areEqual &= type==f.type;
        if(areEqual){
            switch (type)
            {
            case t_int:
                areEqual&= as_integer == f.as_integer;
                break;
            case t_float:
                areEqual&= as_float == f.as_float;
                break;
            case t_bool:
                areEqual&= as_bool == f.as_bool;
                break;
            case t_string:
                areEqual&= as_string == f.as_string;
                break;
            case t_list:
                areEqual&= as_list == f.as_list;
                break;
            // case t_map:
            //     areEqual&= as_map == f.as_map;
            //     break;
            case t_lambda:
                areEqual= false;
                break;
            case t_signal:
                areEqual = false;
            
            default:
                break;
            }
        }
        return areEqual;

    }
    

    friend void swapUnion(SporeDataVariant& f1, SporeDataVariant& f2) noexcept{
        std::cout<<"swap called\n";
        using std::swap;
        swap(f1.type,f2.type);
        switch (f1.type)
        {
        case t_int:
            swap(f1.as_integer, f2.as_integer);
            break;
        case t_float:
            swap(f1.as_float, f2.as_float);
            break;
        case t_bool:
            swap(f1.as_bool, f2.as_bool);
            break;
        case t_string:
            swap(f1.as_string, f1.as_string);
        case t_list:
            swap(f1.as_list, f1.as_list);
        case t_lambda:
            swap(f1.as_lambda, f1.as_lambda);
        // case t_map:
        //     swap(f1.as_map, f2.as_map);
        case t_signal:
            swap(f1.as_signal,f2.as_signal);
        default:
            break;
        }
    }

};
