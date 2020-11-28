#pragma once
#include <string>
#include <map>
#include "data_varient.h"
#include "maybe.h"
#include <iostream>

struct ActivationRecord {
    std::string name;
    enum Type {t_function_call, t_program} type;
    int nestingLevel;
    std::map<std::string, SporeDataVariant> members;

    ActivationRecord(std::string name, Type type, int nestingLevel)
    :name(name),type(type),nestingLevel(nestingLevel),members(std::map<std::string, SporeDataVariant>())
    {}
 
    Maybe<SporeDataVariant> get(std::string name){
        if(members.find(name)!=members.end()){
            return Maybe<SporeDataVariant>::of(members[name]);
        } else {
            return Maybe<SporeDataVariant>::nothing();
        };
    }

    void set(std::string name, const SporeDataVariant& value){
        std::cout<<"set called\n";
        members[name] = value;
    }
    
};