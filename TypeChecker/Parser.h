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
#include <iostream>



using namespace std;

LexicalAnalyzer lexer;
class scope;
class symbol;
class reference;
symbol* lookup_inlocal(symbol s,scope *scope);
symbol* lookup_inlocal(reference s,scope *scope);

scope *currentscope=nullptr;
string error;
string output;
string unintalized;
string type;

void syntaxerror(){
    cout<<"Syntax Error";
    exit(1);
}

void isBuiltInType(Token y){
   if(y.lexeme== "INT" || y.lexeme== "REAL"||y.lexeme== "BOOLEAN"||y.lexeme== "STRING"){
       syntaxerror();
      }
}

struct boolvalue{
    bool value=false;
};

class scope {
public:
    vector <symbol*> symboltable;
    scope *parent;
};

class symbol{
public:
    int linedeclared;
    vector<reference*> *allrefs;
    string name;
    TokenType declaredType;
    bool isInitalized=false;
    scope *scope_;
    symbol *nextsymbol;
    
    symbol(){
        
    }
    symbol(Token t){
        linedeclared=t.line_no;
        name=t.lexeme;
        allrefs=new vector<reference*>;
    }
    
    
};




class reference{
public:
    int line;
    string side;
    TokenType reftype;
    string name;
    reference(){
        
    }
    reference(int lineno, string name2){
        name=name2;
        line=lineno;
    }
};


scope* whileLoopScope(scope *currentScope, scope *newscope){

    for(int u=0;u<currentScope->symboltable.size();u++){
       // reference *r=new reference();
        newscope->symboltable.push_back(new symbol());
        newscope->symboltable[u]->isInitalized=currentScope->symboltable[u]->isInitalized;
        newscope->symboltable[u]->allrefs=currentScope->symboltable[u]->allrefs;
        newscope->symboltable[u]->name=currentScope->symboltable[u]->name;
        newscope->symboltable[u]->declaredType=currentScope->symboltable[u]->declaredType;
        newscope->symboltable[u]->linedeclared=currentScope->symboltable[u]->linedeclared;
    }
    return newscope;
}


symbol* lookup(scope *scope,symbol *s){
    if(scope!=nullptr){
        if(lookup_inlocal(*s,scope) != nullptr){
            return  lookup_inlocal(*s, scope);
        }
        else{
            return lookup(scope->parent,s);
        }
        
    } else {
        return nullptr;
    }
}

symbol* lookup(scope *scope,reference *s){
    if(scope!=nullptr){
        if(lookup_inlocal(*s,scope) != nullptr){
            return  lookup_inlocal(*s, scope);
        }
        else{
            return lookup(scope->parent,s);
        }
        
    } else {
        return nullptr;
    }
}

symbol* lookup_inlocal(reference s,scope *scope){ //looking up a reference and checking if it exists as a decl
    symbol *returnvalue=nullptr;
    for(int i=0; i<scope->symboltable.size(); i++){
        if(s.name==scope->symboltable[i]->name){
            returnvalue=scope->symboltable[i];
        }
        
    }
    return returnvalue;
}


symbol* lookup_inlocal(symbol s,scope *scope){ //looking up a reference and checking if it exists as a decl
    symbol *returnvalue=nullptr;
    for(int i=0; i<scope->symboltable.size(); i++){
        if(s.name==scope->symboltable[i]->name){
            returnvalue=scope->symboltable[i];
        }
        
    }
    return returnvalue;
}

void addto_output(scope *currentscope,symbol *s,reference *r){
 
   

    output.append(s->name); output.append(" "); output.append(std::to_string(r->line)); output.append(" "); output.append(std::to_string(s->linedeclared));output.append("\n");
    
}






class Parser {
public:
    
    Token expect(TokenType expected_type);
    Token peek();
    void syntax_error();
    void parse_boolconst();
    TokenType parse_primary();
    TokenType parse_operator();
    TokenType parse_expr();
    void parse_condition();
    void parse_whilestmt();
    void parse_stmt();
    void parse_stmt_list();
    void parse_assignstmt();
    void parse_typename();
    symbol* parse_idlist();
    void parse_vardecl();
    void parse_scope();
    void parse_scope_list();
    void parse_program();
    TokenType type_checker(TokenType op,TokenType type1, TokenType type2){
        if(op==PLUS || op == MULT || op== MINUS || op==DIV){
            if((type1==INT || type1==REAL) && (type2==INT || type2==REAL)){
                if(type1==INT && type2==INT && op==DIV){
                    return REAL;
                }
               else if(type1 == REAL || type2 == REAL){
                    return REAL;
                }
                else if(type1 == INT && type2 == INT){
                    return INT;
                }
            }
                else {
                    type.append("TYPE MISMATCH "); type.append(std::to_string(lexer.get_line_no())); type.append(" C3"); type.append("\n");
                    return ERROR;
                }
            
        }
        
        
        else if(op==AND || op == OR || op== XOR ){
            if(type1==BOOLEAN && type2==BOOLEAN){
                return BOOLEAN;
            }else {
                 type.append("TYPE MISMATCH "); type.append(std::to_string(lexer.get_line_no())); type.append(" C4");type.append("\n");
                return ERROR;
            }
        }
        else if(op==GREATER || op == GTEQ || op== LESS || op==NOTEQUAL || op==LTEQ){
            if((type1==INT || type1==REAL)){
                if(type2==INT || type2==REAL){
                    return BOOLEAN;}
                else {
                     type.append("TYPE MISMATCH "); type.append(std::to_string(lexer.get_line_no())); type.append(" C6");type.append("\n");
                }
            }
            else if((type1==STRING && type2==STRING) || (type1==BOOLEAN && type2==BOOLEAN)){
                return BOOLEAN;
                }
            else {
                type.append("TYPE MISMATCH "); type.append(std::to_string(lexer.get_line_no())); type.append(" C5");type.append("\n");
                return ERROR;
            }
            
        }
                                                                                                                                
        return ERROR;
    }
private:
};
