#pragma once
#ifndef PARSER_H_
#define PARSER_H_

/*
 *
 *
Copyright (c) 2007 Michael Haupt, Tobias Pape, Arne Bergmann
Software Architecture Group, Hasso Plattner Institute, Potsdam, Germany
http://www.hpi.uni-potsdam.de/swa/

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
  */


#include <fstream>
#include <string>

#include "../misc/defs.h"

#include "Lexer.h"
#include "ClassGenerationContext.h"
#include "MethodGenerationContext.h"
#include "BytecodeGenerator.h"


class Parser {
public:
	Parser(istream& file);
	~Parser();

	void Classdef(ClassGenerationContext* cgenc);
private:	
	bool        eob(void);

	bool        symIn(Symbol* ss);
	bool        accept(Symbol s);
	bool        acceptOneOf(Symbol* ss);
	bool        expect(Symbol s);
	bool        expectOneOf(Symbol* ss);
	void        SingleOperator(void);
	void        instanceFields(ClassGenerationContext* cgenc);
	void        classFields(ClassGenerationContext* cgenc);
	void        method(MethodGenerationContext* mgenc);
	void        primitiveBlock(void);
	void        pattern(MethodGenerationContext* mgenc);
	void        unaryPattern(MethodGenerationContext* mgenc);
	void        binaryPattern(MethodGenerationContext* mgenc);
	void        keywordPattern(MethodGenerationContext* mgenc);
	void        methodBlock(MethodGenerationContext* mgenc);
	pVMSymbol   unarySelector(void);
	pVMSymbol   binarySelector(void);
	StdString     identifier(void);
	StdString     keyword(void);
	StdString     argument(void);
	void        blockContents(MethodGenerationContext* mgenc);
	void        locals(MethodGenerationContext* mgenc);
	void        blockBody(MethodGenerationContext* mgenc, bool seen_period);
	void        result(MethodGenerationContext* mgenc);
	void        expression(MethodGenerationContext* mgenc);
	void        assignation(MethodGenerationContext* mgenc);
	void        assignments(MethodGenerationContext* mgenc, list<string>& l);
	StdString     assignment(MethodGenerationContext* mgenc);
	void        evaluation(MethodGenerationContext* mgenc);
	void        primary(MethodGenerationContext* mgenc, bool* super);
	StdString     variable(void);
	void        messages(MethodGenerationContext* mgenc, bool super);
	void        unaryMessage(MethodGenerationContext* mgenc, bool super);
	void        binaryMessage(MethodGenerationContext* mgenc, bool super);
	void        binaryOperand(MethodGenerationContext* mgenc, bool* super);
	void        keywordMessage(MethodGenerationContext* mgenc, bool super);
	void        formula(MethodGenerationContext* mgenc);
	void        nestedTerm(MethodGenerationContext* mgenc);
	void        literal(MethodGenerationContext* mgenc);
	void        literalNumber(MethodGenerationContext* mgenc);
	uint32_t    literalDecimal(void);
	int32_t     negativeDecimal(void);
	uint32_t    literalInteger(void);
	void        literalSymbol(MethodGenerationContext* mgenc);
	void        literalString(MethodGenerationContext* mgenc);
	pVMSymbol   selector(void);
	pVMSymbol   keywordSelector(void);
	StdString     _string(void);
	void        nestedBlock(MethodGenerationContext* mgenc);
	void        blockPattern(MethodGenerationContext* mgenc);
	void        blockArguments(MethodGenerationContext* mgenc);
	void        genPushVariable(MethodGenerationContext*, const StdString&);
	void        genPopVariable(MethodGenerationContext*, const StdString&);

	Lexer* lexer;
	
	Symbol sym;
	
    StdString text;

	Symbol nextSym;
	
    StdString nextText;
	
    BytecodeGenerator* bcGen;
};

#endif
