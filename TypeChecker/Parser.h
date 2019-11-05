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



LexicalAnalyzer lexer;



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




