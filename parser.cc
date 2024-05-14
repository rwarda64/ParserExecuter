/*
 * Copyright (C) Rida Bazzi, 2022
 *
 * Do not share this file with anyone
 *
 * Do not post this file or derivatives of
 * of this file online
 *
 */


#include <iostream>
#include <cstdlib>
#include "parser.h"
#include <vector>
#include <map>


using namespace std;


std::map<std::string, int>   loc;   // * hashmap/table for location (for variables in memory).
std::map<std::string, stmt*> proc_table;  // * hashmap/table for procedures.

int next_available = 0; // * you increment next_available so that the next allocation will be to the next location in memory.
int next_input = 0;     // * keeps track of how many inputs are consumed.

vector <int> mem;   // holds values of data.
vector <int> inputs;    // * holds all input values to be accessed later when program is "executed".


static int get_location(string lexeme) {
	if(loc.find(lexeme) != loc.end()){
        // if the variable name exists
       return loc[lexeme];   // find idx location of variable name, set it to op1.
    }
    else{
        // otherwise, (if variable DNE) allocate for it
        loc[lexeme] = next_available; // set variable to next_available slot
        next_available++;               // incr for next allocation
        return loc[lexeme];    // set op1 to created memory location for variable
    }
}


void Parser::syntax_error(){
    cout << "SYNTAX ERROR !!!\n";
    exit(1);
}

/**
input                = <input_program> <inputs> .
main                 = 'MAIN' <procedure_body> .
input_program        = [<proc_decl_section>] <main> .

proc_decl_section    = <proc_decl>+ .

proc_decl            = 'PROC' <procedure_name> <procedure_body> 'ENDPROC' .
 */

// inputs = NUM+ .
void Parser::parse_inputs() {
	expect(NUM);

    Token t = lexer.peek(1);
    if(t.token_type == END_OF_FILE){
        return;
    }
    else if(t.token_type == NUM){
        parse_inputs();
    }
    else{
        syntax_error();
    }
}

// primary = NUM | ID .
void Parser::parse_primary() {
	Token t = lexer.peek(1);
	switch( t.token_type ) {
		case NUM:{
            expect(NUM);
        }
			break;

		case ID:{
            expect(ID);
        }
            break;
		default:
			syntax_error();
	}
}

// operator = '+' | '-' | '/' | '*' .
void Parser::parse_operator() {
    Token t = lexer.peek(1);
	if( t.token_type >= PLUS && t.token_type <= DIV ) {     // PLUS > MINUS > MULT > DIV
		expect(t.token_type);
	}
    else{
        syntax_error();
    }

}

void Parser::parse_expr(){
    parse_primary();
    Token t = lexer.peek(1);
    if(t.token_type == SEMICOLON){
        return;
    }
    else if( t.token_type >= PLUS && t.token_type <= DIV ){
        parse_operator();
        parse_primary();
    }
    else{
        syntax_error();
    }
}

// assign_statement = ID '=' <expr> ';' .
void Parser::parse_assign_statement() { // TODO <COMEBACK>
	expect(ID);           // consume ID
    expect(EQUAL);        // consume '='
    parse_expr();
    expect(SEMICOLON);    // consume ';'
}

// do_statement = 'DO' ID <procedure_invocation> .
void Parser::parse_do_statement() {
    expect(DO);
    expect(ID);
    parse_procedure_invocation();
}


// procedure_invocation = <procedure_name> ';' . <--great-->
void Parser::parse_procedure_invocation() {
	parse_procedure_name();
    expect(SEMICOLON);
}

// output_statement = 'OUTPUT' ID ';' . -<-
void Parser::parse_output_statement() {
	expect(OUTPUT);
    expect(ID);
    expect(SEMICOLON);
}

// input_statement = 'INPUT' ID ';' .
void Parser::parse_input_statement() {
    expect(INPUT);
    expect(ID);
    expect(SEMICOLON);
}

// statement = <input_statement> | <output_statement> | <procedure_invocation> | <do_statement> | <assign_statement> .
void Parser::parse_statement() {
    Token t1 = lexer.peek(1);   // first lookahead
	switch( t1.token_type ) {
        case INPUT:
            parse_input_statement();
            break;

        case OUTPUT:
            parse_output_statement();
            break;

        case ID:{
            Token t2 = lexer.peek(2);
            if(t2.token_type == EQUAL){
                parse_assign_statement();
            }
            else if(t2.token_type == SEMICOLON){
                parse_procedure_invocation();               // X PROC  PROC X
            }
            else{
                syntax_error();
            }
        }
        break;

        case NUM:{
            parse_procedure_invocation();
        }
        break;

		case DO:
			parse_do_statement();
            break;

		default:
			syntax_error();
	}
}


