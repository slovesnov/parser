let gdata = `"0x."
static_compile
compile_error

"0x"
static_compile
compile_error

"0x1"
static_compile
1

"0x.1"
static_compile
0.0625

"0x.+1"
static_compile
compile_error

"0x+1"
static_compile
compile_error

"0x1.1"
static_compile
1.0625

"0b"
static_compile
compile_error

"0b."
static_compile
compile_error

"0b1"
static_compile
1

"0b.1"
static_compile
.5

"0b.+1"
static_compile
compile_error

"0b+1"
static_compile
compile_error

"0b1.1"
static_compile
1.5

"0b10.10"
static_compile
2.5

"0b01"
static_compile
1

"0b.11"
static_compile
.75

"0o17"
static_compile
15

"0o81"
static_compile
compile_error

"0o21"
static_compile
17

"0o21.4"
static_compile
17.5

"+1.e1"
static_compile
10

"x0"
static_compile
compile_error

"a"
static_compile
compile_error

"x0+x1"
static_compile
compile_error

"a+b"
static_compile
compile_error

"pi"
static_compile
3.141592653589793

"e"
static_compile
2.718281828459045

"sqrt2"
static_compile
1.414213562373095

"sqrt1_2"
static_compile
0.7071067811865476

"ln2"
static_compile
0.6931471805599453

"ln10"
static_compile
2.302585092994046

"log2e"
static_compile
1.442695040888963

"log10e"
static_compile
0.4342944819032518

"exp(.5)"
static_compile
1.648721270700128

"log(.5)"
static_compile
-0.6931471805599453

"sqrt(.5)"
static_compile
0.7071067811865476

"abs(.5)"
static_compile
0.5

"sign(.5)"
static_compile
1

"sign(0)"
static_compile
0

"sign(-.5)"
static_compile
-1

"sin(.5)"
static_compile
0.479425538604203

"cos(.5)"
static_compile
0.8775825618903728

"tan(.5)"
static_compile
0.5463024898437905

"cot(.5)"
static_compile
1.830487721712452

"sec(.5)"
static_compile
1.139493927324549

"csc(.5)"
static_compile
2.085829642933488

"asin(.5)"
static_compile
0.5235987755982989

"acos(.5)"
static_compile
1.047197551196598

"atan(.5)"
static_compile
0.4636476090008061

"acot(.5)"
static_compile
1.10714871779409

"asec(.5)"
static_compile
nan

"acsc(.5)"
static_compile
nan

"sinh(.5)"
static_compile
0.5210953054937474

"cosh(.5)"
static_compile
1.127625965206381

"tanh(.5)"
static_compile
0.4621171572600097

"coth(.5)"
static_compile
2.163953413738653

"sech(.5)"
static_compile
0.886818883970074

"csch(.5)"
static_compile
1.919034751334944

"asinh(.5)"
static_compile
0.4812118250596035

"acosh(.5)"
static_compile
nan

"atanh(.5)"
static_compile
0.5493061443340549

"acoth(.5)"
static_compile
nan

"asech(.5)"
static_compile
1.316957896924817

"acsch(.5)"
static_compile
1.44363547517881

"sin(2)"
static_compile
0.9092974268256817

"cos(2)"
static_compile
-0.4161468365471424

"tan(2)"
static_compile
-2.185039863261519

"cot(2)"
static_compile
-0.4576575543602858

"sec(2)"
static_compile
-2.402997961722381

"csc(2)"
static_compile
1.099750170294616

"asin(2)"
static_compile
nan

"acos(2)"
static_compile
nan

"atan(2)"
static_compile
1.10714871779409

"acot(2)"
static_compile
0.4636476090008061

"asec(2)"
static_compile
1.047197551196598

"acsc(2)"
static_compile
0.5235987755982989

"sinh(2)"
static_compile
3.626860407847019

"cosh(2)"
static_compile
3.762195691083631

"tanh(2)"
static_compile
0.9640275800758169

"coth(2)"
static_compile
1.037314720727548

"sech(2)"
static_compile
0.2658022288340797

"csch(2)"
static_compile
0.2757205647717832

"asinh(2)"
static_compile
1.44363547517881

"acosh(2)"
static_compile
1.316957896924817

"atanh(2)"
static_compile
nan

"acoth(2)"
static_compile
0.5493061443340549

"asech(2)"
static_compile
nan

"acsch(2)"
static_compile
0.4812118250596035

"ceil(.5)"
static_compile
1

"floor(.5)"
static_compile
0

"round(.5)"
static_compile
1

"atan2(2,.5)"
static_compile
1.325817663668033

"pow(2,.5)"
static_compile
1.414213562373095

"min(2,.5)"
static_compile
0.5

"max(2,.5)"
static_compile
2

"1+2"
static_compile
3

"1-2"
static_compile
-1

"1*2"
static_compile
2

"1/2"
static_compile
0.5

"2^3"
static_compile
8

"3**2"
static_compile
9

"2 + 3 * * 2 + 8"
static_compile
19

"1&2"
static_compile
compile_error

"-1"
static_compile
-1

"+1"
static_compile
1

"(1+"
static_compile
compile_error

"(1+2)"
static_compile
3

"[1+2)"
static_compile
compile_error

"[1+2.1]"
static_compile
3.1

"{1+2.}"
static_compile
3

"1+("
static_compile
compile_error

"	 	 1 + 2 "
static_compile
3

"a+2*A" "a A" "1 2"
compile_calculate
5

"a+2*A+" "a A" "1 2"
compile_calculate
compile_error

"x0" "" "1"
compile_calculate
1

"x 0" "x0" "3"
compile_calculate
3

"a+2*sin(0)" "a sIn" "1 2"
compile_calculate
compile_error

"a+b" "a b" "1 2 4"
compile_calculate
calculate_error

"a+b" "a b" "1"
compile_calculate
calculate_error

"a+a1a" "a a1a" "3 4"
compile_calculate
7

"a+a1a+x0" "a a1a" "3 4"
compile_calculate
compile_error

"x0+x1" "x0" "0"
compile_calculate
compile_error

"a+a1a+x0" "a a1a a x0" "3 4 0 1"
compile_calculate
compile_error

"x+#0+y" "x y" "1 2"
compile_calculate
compile_error

"1x" "1x" "1"
compile_calculate
compile_error

"pi" "pi" "0"
compile_calculate
compile_error

"e" "e" "0"
compile_calculate
compile_error

"sqrt2" "sqrt2" "0"
compile_calculate
compile_error

"sqrt1_2" "sqrt1_2" "0"
compile_calculate
compile_error

"ln2" "ln2" "0"
compile_calculate
compile_error

"ln10" "ln10" "0"
compile_calculate
compile_error

"log2e" "log2e" "0"
compile_calculate
compile_error

"log10e" "log10e" "0"
compile_calculate
compile_error

"pow" "pow" "0"
compile_calculate
compile_error

"atan2" "atan2" "0"
compile_calculate
compile_error

"min" "min" "0"
compile_calculate
compile_error

"max" "max" "0"
compile_calculate
compile_error

"sin" "sin" "0"
compile_calculate
compile_error

"cos" "cos" "0"
compile_calculate
compile_error

"tan" "tan" "0"
compile_calculate
compile_error

"cot" "cot" "0"
compile_calculate
compile_error

"sec" "sec" "0"
compile_calculate
compile_error

"csc" "csc" "0"
compile_calculate
compile_error

"asin" "asin" "0"
compile_calculate
compile_error

"acos" "acos" "0"
compile_calculate
compile_error

"atan" "atan" "0"
compile_calculate
compile_error

"acot" "acot" "0"
compile_calculate
compile_error

"asec" "asec" "0"
compile_calculate
compile_error

"acsc" "acsc" "0"
compile_calculate
compile_error

"sinh" "sinh" "0"
compile_calculate
compile_error

"cosh" "cosh" "0"
compile_calculate
compile_error

"tanh" "tanh" "0"
compile_calculate
compile_error

"coth" "coth" "0"
compile_calculate
compile_error

"sech" "sech" "0"
compile_calculate
compile_error

"csch" "csch" "0"
compile_calculate
compile_error

"asinh" "asinh" "0"
compile_calculate
compile_error

"acosh" "acosh" "0"
compile_calculate
compile_error

"atanh" "atanh" "0"
compile_calculate
compile_error

"acoth" "acoth" "0"
compile_calculate
compile_error

"asech" "asech" "0"
compile_calculate
compile_error

"acsch" "acsch" "0"
compile_calculate
compile_error

"random" "random" "0"
compile_calculate
compile_error

"ceil" "ceil" "0"
compile_calculate
compile_error

"floor" "floor" "0"
compile_calculate
compile_error

"round" "round" "0"
compile_calculate
compile_error

"abs" "abs" "0"
compile_calculate
compile_error

"exp" "exp" "0"
compile_calculate
compile_error

"log" "log" "0"
compile_calculate
compile_error

"sqrt" "sqrt" "0"
compile_calculate
compile_error`