#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

template <typename INTEGER, typename FLOAT, typename STRING, typename SIGNAL>
struct SporeDataVariant {
    using LAMBDA = std::function<SporeDataVariant(SporeDataVariant)>;
    using LIST = std::vector<SporeDataVariant>;
    using MAP = std::map<SporeDataVariant,SporeDataVariant>;
    enum SPORE_TYPE {t_int, t_float, t_string, t_signal, t_lambda, t_list, t_map} type;  
    union {
        INTEGER as_integer;
        FLOAT as_float;
        STRING as_string;
        SIGNAL as_signal;
        LAMBDA as_lambda;
        LIST as_list;
        MAP as_map;
    } data;
    // SporeDataVariant(){
    //     type=t_int;
    //     new (&as_integer) INTEGER(0);
    // }
    //4. MOVE
    SporeDataVariant(const SporeDataVariant&& v)
    
    {   
        std::cout << "in move constructor";
        type =std::move(v.type);
        switch(type){
            case t_int:
                new (&as_integer) INTEGER(std::move(v.as_integer));
                break;
            case t_float:
                new (&as_float) FLOAT(std::move(v.as_float));
                break;
            case t_string:
                new (&as_string) STRING(std::move(v.as_string));
                std::cout << "value assigned: "<<as_string<<std::endl;
                break;
            case t_signal:
                new (&as_signal) SIGNAL(std::move(v.as_signal));
                break;
            case t_lambda:
                new (&as_lambda) LAMBDA(std::move(v.as_lambda));
                break;
            case t_list:
                new (&as_list) LIST(std::move(v.as_list));
                break;
            case t_map:
                new (&as_map) MAP(std::move(v.as_map));
                break;
            default: 
                std::cout<<"OH SHIT"<<std::endl;               
                break;

        }
    }
    //2. COPY
    SporeDataVariant(const SporeDataVariant& v){
        std::cout<<"input value in copy constructor now has: "<<v.as_string<<std::endl;

        type = v.type;
        switch(type){
            case t_int:
                // as_integer = INTEGER();
                new (&as_integer) INTEGER(v.as_integer);
                break;
            case t_float:
                // as_float = FLOAT();
                new (&as_float) FLOAT(v.as_float);
                break;
            case t_string:
                new (&as_string) STRING(v.as_string);
                break;
            case t_signal:
                new (&as_signal) SIGNAL(v.as_signal);
                break;
            case t_lambda:
                new (&as_lambda) LAMBDA(v.as_lambda);
                break;
            case t_list:
                new (&as_list) LIST(v.as_list);
                break;
            case t_map:
                new (&as_map) MAP(v.as_map);
                break;
            default: 
                std::cout<<"OH SHIT"<<std::endl;               
                break;

        }
    }
    SporeDataVariant(const INTEGER& value)
    :type(t_int)
    // {new (&as_integer) INTEGER(value);}
    ,as_integer(value)
    {
        // new (&as_integer) INTEGER(value);
        std::cout << "making int... " << as_integer << std::endl;
    }

    SporeDataVariant(const FLOAT& value)
    :type(t_float)
    // {new (&as_float) FLOAT(value);}
    ,as_float(value)
    {
        std::cout << "making float... " << as_float << std::endl;

    }
    
    SporeDataVariant(const STRING& value)
    :type(t_string)
    // {new (&as_string) STRING(value);
    //     std::cout<<"object now has: "<<as_string<<std::endl;
    // }
    ,as_string(value)
    // ,as_string(std::move(value))
    {
        std::cout<<"object now has: "<<as_string<<std::endl;

    }

    SporeDataVariant(const SIGNAL& value)
    :type(t_signal)
    // {new (&as_signal) SIGNAL(value);}
    ,as_signal(value)
    {
        std::cout << "making signal... " << as_signal(0) << std::endl;

    }

    SporeDataVariant(const LAMBDA& value)
    :type(t_lambda)
    // {new (&as_lambda) LAMBDA(value);}
    ,as_lambda(value)
    {
        std::cout << "making lambda..." << std::endl;
    }

    SporeDataVariant(const LIST& value)
    :type(t_list)
    // {new (&as_list) LIST(value);}
    ,as_list(value)
    {
        std::cout << "making list..." << std::endl;

    }

