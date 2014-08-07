#pragma once

enum OpCode {
#define OPCODE(name, numArgs) \
	Op_##name
#include "OpCodes.h"
#undef OPCODE
};


extern char *opCodeNames[];
extern int opCodeArgCount[];

enum ValueContext
{
	Val_normal,
	Val_pow
};

class ExpressionMaker
{
	double MakeValue();
public:
	static const int MaxOpCodes = 6;
	std::list<OpCode> opCodes;
	std::vector<double> values;
	ExpressionMaker();
	double Run();
	void Dump();
};