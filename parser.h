/*
 * Copyright (C) Rida Bazzi, 2019
 *
 * Do not share this file with anyone
 */
#ifndef __PARSER_H__
#define __PARSER_H__

#include <string>
#include "lexer.h"


class Parser {
public:
	void readAndPrintAllInput();
	
	// inputs = NUM+ .
	void parse_inputs();
	
	// primary = NUM | ID .
    void parse_primary();
	
	// operator = '+' | '-' | '/' | '*' .
    void parse_operator();
	
	// expr = <primary> [<operator> <primary>] .
    void parse_expr();

	// assign_statement rr= ID '=' <expr> ';' .
	void parse_assign_statement();
	
	// do_statement = 'DO' ID <procedure_invocation> .
    void parse_do_statement();
	
	// procedure_invocation = <procedure_name> ';' .
    void parse_procedure_invocation();
	
	// output_statement = 'OUTPUT' ID ';' .
    void parse_output_statement();
	
	// input_statement = 'INPUT' ID ';' .
    void parse_input_statement();
	
	// statement = <input_statement> | <output_statement> | <procedure_invocation> | <do_statement> | <assign_statement> .
    void parse_statement();
	
	// statement_list = <statement>+ .
    void parse_statement_list();
	
	// procedure_body = <statement_list> .
    void parse_procedure_body();
	
	// procedure_name = ID | NUM .
    void parse_procedure_name();
	
	// proc_decl = 'PROC' <procedure_name> <procedure_body> 'ENDPROC' .
	void parse_proc_decl();
	
	// proc_decl_section = <proc_decl>+ .
	void parse_proc_decl_section();
	
	// input_program = [<proc_decl_section>] <main> .
	void parse_input_program();
	
	// main = 'MAIN' <procedure_body> .
	void parse_main();
	
	// input = <input_program> <inputs> .
	void parse_input();
	
private:
	LexicalAnalyzer lexer;
	void syntax_error();
	Token expect(TokenType expected_type);
	void parse_program();
};


// ASSIGN  - uses 'op, op1, op2'
// DO      - uses 'next->proc_name' & 'op1'
// INPUT   - uses 'op1'
// OUTPUT  - uses 'op1'
// PROC    - uses 'proc_name'

enum {
	ASSIGN = EQUAL
};
struct stmt {
	int op, op1, op2, LHS;
	struct stmt *next;
	std::string proc_name;
    int stmt_type;  // indicates type of statement [ASSIGN, INPUT, OUTPUT, INVOKE, and DO]    
};

#endif