#pragma once
#include <map>
#include <string>
#include "data_varient.h"
#include "maybe.h"

struct Library{
    std::map<std::string, SporeDataVariant> books = std::map<std::string, SporeDataVariant>();
    Library(){}
    Library(std::map<std::string, SporeDataVariant> bindings){
        load(bindings);
    }
    Maybe<SporeDataVariant> get(std::string name){
        return books.find(name)!=books.end()?
            Maybe<SporeDataVariant>::of(books[name]):
            Maybe<SporeDataVariant>::nothing();
    }
    void load(std::map<std::string, SporeDataVariant> bindings){
        books.insert(bindings.begin(),bindings.end());
    }
};