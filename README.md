# SimpleLangParserExecuter
Procedure-Oriented Language Parser and Executor

## Simple Compiler/Executor for a Custom Programming Language

## Introduction
This project is a simple compiler and executor for a custom programming language, developed as part of the CSE340 course at Arizona State University. It's implemented in C++ and utilizes basic parsing techniques along with simple data structures to interpret and execute programs written in a specifically designed grammar.

## Technological Stack
* Language: C++
* Tools: Lexical Analyzer (provided as part of the course materials)

## Installation
1. To set up the parser on your local machine:
```git clone https://github.com/your-username/your-repository.git```
2. Navigate to the cloned directory
```cd your-repository```

## Usage
To use the parser, compile the source files using a C++ compiler that supports C++11 standards. For instance, using g++ you would run:
```g++ -std=c++11 parser.cc -o parser```
To execute the parser, you can run it directly from the command line:
```./parser```

Input to the parser should be structured as described in the project's grammar specifications, with procedure declarations followed by a main procedure and a sequence of integers.

## Code Structure
* parser.cc: Contains the main logic for parsing the input according to the grammar and storing it into appropriate data structures.
* parser.h: Header file for the parser, declaring necessary functions and data structures.
* lexer.cc:
* lexer.h:
* inputbuf.cc:
* inputbuf.h:

## Features
* Simple Parsing: Capable of parsing custom programming instructions into an intermediate representation.
* Execution: Executes the parsed code and outputs results based on the operations defined in the input script.
* Memory Management: Allocates memory for variables and handles their scope within the program.
