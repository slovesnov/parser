<?php

/*
notes for OPERATOR_ENUM 
PLUS should be first in enum. Common operators should go in a row. Order is important. 
POW, ATAN2, MIN, MAX should go in a row see parse3 function
PLUS-POW should go in row getToken()
POW-SQRT should go in row from parse3()
finally PLUS should be first, PLUS-SQRT should goes in row
*/
class OPERATOR_ENUM
{
	const PLUS = 0;
	const MINUS = 1;
	const MULTIPLY = 2;
	const DIVIDE = 3;
	const LEFT_BRACKET = 4;
	const RIGHT_BRACKET = 5;
	const LEFT_SQUARE_BRACKET = 6;
	const RIGHT_SQUARE_BRACKET = 7;
	const LEFT_CURLY_BRACKET = 8;
	const RIGHT_CURLY_BRACKET = 9;
	const COMMA = 10;
	const POW = 11;
	const ATAN2 = 12;
	const MIN = 13;
	const MAX = 14;
	const SIN = 15;
	const COS = 16;
	const TAN = 17;
	const COT = 18;
	const SEC = 19;
	const CSC = 20;
	const ASIN = 21;
	const ACOS = 22;
	const ATAN = 23;
	const ACOT = 24;
	const ASEC = 25;
	const ACSC = 26;
	const SINH = 27;
	const COSH = 28;
	const TANH = 29;
	const COTH = 30;
	const SECH = 31;
	const CSCH = 32;
	const ASINH = 33;
	const ACOSH = 34;
	const ATANH = 35;
	const ACOTH = 36;
	const ASECH = 37;
	const ACSCH = 38;
	const RANDOM = 39;
	const CEIL = 40;
	const FLOOR = 41;
	const ROUND = 42;
	const ABS = 43;
	const EXP = 44;
	const LOG = 45;
	const SQRT = 46;
	const X = 47;
	const NUMBER = 48;
	const UNARY_MINUS = 49;
	const END = 50;
}
