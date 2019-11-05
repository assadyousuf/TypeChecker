/*
 * Copyright (C) Rida Bazzi, 2017
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */

#include "Parser.h"
#include <iostream>

void Parser::syntax_error(){
    std::cout<<"SYNTAX ERROR\n";
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

void Parser::parse_boolconst(){
    Token boolconst=peek();
    if(boolconst.token_type==FALSE|| boolconst.token_type==TRUE){
        boolconst=lexer.GetToken();
        return;
    }else{
        std::cout << "in parseboolconst\n";
        syntax_error();
        exit(0);
    }
}


void Parser::parse_primary(){
    Token primary=peek();
    switch (primary.token_type) {
        case BOOLEAN:
            primary=lexer.GetToken();
            return;
        case ID:
            primary=lexer.GetToken();
             return;
        case NUM:
            primary=lexer.GetToken();
             return;
        case REALNUM:
            primary=lexer.GetToken();
             return;
        case STRING_CONSTANT:
            primary=lexer.GetToken();
             return;
        default:
            std::cout << "in parseprimary\n";
            syntax_error();
            exit(0);
    }
   
}

void Parser::parse_operator(){
    Token operatorr=peek();
    switch (operatorr.token_type) {
        case PLUS:
            operatorr=lexer.GetToken();
            return;
        case MINUS:
            operatorr=lexer.GetToken();
            return;
        case MULT:
            operatorr=lexer.GetToken();
            return;
        case DIV:
            operatorr=lexer.GetToken();
            return;
        case AND:
            operatorr=lexer.GetToken();
            return;
        case OR:
            operatorr=lexer.GetToken();
            return;
        case XOR:
            operatorr=lexer.GetToken();
            return;
        case GREATER:
            operatorr=lexer.GetToken();
            return;
        case GTEQ:
            operatorr=lexer.GetToken();
            return;
        case LESS:
            operatorr=lexer.GetToken();
            return;
        case NOTEQUAL:
            operatorr=lexer.GetToken();
            return;
        case LTEQ:
            operatorr=lexer.GetToken();
            return;
        default:
            std::cout << "in parseoperator\n";
            syntax_error();
            exit(0);
    }
    
    
    

}

void Parser::parse_expr(){
    Token expr=peek();
    
    if(expr.token_type==NOT){
        lexer.GetToken();
        parse_expr();
    }
    else if(expr.token_type==BOOLEAN || expr.token_type==ID || expr.token_type==REALNUM || expr.token_type==STRING_CONSTANT || expr.token_type==NUM ){
        parse_primary();
    }
   
    else if(operators.find(expr.token_type) != operators.end() ){
        parse_operator();
        parse_expr();
        parse_expr();
    }
    
    else {
        std::cout << "in parsexpr\n";
        syntax_error();
          exit(0);
    }
    
}

void Parser::parse_condition(){
    Token condition=peek();
    if(condition.token_type==LPAREN){
        lexer.GetToken();
        parse_expr();
        condition=lexer.GetToken();
        if(condition.token_type==RPAREN){
            //lexer.GetToken();
        }else{
            std::cout << "in parsecond\n";
            syntax_error();
              exit(0);
        }
    }
    else {
        std::cout << "in parsecond\n";
        syntax_error();
          exit(0);
    }
}


void Parser::parse_stmt(){
    Token stmt=peek();
    
    if(stmt.token_type==ID){
        stmt=lexer.GetToken();
        stmt=peek();
        if(stmt.token_type==EQUAL){
            stmt=lexer.GetToken();
            stmt=peek();
            parse_expr();
            stmt=lexer.GetToken();
            if(stmt.token_type==SEMICOLON){
                
            }else{
                syntax_error();
                std::cout << "in parsestmt\n";
                         exit(0);
            }
        }else {
            syntax_error();
            std::cout << "in parsestmt\n";
                     exit(0);
        }
    }else if(stmt.lexeme=="WHILE"){
        parse_whilestmt();
    }else {
        syntax_error();
        std::cout << "in parsestmt\n";
          exit(0);
    }
    
}

void Parser::parse_whilestmt(){
    bool leftbrace=false;
    Token whilestmt=peek();
    if(whilestmt.lexeme=="WHILE"){
        lexer.GetToken();
        parse_condition();
        whilestmt=peek();
        if(whilestmt.token_type==LBRACE){
            leftbrace=true;
            lexer.GetToken();
        }
        parse_stmt();
        whilestmt=peek();
        if(whilestmt.token_type==WHILE||whilestmt.token_type==ID ){
            parse_stmt();
            whilestmt=peek();
        }
        
        if(whilestmt.token_type==RBRACE && leftbrace==true ){
           lexer.GetToken();
                                        }
        
        
        
        
    } else{
        syntax_error();
        std::cout << "in parsewhilestmt\n";
          exit(0);
    }
    
}

void Parser::parse_typename(){
    Token typenames=peek();
    switch (typenames.token_type) {
        case REAL:
            lexer.GetToken();
            break;
            
        case INT:
            lexer.GetToken();
            break;
            
        case BOOLEAN:
            lexer.GetToken();
            break;
            
        case STRING:
            lexer.GetToken();
            break;
        
        default:
            
            syntax_error();
            std::cout << "in parsetypename\n";
            exit(0);
            
            
    }
    
}

void Parser::parse_idlist(){
    Token idlist=peek();
    if(idlist.token_type==ID){
        lexer.GetToken();
        idlist=peek();
        if(idlist.token_type==COMMA){
            parse_idlist();
        }
    }else {
        syntax_error();
    }
    
}

void Parser::parse_vardecl(){
    Token var=peek();
    if(var.token_type==ID){
        parse_idlist();
        var=peek();
        if(var.token_type == COLON){
            lexer.GetToken();
            parse_typename();
            var=peek();
            if(var.token_type==SEMICOLON){
                lexer.GetToken();
            }else{
                syntax_error();
                std::cout << "in parsevardecl\n";
                exit(0);
            }
        }else{
            syntax_error();
            std::cout << "in parsevardecl\n";
            exit(0);
            
        }
    }else {
       syntax_error();
       std::cout << "in parsevardecl\n";
       exit(0);
    }
}


void Parser::parse_scope(){
    Token scope=peek();
    if(scope.token_type==LBRACE){
        lexer.GetToken();
        parse_scope_list();
        scope=lexer.GetToken();
        if(scope.token_type==RBRACE){
            
        }else {
           syntax_error();
           std::cout << "in parsescope\n";
           exit(0);
        }
    }else {
        syntax_error();
        std::cout << "in parsescope\n";
        exit(0);
    }
    
}

void Parser::parse_scope_list(){
    Token scopelist=peek();
    Token lookahead=lexer.GetToken();
    
      if(scopelist.token_type==ID || scopelist.lexeme=="WHILE"){ // var_decl or var_decl scope_list or stmt or stmt list
          lookahead=lexer.GetToken();
        if(lookahead.token_type==COLON){
            lexer.UngetToken(lookahead);
            lexer.UngetToken(scopelist);
            parse_vardecl();
            scopelist=peek();
        }else if(lookahead.token_type==EQUAL || lookahead.token_type==LPAREN){
            lexer.UngetToken(lookahead);
            lexer.UngetToken(scopelist);
            parse_stmt();
        }
        
        if( (scopelist.token_type==ID ||scopelist.lexeme=="WHILE" || scopelist.token_type==LBRACE) && peek().token_type!=RBRACE ){
            parse_scope_list();
                              }
    }
    
    else if(scopelist.token_type==LBRACE){//scope or scope scope_list
        lexer.UngetToken(lookahead);
        parse_scope();
        scopelist=peek();
        if((scopelist.token_type==ID ||scopelist.lexeme=="WHILE" || scopelist.token_type==LBRACE) && peek().token_type!=RBRACE  ){
            parse_scope_list();
        }
    }
    
    
    
}


void Parser::parse_program(){
    parse_scope();
}























int main()
{
    
    operators.insert(PLUS); operators.insert(MINUS); operators.insert(MULT); operators.insert(DIV);
    
    
    operators.insert(AND); operators.insert(OR); operators.insert(XOR); operators.insert(GREATER); operators.insert(GTEQ); operators.insert(LESS); operators.insert(NOTEQUAL);  operators.insert(LTEQ);
    
    Parser *p=new Parser();
    p->parse_program();
   
    
    
    
}

