
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


TokenType Parser::parse_operator(){
    Token t=peek();
    if(t.token_type==AND || t.token_type==OR || t.token_type==XOR || t.token_type==PLUS || t.token_type==MINUS || t.token_type==DIV || t.token_type==MULT || t.token_type==GREATER || t.token_type==GTEQ || t.token_type==LESS || t.token_type==NOTEQUAL || t.token_type==LTEQ  ){
        t=lexer.GetToken();
        return t.token_type;
    }
    else {
        syntax_error();
        return ERROR;
    }
}

TokenType Parser::parse_primary(){
   Token p=peek();
    symbol *d;
   //isBuiltInType(p);
   if(p.token_type==ID){
       reference *s=new reference(p.line_no,p.lexeme);
       s->side="RIGHT";
       s->line=p.line_no;
       if(lookup(currentscope,s)!=nullptr){
           d=lookup(currentscope, s);
            lookup(currentscope,s)->allrefs->push_back(s);
           addto_output(currentscope,d,s);
           if(d->isInitalized==false){
               unintalized.append("UNINITIALIZED "); unintalized.append(s->name); unintalized.append(" "); unintalized.append(std::to_string(s->line)); unintalized.append("\n");
           }
           
       }
       else {
            error.append("ERROR CODE 1.2 "); error.append(p.lexeme); error.append("\n");
       }
   }
      if(p.token_type==ID || p.token_type==NUM || p.token_type==STRING_CONSTANT || p.token_type==TRUE || p.token_type==FALSE || p.token_type==REALNUM ){
          switch(p.token_type){
              case ID:
                  lexer.GetToken();
                  return d->declaredType;
              case NUM:
                  lexer.GetToken();
                  return INT;

              case STRING_CONSTANT:
                  lexer.GetToken();
                  return STRING;

              case TRUE:
                  lexer.GetToken();
                  return BOOLEAN;

              case FALSE:
                  lexer.GetToken();
                  return BOOLEAN;

              case REALNUM:
                  lexer.GetToken();
                  return REAL;


          }
          p=lexer.GetToken();
      }
      else{
          syntax_error();
      }

    return NUM;


}

TokenType Parser::parse_expr(){
    Token e=peek();
    TokenType op;
    TokenType type1;
    TokenType type2;

    if(e.token_type==AND || e.token_type==OR || e.token_type==XOR || e.token_type==PLUS || e.token_type==MINUS || e.token_type==DIV || e.token_type==MULT || e.token_type==GREATER || e.token_type==GTEQ || e.token_type==LESS || e.token_type==NOTEQUAL || e.token_type==LTEQ ) /*expr->operator expr expr */{
        op=parse_operator();
        type1=parse_expr();
        type2=parse_expr();
        return type_checker(op, type1, type2);
    }
    else if(e.token_type==NOT){
        e=lexer.GetToken();
        TokenType u=parse_expr();
        if(u!=BOOLEAN){
            type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C8"); type.push_back("\n");
            typeindex++;
            rightsidemismatch=true;
            mismatchonthisline=true;
            return ERROR;
        } else {
            return u;
        }
    }
    else {
        return parse_primary();
    }

}

void Parser::parse_condition(){
    expect(LPAREN);
    TokenType isbool=parse_expr();
    if(isbool != BOOLEAN && !rightsidemismatch){
         type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C7"); type.push_back("\n");
         typeindex++;
        mismatchonthisline=true;
    }
    expect(RPAREN);
}

void Parser::parse_whilestmt(){
    expect(WHILE);
    parse_condition(); //WHILE condition
    Token w=peek();

    scope *temp;
    temp=currentscope;
    scope *newscope=new scope();
    newscope->parent=currentscope;
    currentscope=whileLoopScope(currentscope, newscope);


    if(w.token_type==LBRACE){ //WHILE condition LBRACE stmtlist RBRACE
        lexer.GetToken();
        parse_stmt_list();
        expect(RBRACE);
        currentscope=temp;
    }
    else if(w.token_type==ID || w.token_type==WHILE){//WHILE condition stmt
        parse_stmt();
    }
    else{
        syntax_error();
    }
}

