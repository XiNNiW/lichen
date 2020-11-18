#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

// template <typename INTEGER, typename FLOAT, typename STRING, typename SIGNAL>

struct SporeDataVariant {
    using INTEGER = int;
    using FLOAT = double;
    using STRING = std::string;
    using SIGNAL = std::function<double(int)>;
    using LAMBDA = std::function<SporeDataVariant(SporeDataVariant)>;
    using LIST = std::vector<SporeDataVariant>;
    using MAP = std::map<SporeDataVariant,SporeDataVariant>;
    enum SPORE_TYPE {t_int, t_float, t_string, t_signal, t_lambda, t_list, t_map, t_unknown} type;  
    union {
        INTEGER as_integer;
        FLOAT as_float;
        STRING as_string;
        // SIGNAL as_signal;
        // LAMBDA as_lambda;
        // LIST as_list;
       // MAP as_map;
    };
    friend void swap(SporeDataVariant& v1, SporeDataVariant& v2){
        std::cout <<"in swap fmethod" << std::endl;
        using std::swap;

        swap(v1.type,v2.type);
        switch(v1.type){
            case t_int:
                swap(v1.as_integer,v2.as_integer);
                break;
            case t_float:
                swap(v1.as_float, v2.as_float);
                break;
            case t_string:
                swap(v1.as_string, v2.as_string);
                break;
            // case t_signal:
            //     swap(v1.as_signal, v2.as_signal);
            //     break;
            // case t_lambda:
            //     swap(v1.as_lambda, v2.as_lambda);
            //     break;
            // case t_list:
            //     swap(v1.as_list, v2.as_list);
            //     break;
            // case t_map:
            //     swap(v1.as_map,v2.as_map);
            //     break;
            case t_unknown:
                break;
            default:
                assert(false);
        }
    }
    SporeDataVariant():type(t_unknown){
        std::cout << "in default cons..."<<std::endl;

    }
    //1. DESTRUCTOR
    ~SporeDataVariant() {
        switch (type){
            case t_int:
                as_integer.~INTEGER();
                break;
            case t_float:
                as_float.~FLOAT();
                break;
            case t_string:
                as_string.~STRING();
                break;
            // case t_signal:
            //     as_signal.~SIGNAL();
            //     break;
            // case t_lambda:
            //     as_lambda.~LAMBDA();
            //     break;
            // case t_list:
            //     as_list.~LIST();
            //     break;
            // case t_map:
            //     as_map.~MAP();
            //     break;
            case t_unknown:
                break;
            default:
                assert(false);

        }
        
    }
    //2. COPY
    // SporeDataVariant(const SporeDataVariant& v) = delete;
    SporeDataVariant(SporeDataVariant& v) noexcept
    // :SporeDataVariant()
    {
        std::cout<<"input value in copy constructor now has: "<<v.as_string<<std::endl;
        // swap(*this, v); 
        // *this=v;
        type = v.type;
        switch(type){
            case t_int:
                new (&as_integer) INTEGER(v.as_integer);
                break;
            case t_float:
                new (&as_float) FLOAT(v.as_float);
                break;
            case t_string:
                new (&as_string) STRING(v.as_string);
                break;
            // case t_signal:
            //     new (&as_signal) SIGNAL(v.as_signal);
            //     break;
            // case t_lambda:
            //     new (&as_lambda) LAMBDA(v.as_lambda);
            //     break;
            // case t_list:
            //     new (&as_list) LIST(v.as_list);
            //     break;
            // case t_map:
            //     new (&as_map) MAP(v.as_map);
            //     break;
            case t_unknown:
                break;
            default: 
                assert(false);
              

        }
    }
    //3. COPY ASSIGNMENT
    SporeDataVariant& operator = (SporeDataVariant v){
        std::cout << "in overloaded assignment"<<std::endl;
        swap(*this, v); 
        return *this;
    }
    //4. MOVE
    SporeDataVariant(SporeDataVariant&& v) noexcept
    {   
        type = std::move(v.type);
        switch (type)
        {
        case t_int:
            as_integer = v.as_integer;
            as_integer = 0;
            break;
        case t_float:
            as_float =  v.as_float;
            as_float =  0;
            break;
        case t_string:
            as_string =  v.as_string;
            as_string =  nullptr;
            break;
        // case t_signal:
        //     as_signal =  std::move(v.as_signal);
        //     break;
        // case t_lambda:
        //     as_lambda =  std::move(v.as_lambda);
        //     break;
        // case t_list:
        //     as_list =  std::move(v.as_list);
        //     break;
        // case t_map:
        //     as_map =  std::move(v.as_map);
        //     break;
        default:
            break;
        }
        
    }
    // //5. MOVE ASSIGNMENT
    // SporeDataVariant operator = (SporeDataVariant&& v){
        
    //     swap(*this,v);
    //     return *this;
    // }
    explicit SporeDataVariant(const INTEGER& value)
    :type(t_int)
    ,as_integer(value)
    {
        std::cout << "making int... " << as_integer << std::endl;
    }

    explicit SporeDataVariant(const FLOAT& value)
    :type(t_float)
    // {new (&as_float) FLOAT(value);}
    ,as_float(value)
    {
        std::cout << "making float... " << as_float << std::endl;

    }
    
    explicit SporeDataVariant(const STRING& value)
    :type(t_string)
    {
        new (&as_string) STRING(value);
        std::cout<<"object now has: "<<as_string<<std::endl;

    }

    // explicit SporeDataVariant(const SIGNAL& value)
    // :type(t_signal)
    // // ,as_signal(value)
    // {
    //     new (&as_signal) SIGNAL(value);
    //     std::cout << "making signal... " << as_signal(0) << std::endl;

    // }

    // explicit SporeDataVariant(const LAMBDA& value)
    // :type(t_lambda)
    // // ,as_lambda(value)
    // {
    //     new (&as_lambda) LAMBDA(value);
    //     std::cout << "making lambda..." << std::endl;
    // }

    // explicit SporeDataVariant(const LIST& value)
    // :type(t_list)
    // // ,as_list(value)
    // {
    //     new (&as_list) LIST(value);
    //     std::cout << "making list..." << std::endl;

    // }

    // explicit SporeDataVariant(const MAP& value)
    // :type(t_map)
    // // ,as_map(value)
    // {
    //     new (&as_map) MAP(value);
    //     std::cout << "making map..." << std::endl;

    // }

    // bool operator == (const SporeDataVariant& v){
    //     std::cout<< "evaluating equality"<<std::endl;
    //     bool areEqual = false;
    //     bool typesMatch = type == v.type;
    //     if (typesMatch){
    //         switch (type){
    //             case t_int:
    //                 areEqual = as_integer == v.as_integer;
    //                 break;
    //             case t_float:
    //                 areEqual = as_float == v.as_float;
    //                 break;
    //             case t_string:
    //                 areEqual = as_string == v.as_string;
    //                 break;
    //             case t_signal:
    //                 areEqual = as_signal == v.as_signal;
    //                 break;
    //             case t_lambda:
    //                 areEqual = as_lambda == v.as_lambda;
    //                 break;
    //             case t_list:
    //                 areEqual = as_list == v.as_list;
    //                 break;
    //             case t_map:
    //                 areEqual = as_map == v.as_map;
    //                 break;
    //             case t_unknown:
    //                 break;
    //             default:
    //                 assert(false);

    //         }
    //     } 

    //     return areEqual;
    // }

};