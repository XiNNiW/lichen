#pragma once
#include <string>
#include <map>
#include "data_varient.h"
#include <iostream>

struct ActivationRecord {
    std::string name;
    enum Type {t_function_call, t_program} type;
    int nestingLevel;
    std::map<std::string, SporeDataVariant> members;

    ActivationRecord(std::string name, Type type, int nestingLevel)
    :name(name),type(type),nestingLevel(nestingLevel),members(std::map<std::string, SporeDataVariant>())
    {}
 
    SporeDataVariant get(std::string name){
        std::cout<<"get called\n";
        return members[name];
    }

    void set(std::string name, const SporeDataVariant& value){
        std::cout<<"set called\n";
        members[name] = value;
    }
    
};