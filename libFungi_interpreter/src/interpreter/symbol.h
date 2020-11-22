#pragma once
#include "../lang/sporelang_syntaxtree.h"
#include <iostream>

struct BuiltInSymbol{
    std::string name="";
    int scopeLevel;
    BuiltInSymbol(const BuiltInSymbol& b)=default;
    BuiltInSymbol(const std::string& name)
    :name(name)
    {}
    BuiltInSymbol(const std::string& name, const int& scopeLevel)
    :name(name),scopeLevel(scopeLevel)
    {}
    bool operator == (const BuiltInSymbol& b){
        return scopeLevel == b.scopeLevel && name==b.name;
    }

};
struct DataVariableSymbol{
    std::string name="";
    int scopeLevel;

    DataVariableSymbol(const DataVariableSymbol& d)=default;
    DataVariableSymbol(const std::string& name)
    :name(name)
    {
       // std::cout << "constructing Data with name only";

    }
    DataVariableSymbol(const int& scopeLevel)
    :scopeLevel(scopeLevel),name("")
    {}
    DataVariableSymbol(const std::string& name, const int& scopeLevel)
    :name(name),scopeLevel(scopeLevel)
    {}
    bool operator == (const DataVariableSymbol& b){
        return scopeLevel == b.scopeLevel && name==b.name;
    }


};
struct IdentifierSymbol{
    std::string name="";
    IdentifierSymbol(const std::string& name):name(name){}
    bool operator == (const IdentifierSymbol& id){
        return name==id.name;
    }
};
struct ArgumentsListSymbol{
    std::vector<std::string> list;//=std::vector<std::string>();
    ArgumentsListSymbol(const std::vector<std::string>& formalParams){
        // std:: cout << "in args cons";
        list = formalParams;
    }
    // ArgumentsListSymbol(const ArgumentsListSymbol& a)=default;
    // ArgumentsListSymbol(const ArgumentsListSymbol&& a)=default;
    // void operator =(const ArgumentsListSymbol& a) = default;
    // void operator =(const ArgumentsListSymbol&& a) = default;
    bool operator == (const ArgumentsListSymbol& args){
        return list==args.list;
    }
};
struct FunctionVariableSymbol{
    std::string name="";
    int scopeLevel;
    std::vector<std::string> formalParams=std::vector<std::string>();
    BlockNode block=BlockNode(nullptr);
    FunctionVariableSymbol(const FunctionVariableSymbol& f)=default;
    FunctionVariableSymbol(const std::string& name, const std::vector<std::string>& formalParams, const BlockNode& block)
    :name(name),formalParams(formalParams),block(block)
    {}
    FunctionVariableSymbol(const std::string& name, const int& scopeLevel, const std::vector<std::string>& formalParams, const BlockNode& block)
    :name(name),scopeLevel(scopeLevel),formalParams(formalParams),block(block)
    {}
    FunctionVariableSymbol(const int& scopeLevel, const std::vector<std::string>& formalParams)
    :name(""),scopeLevel(scopeLevel),formalParams(formalParams),block(nullptr){}
    FunctionVariableSymbol(const std::vector<std::string>& formalParams)
    :name(""),formalParams(formalParams),block(nullptr){}
    FunctionVariableSymbol(const std::vector<std::string>& formalParams,const BlockNode& block)
    :name(""),formalParams(formalParams),block(block){}

    bool operator == (const FunctionVariableSymbol& b){
        // should compare block node too??
        return  
            scopeLevel == b.scopeLevel 
            && name==b.name 
            && formalParams == b.formalParams;
        
    }

};

struct VoidSymbol{};

