#pragma once
#include <map>
#include <string>
#include "symbol.h"

struct ScopedSymbolTable{
    std::map<std::string, SymbolVariant> symbols;
    std::string scopeName;
    int scopeLevel;
    ScopedSymbolTable* enclosingScope = nullptr;

    ScopedSymbolTable(std::string name, int scopeLevel)
    :symbols(std::map<std::string,SymbolVariant>())
    ,scopeName(name)
    ,scopeLevel(scopeLevel)
    {}

    ScopedSymbolTable(std::string name, int scopeLevel, ScopedSymbolTable* enclosingScope)
    :symbols(std::map<std::string,SymbolVariant>())
    ,scopeName(name)
    ,scopeLevel(scopeLevel)
    ,enclosingScope(enclosingScope)
    {}
    void insert(SymbolVariant symbol){
        setScopeLevel(symbol,scopeLevel);
        symbols[getSymbolName(symbol)] = symbol;
    }


    SymbolVariant lookupInCurrentScopeOnly(std::string name){
        auto keyIterator = symbols.find(name);
        bool foundIt = keyIterator!=symbols.end();
        SymbolVariant symbol = SymbolVariant::voidSymbol();
        if(foundIt){
            symbol = symbols[name];
        }
        return symbol;
    }
    SymbolVariant lookup(std::string name){
        SymbolVariant symbol = lookupInCurrentScopeOnly(name);
        bool foundIt = symbol.type!=SymbolVariant::t_unknown;
        
        if(!foundIt && enclosingScope ){
            auto keyIterator = enclosingScope->symbols.find(name);
            foundIt = keyIterator!=enclosingScope->symbols.end();
            if(foundIt){
                symbol = symbols[name];
            }
        }
        return symbol;
    }
};