// statement_list = <statement>+ .
void Parser::parse_statement_list() {
	// make linked list chain of statements.
    parse_statement();

	Token t = lexer.peek(1);
    Token t2 = lexer.peek(2);
    if ((t.token_type == ENDPROC) || (t.token_type == NUM && t2.token_type == NUM) || (t.token_type == NUM && t2.token_type == END_OF_FILE)){
        return;
    }
    else if((t.token_type == INPUT || t.token_type == OUTPUT || t.token_type == ID || t.token_type == DO) || (t.token_type == NUM && t2.token_type == SEMICOLON)){
        parse_statement_list();
    }
    else{
        syntax_error();
    }
}


// procedure_body = <statement_list> .
void Parser::parse_procedure_body() {
	parse_statement_list();
}

// procedure_name = ID |  NUM.
void Parser::parse_procedure_name() {
	Token t = lexer.peek(1);
	if( t.token_type == ID || t.token_type == NUM ) {
		t = expect(t.token_type);  // consumes 'ID' | 'NUM'
	}
    else{
        syntax_error();
    }

}

// proc_decl = 'PROC' <procedure_name> <procedure_body> 'ENDPROC' .
void Parser::parse_proc_decl() {
	expect(PROC);

	parse_procedure_name();

	parse_procedure_body();

	expect(ENDPROC);
}

// proc_decl_section = <proc_decl>+ .
void Parser::parse_proc_decl_section() {
    parse_proc_decl();

	Token t = lexer.peek(1);    // peek next token.

    if(t.token_type == MAIN){
        return;
    }
    else if(t.token_type == PROC){
        parse_proc_decl_section();
    }
    else{
        syntax_error();
    }


//    if(t.token_type == PROC){
//        parse_proc_decl();
//        t = lexer.peek(1);
//        if(t.token_type == PROC){
//            parse_proc_decl_section();
//        }
//    }
//    else{
//        syntax_error();
//    }
}

// input_program = [<proc_decl_section>] <main> .
void Parser::parse_input_program() {
    Token t = lexer.peek(1);

	// if there's a procedure, parse it.
	if( t.token_type == PROC ) {
		parse_proc_decl_section();
        parse_main();
	}
    else if(t.token_type == MAIN){
        parse_main();
    }
    else{
        syntax_error();
    }
}

// main = 'MAIN' <procedure_body> .
void Parser::parse_main() {
    expect(MAIN);
    parse_procedure_body();
}

// input = <input_program> <inputs> .
void Parser::parse_input() {
	parse_input_program();
	parse_inputs();
}


// this function gets a token and checks if it is
// of the expected type. If it is, the token is
// returned, otherwise, synatx_error() is generated
// this function is particularly useful to match
// terminals in a right hand side of a rule.
// Written by Mohsen Zohrevandi
Token Parser::expect(TokenType expected_type)
{
    Token t = lexer.GetToken();
    if (t.token_type != expected_type)
        syntax_error();
    return t;
}

// This function simply reads and prints all tokens
// I included it as an example. You should compile the provided code
// as it is and then run ./a.out < tests/test0.txt to see what this 
// function does
void Parser::readAndPrintAllInput()
{
    Token t;

    // get a token
    t = lexer.GetToken();

    // while end of input is not reached
    while (t.token_type != END_OF_FILE) 
    {
        t.Print();         	// print token
        t = lexer.GetToken();	// and get another one
    }
        
    // note that you should use END_OF_FILE and not EOF
}



// TODO ...
void execute_program(struct stmt* start) {
	struct stmt *pc = start;
	while(pc != NULL) {
		switch(pc->stmt_type) {
			case ASSIGN: switch(pc->op){
				case PLUS:
					mem[pc->LHS] = mem[pc->op1] + mem[pc->op2];
					break;
				case MINUS:
					mem[pc->LHS] = mem[pc->op1] - mem[pc->op2];
					break;
				case MULT:
					mem[pc->LHS] = mem[pc->op1] * mem[pc->op2];
					break;
				case DIV:
					mem[pc->LHS] = mem[pc->op1] / mem[pc->op2];
					break;
				default:
					mem[pc->LHS] = mem[pc->op1];
			}
			break;
			
			case OUTPUT:
				cout << mem[pc->op1];
				break;
			
			case INPUT:
				mem[pc->op1] = inputs[next_input];
				next_input++;   // increment to consume input.
				break;
			
			case PROC:
				execute_program(proc_table[pc->proc_name]);
				break;
			
			case DO: {
				int count = mem[pc->op1];
				auto proc_bod = proc_table[pc->next->proc_name];
				while( count != 0 ) {
					execute_program(proc_bod);
					count--;
				}
				break;
			}
		}
		pc = pc->next;
	}
}


int main(int argc, char *argv[])
{
    // note: the parser class has a lexer object instantiated in it (see file
    // parser.h). You should not be declaring a separate lexer object. 
    // You can access the lexer object in the parser functions as shown in 
    // the example  method Parser::readAndPrintAllInput()
    // If you declare another lexer object, lexical analysis will 
    // not work correctly
    Token t;

    Parser parser;

    parser.parse_input();
	
}