    SporeDataVariant(const MAP& value)
    :type(t_map)
    // {new (&as_map) MAP(value);}
    ,as_map(value)
    {
        std::cout << "making map..." << std::endl;

    }
//3. COPY ASSIGNMENT
    SporeDataVariant operator = (const SporeDataVariant& v){
        std::cout << "in overloaded assignment"<<std::endl;
        // new SporeDataVariant(v);
        type = v.type;
        switch(type){
            case t_int:
                // as_integer = INTEGER();
                new (&as_integer) INTEGER(v.as_integer);
                break;
            case t_float:
                // as_float = FLOAT();
                new (&as_float) FLOAT(v.as_float);
                break;
            case t_string:
                // as_string = STRING();
                std::cout << "in copy constructor"<<std::endl;
                std::cout << "it is: "<<v.as_string<<std::endl;
                // as_string = STRING(v.as_string);
                new (&as_string) STRING(v.as_string);
                std::cout << "value assigned: "<<as_string<<std::endl;
                break;
            case t_signal:
                new (&as_signal) SIGNAL(v.as_signal);
                break;
            case t_lambda:
                new (&as_lambda) LAMBDA(v.as_lambda);
                break;
            case t_list:
                // LIST temp = LIST(v.as_list.size());
                // std::copy(v.as_list.begin(),v.as_list.end(),temp);
                // as_list = SporeDataVariant(temp).as_list;
                new (&as_list) LIST(v.as_list);
                break;
            case t_map:
                new (&as_map) MAP(v.as_map);
                break;
            default:                
                break;
        }

        return *this;
        

        
    }
//5. MOVE ASSIGNMENT
    SporeDataVariant operator = (const SporeDataVariant&& v){
        std::cout << "in overloaded move op..."<<std::endl;
        // new SporeDataVariant(v);
        type = std::move(v.type);
        switch(type){
            case t_int:
                // as_integer = INTEGER();
                new (&as_integer) INTEGER(std::move(v.as_integer));
                break;
            case t_float:
                // as_float = FLOAT();
                new (&as_float) FLOAT(std::move(v.as_float));
                break;
            case t_string:
                // as_string = STRING();
                std::cout << "in copy constructor"<<std::endl;
                std::cout << "it is: "<<std::move(v.as_string)<<std::endl;
                // as_string = STRING(v.as_string);
                new (&as_string) STRING(v.as_string);
                std::cout << "value assigned: "<<std::move(as_string)<<std::endl;
                break;
            case t_signal:
                new (&as_signal) SIGNAL(std::move(v.as_signal));
                break;
            case t_lambda:
                new (&as_lambda) LAMBDA(std::move(v.as_lambda));
                break;
            case t_list:
                // LIST temp = LIST(v.as_list.size());
                // std::copy(v.as_list.begin(),v.as_list.end(),temp);
                // as_list = SporeDataVariant(temp).as_list;
                new (&as_list) LIST(std::move(v.as_list));
                break;
            case t_map:
                new (&as_map) MAP(std::move(v.as_map));
                break;
            default:                
                break;
        }

        return *this;
        

        
    }

    bool operator == (const SporeDataVariant& v){
        std::cout<< "evaluating equality"<<std::endl;
        bool areEqual = false;
        bool typesMatch = type == v.type;
        if (typesMatch){
            switch (type){
                case t_int:
                    areEqual = as_integer == v.as_integer;
                    break;
                case t_float:
                    areEqual = as_float == v.as_float;
                    break;
                case t_string:
                    areEqual = as_string == v.as_string;
                    break;
                case t_signal:
                    areEqual = as_signal == v.as_signal;
                    break;
                case t_lambda:
                    areEqual = as_lambda == v.as_lambda;
                    break;
                case t_list:
                    areEqual = as_list == v.as_list;
                    break;
                case t_map:
                    areEqual = as_map == v.as_map;
                    break;
                
                default:
                    break;
            }
        } 

        return areEqual;
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
            case t_signal:
                as_signal.~SIGNAL();
                break;
            case t_lambda:
                as_lambda.~LAMBDA();
                break;
            case t_list:
                as_list.~LIST();
                break;
            case t_map:
                as_map.~MAP();
                break;
            default:
                break;

        }
        
    }
};