void Parser::parse_assignstmt(){
    Token e=expect(ID);
    //isBuiltInType(e);
    reference *ref=new reference(e.line_no,e.lexeme);
    symbol *LHS=lookup(currentscope,ref);
    if(LHS==nullptr) {
        error.append("ERROR CODE 1.2 "); error.append(e.lexeme); error.append("\n");}
    else {
        ref->side="LEFT";
        LHS->allrefs->push_back(ref);
        addto_output(currentscope, LHS, ref);
    }

    expect(EQUAL);
   TokenType RHS= parse_expr();

    if(LHS!=nullptr && rightsidemismatch==false && mismatchonthisline==false){
        LHS->isInitalized=true;
        TokenType LHStype=LHS->declaredType;
        if(LHStype==INT && RHS!=INT){
             type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C1"); type.push_back("\n");
            typeindex++;
            mismatchonthisline=true;
        } else if(LHStype==STRING &&  RHS!=STRING ){
             type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C1"); type.push_back("\n");
            typeindex++;
            mismatchonthisline=true;
        }else if(LHStype==BOOLEAN && RHS!=BOOLEAN){
             type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C1"); type.push_back("\n");
            typeindex++;
            mismatchonthisline=true;
        }else if(LHStype == REAL && (RHS!=REAL)){
             type.push_back("TYPE MISMATCH "); type.push_back(std::to_string(lexer.get_line_no())); type.push_back(" C2"); type.push_back("\n");
            typeindex++;
            mismatchonthisline=true;
        }
        
        

    }

    rightsidemismatch=false;
    mismatchonthisline=false;
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

symbol* Parser::parse_idlist(){
    Token decl=expect(ID);
    //isBuiltInType(decl);
    symbol *s=new symbol(decl);
    //lookup decl in currentscope and set error code if it returns a symbol in the currentscope
    if(lookup(currentscope,s) != nullptr && lookup(currentscope, s)->scope_==currentscope){
        error.append("ERROR CODE 1.1 "); error.append(s->name); error.append("\n");
    }else {
        currentscope->symboltable.push_back(s);
        s->scope_=currentscope;

    }
    Token i=peek();


    if(i.token_type==COMMA){
        i=lexer.GetToken();
        s->linedeclared=lexer.get_line_no();
        s->nextsymbol=parse_idlist();
        s->declaredType=s->nextsymbol->declaredType;
    }
    else {
        Token colon=lexer.GetToken();
           Token type=lexer.GetToken();
           lexer.UngetToken(type);
           lexer.UngetToken(colon);
           s->declaredType=type.token_type;
    }
    return s;
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
    scope *temp=nullptr;
       if(currentscope==nullptr){
           currentscope=new scope();
       } else if(currentscope!=nullptr){
           temp=currentscope;
           scope *newscope=new scope();
           newscope->parent=temp;
           currentscope=newscope;
      }
    


    parse_scope_list();

    for(int i=0;i<currentscope->symboltable.size();i++){
        string name=currentscope->symboltable[i]->name;
        if(currentscope->symboltable[i]->allrefs->size()==0 && error.find(name)==string::npos){
             error.append("ERROR CODE 1.3 "); error.append(currentscope->symboltable[i]->name); error.append("\n");
        }
        
    }

    



    expect(RBRACE);
    currentscope=currentscope->parent;
}

void Parser::parse_program(){
    parse_scope();
    expect(END_OF_FILE);
}


int main(){
    Parser p;
    p.parse_program();

   if(!error.empty()){
        cout<<error;
    }
      else if(!type.empty()){
          for(int i=0; i<type.size(); i++){
              cout << type[i];
          }
    }
     else if(!unintalized.empty()){
        cout <<unintalized;
    } else {
        cout << output;
    }
    
}