struct SymbolVariant{
    static SymbolVariant voidSymbol(){return SymbolVariant();}
    enum {t_built_in, t_data_variable, t_function_variable, t_identifier, t_arguments_list, t_unknown} type;
    union {
        BuiltInSymbol as_built_in;
        DataVariableSymbol as_data_var;
        FunctionVariableSymbol as_func_var;
        IdentifierSymbol as_identifier;
        ArgumentsListSymbol as_arg_list;
    };
    SymbolVariant():type(t_unknown){}
    ~SymbolVariant(){
        switch (type)
        {
        case t_built_in:
            as_built_in.~BuiltInSymbol();
            break;
        case t_data_variable:
            as_data_var.~DataVariableSymbol();
            break;
        case t_function_variable:
            as_func_var.~FunctionVariableSymbol();
            break;
        case t_identifier:
            as_identifier.~IdentifierSymbol();
            break;
        case t_arguments_list:
            as_arg_list.~ArgumentsListSymbol();
            break;
        default:
            break;
        }
    }
    // SymbolVariant(SymbolVariant&& s):SymbolVariant(){
    //     swapVariant(*this,s);
    // }
    SymbolVariant(const SymbolVariant& s)
    :SymbolVariant()
    {
        std::cout << "in symbol variant cons"<<std::endl;
        type = s.type;
        switch (type)
        {
        case t_built_in:
            std::cout << "copying built in..."<<std::endl;
            new (&as_built_in) BuiltInSymbol(s.as_built_in);
            break;
        case t_data_variable:
            std::cout << "copying data..."<<std::endl;
            new (&as_data_var) DataVariableSymbol(s.as_data_var);
            break;
        case t_function_variable:
            std::cout << "copying func..."<<std::endl;
            new (&as_func_var) FunctionVariableSymbol(s.as_func_var);
            break;
        case t_identifier:
            std::cout << "copying id..."<<std::endl;
            new (&as_identifier) IdentifierSymbol(s.as_identifier);
            break;
        case t_arguments_list:
            std::cout << "copying args..."<<std::endl;
            new (&as_arg_list) ArgumentsListSymbol(s.as_arg_list);
            break;
        
        default:
            break;
        }
    }
    SymbolVariant& operator = (SymbolVariant s){
        type = s.type;
        switch (type)
        {
        case t_built_in:
            std::cout << "copying built in..."<<std::endl;
            new (&as_built_in) BuiltInSymbol(s.as_built_in);
            break;
        case t_data_variable:
            std::cout << "copying data..."<<std::endl;
            new (&as_data_var) DataVariableSymbol(s.as_data_var);
            break;
        case t_function_variable:
            std::cout << "copying func..."<<std::endl;
            new (&as_func_var) FunctionVariableSymbol(s.as_func_var);
            break;
        case t_identifier:
            std::cout << "copying id..."<<std::endl;
            new (&as_identifier) IdentifierSymbol(s.as_identifier);
            break;
        case t_arguments_list:
            std::cout << "copying args..."<<std::endl;
            new (&as_arg_list) ArgumentsListSymbol(s.as_arg_list);
            break;
        
        default:
            break;
        }
        // swapVariant(*this, s);
        return *this;
    }
    // friend void swapVariant(SymbolVariant& s1, SymbolVariant& s2) noexcept{
    //     std::swap(s1.type,s2.type);
    //     switch (s1.type)
    //     {
    //     case t_built_in:
    //         std::swap(s1.as_built_in, s2.as_built_in);
    //         break;
    //     case t_data_variable:
    //         std::swap(s1.as_data_var, s2.as_data_var);
    //         break;
    //     case t_function_variable:
    //         std::swap(s1.as_func_var, s2.as_func_var);
    //         break;
    //     case t_identifier:
    //         std::swap(s1.as_identifier,s2.as_identifier);
    //         break;
    //     case t_arguments_list:
    //         std::swap(s1.as_arg_list,s2.as_arg_list);
    //         break;
        
    //     default:
    //         break;
    //     }
    // }
    SymbolVariant(const BuiltInSymbol& b)
    :type(t_built_in)
    {
        new (&as_built_in) BuiltInSymbol(b);
    }
    SymbolVariant(const DataVariableSymbol& d)
    :type(t_data_variable)
    {
        std::cout << "constructing SymbolVariant for Data\n";
        new (&as_data_var) DataVariableSymbol(d);
    }
    SymbolVariant(const FunctionVariableSymbol& f)
    :type(t_function_variable)
    {
        std::cout << "constructing SymbolVariant for FUNK\n";

        new (&as_func_var) FunctionVariableSymbol(f);
    }
    SymbolVariant(const IdentifierSymbol& id)
    :type(t_identifier)
    {
        new (&as_identifier) IdentifierSymbol(id);
    }
    SymbolVariant(const ArgumentsListSymbol& args)
    :type(t_arguments_list)
    {
        std::cout << "constructing SymbolVariant for args\n";

        new (&as_arg_list) ArgumentsListSymbol(args);
    }
    friend void setScopeLevel(SymbolVariant& s, int scopeLevel){
        switch (s.type)
        {
        case t_built_in:
            s.as_built_in.scopeLevel = scopeLevel;
            break;
        case t_data_variable:
            s.as_data_var.scopeLevel = scopeLevel;
            break;
        case t_function_variable:
            s.as_func_var.scopeLevel = scopeLevel;
            break;

        default:
            break;
        }
    }
    friend void setName(SymbolVariant& s, std::string name){
        switch (s.type)
        {
        case t_built_in:
            s.as_built_in.name = name;
            break;
        case t_data_variable:
            s.as_data_var.name = name;
            break;
        case t_function_variable:
            s.as_func_var.name = name;
            break;

        default:
            break;
        }
    }
    friend std::string getSymbolName(const SymbolVariant& s){
        std::string name = "";
        switch (s.type)
        {
        case t_built_in:
            name = s.as_built_in.name;
            break;
        case t_data_variable:
            name = s.as_data_var.name;
            break;
        case t_function_variable:
            name = s.as_func_var.name;
            break;
        default:
            break;
        }
        return name;
    }
    bool operator == (const SymbolVariant& s){
        bool areEqual = type == s.type;
        if(areEqual){
            switch (type)
            {
            case t_built_in:
                areEqual = as_built_in == s.as_built_in;
                break;
            case t_data_variable:
                areEqual = as_data_var == s.as_data_var;
                break;
            case t_function_variable:
                areEqual = as_func_var == s.as_func_var;
                break;
            case t_identifier:
                areEqual = as_identifier == s.as_identifier;
                break;
            case t_arguments_list:
                areEqual = as_identifier == s.as_identifier;
                break;
            
            default:
                break;
            }
        }
        return areEqual;
    }
    bool operator != (const SymbolVariant& s){
        return !(*this==s);
    }
};