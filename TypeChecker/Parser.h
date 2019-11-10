//
//  Assignment3.h
//  Assignment3
//
//  Created by Assad Yousuf on 11/1/19.
//  Copyright © 2019 Assad Yousuf. All rights reserved.
//
#include "lexer.h"
#include "inputbuf.h"
#include <set>
#include <algorithm>


LexicalAnalyzer lexer;

void isBuiltInType(Token y){
    if(y.lexeme== "INT" || y.lexeme== "REAL"||y.lexeme== "BOOLEAN"||y.lexeme== "STRING"){
        printf("Syntax Error");
         exit(1);
    }
}

class Parser {
public:
    
    Token expect(TokenType expected_type);
    Token peek();
    void syntax_error();
    void parse_boolconst();
    void parse_primary();
    void parse_operator();
    void parse_expr();
    void parse_condition();
    void parse_whilestmt();
    void parse_stmt();
    void parse_stmt_list();
    void parse_assignstmt();
    void parse_typename();
    void parse_idlist();
    void parse_vardecl();
    void parse_scope();
    void parse_scope_list();
    void parse_program();
    
    
private:
};


class symbol{
public:
    std::string name;
    std::string declaredtype;
    Token t;
    bool isUsed=false;
    symbol(){
        
    }
    symbol(Token ex,bool value){
        t=ex;
        isUsed=value;
    }
    symbol(std::string e){
        name=e;
    }
    void setToUsed(bool b){
        if(b)
        isUsed=b;
    }
    
    bool compare(symbol s1, symbol s2){
        if(s1.name==s2.name)
            return true;
            
        return false;
    }
    
    
};

class scope{
public:
    std::vector <symbol*> symboltable;
    scope *parent=nullptr; scope *child=nullptr;
    scope(){
        
    }
    
    symbol* lookup(symbol s,scope *scope){ //returns true for found false for not found
        symbol *returnvalue=nullptr;
        for(int i=0; i<scope->symboltable.size(); i++){
            symbol *pointer=scope->symboltable[i];
            if(s.compare(s,*pointer)){
                returnvalue=pointer;
            }
        }
        return returnvalue;
    }
    void addToTable(symbol *newsymbol){
        symboltable.push_back(newsymbol);
    }
    std::vector<symbol*>  getSymbolTable(){
        return symboltable;
    }
    symbol* varNotUsed(){
       
        for(int j=0;j<symboltable.size(); j++){
           if(!symboltable[j]->isUsed)
               return symboltable[j];
        }
        return nullptr;
    }
    
    
};



