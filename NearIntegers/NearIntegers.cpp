// NearIntegers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	const double epsilon = 0.000000001;
	srand(static_cast<unsigned int>(time(NULL)));
	
	while (1)
	{
		ExpressionMaker e;

		// Hack
		if (std::find(e.opCodes.begin(), e.opCodes.end(), Op_sin) == e.opCodes.end())
		{
			continue;
		}

		//printf("TESTING: \n");
		//e.Dump();

		double ret = e.Run();
		double modResult = fmod(ret, 1);

		// printf("MODRESULT: %.32f, %d, %d\n", modResult, static_cast<int>(modResult), static_cast<int>(modResult) != modResult);

		if (fabs(fmod(ret, 1)) < epsilon && fabs(ret) > 1 && fabs(ret) < 10 && static_cast<int>(modResult) != modResult)
		{
			printf("Found result: %f\n", ret);
			e.Dump();
			printf("\n");
		}
	}

	return 0;
}

ExpressionMaker::ExpressionMaker()
{
	opCodes.push_back(Op_value);
	while (1)
	{
		OpCode op;
		if (opCodes.size() > MaxOpCodes)
		{
			break;
		}
		else
		{
			op = static_cast<OpCode>(rand() % Op_END);
		}

		for (int i = 1; i < opCodeArgCount[op]; ++i)
		{
			opCodes.push_back(Op_value);
		}
		opCodes.push_back(op);
	}
}

void ExpressionMaker::Dump()
{
	auto it = values.begin();
	for (OpCode op : opCodes)
	{
		if (op == Op_value && it != values.end())
		{
			printf("%f\n", *it++);
		}
		else
		{
			printf("%s\n", opCodeNames[op]);
		}
	}
	assert(it == values.end());
}

double ExpressionMaker::MakeValue()
{
	return rand() % 100;
}

double ExpressionMaker::Run()
{
	std::list<double> stack;


	for (OpCode op : opCodes)
	{
		std::vector<double> args;
		assert(stack.size() >= static_cast<size_t>(opCodeArgCount[op]));

		// Build the arguments list.
		for (int i = 0; i < opCodeArgCount[op]; ++i)
		{
			args.push_back(stack.back());
			stack.pop_back();
		}

		double val;
		switch (op)
		{
		case Op_value:
			val = MakeValue();
			values.push_back(val);
			stack.push_back(val);
			break;
		case Op_add:
			stack.push_back(args[0] + args[1]);
			break;
		case Op_mul:
			stack.push_back(args[0] * args[1]);
			break;
		case Op_div:
			stack.push_back(args[0] / args[1]);
			break;
		case Op_sin:
			stack.push_back(sin(args[0]));
			break;
		case Op_log:
			stack.push_back(log(args[0]));
			break;
		}
	}

	// TODO: we will insert unnecessary values sometimes, ignore this
	// assert(stack.size() == 1);
	return stack.back();
}

char *opCodeNames[] = {
#define OPCODE(name, numArgs) \
	#name
#include "OpCodes.h"
#undef OPCODE
};

int opCodeArgCount[] = {
#define OPCODE(name, numArgs) \
	numArgs
#include "OpCodes.h"
#undef OPCODE
};
