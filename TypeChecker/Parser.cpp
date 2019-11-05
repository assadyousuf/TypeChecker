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
    std::cout<<"Syntax Error";
   // std::cout<<" line no: "<< lexer.get_line_no();
    exit(1);
}

Token Parser::peek()
{
    Token t = lexer.GetToken();
    lexer.UngetToken(t);
    return t;
}

Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}


void Parser::parse_operator(){
    Token t=peek();
    if(t.token_type==AND || t.token_type==OR || t.token_type==XOR || t.token_type==PLUS || t.token_type==MINUS || t.token_type==DIV || t.token_type==MULT || t.token_type==GREATER || t.token_type==GTEQ || t.token_type==LESS || t.token_type==NOTEQUAL || t.token_type==LTEQ  ){
        t=lexer.GetToken();
    }
    else {
        syntax_error();
    }
}

void Parser::parse_primary(){
    Token p=peek();
       if(p.token_type==ID || p.token_type==NUM || p.token_type==STRING_CONSTANT || p.token_type==TRUE || p.token_type==FALSE || p.token_type==REALNUM ){
           p=lexer.GetToken();
       }
       else{
           syntax_error();
       }
}

void Parser::parse_expr(){
    Token e=peek();
    if(e.token_type==AND || e.token_type==OR || e.token_type==XOR || e.token_type==PLUS || e.token_type==MINUS || e.token_type==DIV || e.token_type==MULT || e.token_type==GREATER || e.token_type==GTEQ || e.token_type==LESS || e.token_type==NOTEQUAL || e.token_type==LTEQ ) /*expr->operator expr expr */{
        parse_operator();
        parse_expr();
        parse_expr();
    }
    else if(e.token_type==NOT){
        e=lexer.GetToken();
        parse_expr();
    }
    else {
        parse_primary();
    }
    
}

void Parser::parse_condition(){
    expect(LPAREN);
    parse_expr();
    expect(RPAREN);
}

void Parser::parse_whilestmt(){
    expect(WHILE);
    parse_condition(); //WHILE condition
    Token w=peek();
    if(w.token_type==LBRACE){ //WHILE condition LBRACE stmtlist RBRACE
        lexer.GetToken();
        parse_stmt_list();
        expect(RBRACE);
    }
    else if(w.token_type==ID || w.token_type==WHILE){//WHILE condition stmt
        parse_stmt();
    }
    else{
        syntax_error();
    }
}

void Parser::parse_assignstmt(){
    expect(ID);
    expect(EQUAL);
    parse_expr();
    expect(SEMICOLON);
}

void Parser::parse_stmt(){
    Token s=peek();
    if(s.token_type==WHILE){
        parse_whilestmt();
    }
    else {
        parse_assignstmt();
    }
}

void Parser::parse_stmt_list(){ //may cause problems
    parse_stmt();
    Token s=lexer.GetToken();
    Token s2=lexer.GetToken();
    lexer.UngetToken(s2);
    lexer.UngetToken(s);
    
    if(s.token_type==WHILE){
        parse_stmt_list();
    }
    else if(s.token_type==ID){
        if(s2.token_type==EQUAL){
            parse_stmt_list();
        }
    }
}

void Parser::parse_typename(){
    Token t=peek();
    if(t.token_type==REAL|| t.token_type==INT || t.token_type==BOOLEAN || t.token_type==STRING){
        t=lexer.GetToken();
    }else {
        syntax_error();
    }
}

void Parser::parse_idlist(){
    expect(ID);
    Token i=peek();
    if(i.token_type==COMMA){
        i=lexer.GetToken();
        parse_idlist();
    }
}

void Parser::parse_vardecl(){
    parse_idlist();
    expect(COLON);
    parse_typename();
    expect(SEMICOLON);
}



void Parser::parse_scope_list(){
    Token s=lexer.GetToken();
    Token s2=lexer.GetToken(); //unget S then S2
    lexer.UngetToken(s2);
    lexer.UngetToken(s);
   
    if(s.token_type==WHILE){
        parse_stmt();
    }
    
    else if(s.token_type==ID){
        if(s2.token_type==EQUAL){
            parse_stmt();
        }
        else {
            parse_vardecl();
        }
        }
    
    else if(s.token_type==LBRACE){
           parse_scope();
       }
    
    
    else {
        syntax_error();
    }
    
    s=peek();
    if(s.token_type!=RBRACE){
              parse_scope_list();
    }
}


void Parser::parse_scope(){
    expect(LBRACE);
    parse_scope_list();
    expect(RBRACE);
}

void Parser::parse_program(){
    parse_scope();
    //expect(END_OF_FILE);
}

























int main()
{
    
  
    Parser *p=new Parser();
    p->parse_program();
    
    
    
}

