<?php
include('node.php');

class ExpressionEstimator
{
	private $m_expression;
	private $m_tokenValue;
	private $m_operator;
	private $m_position;
	private $m_argument;
	private $m_arguments;
	private $m_root;

	private function isLetter()
	{
		return ctype_alpha($this->m_expression[$this->m_position]);
	}

	private function isDigit()
	{
		return ctype_digit($this->m_expression[$this->m_position]);
	}

	private function isPoint()
	{
		return $this->m_expression[$this->m_position] == '.';
	}

	private function isDigitOrPoint()
	{
		return $this->isDigit() || $this->isPoint();
	}

	private function isFunctionSymbol()
	{
		$c = $this->m_expression[$this->m_position];
		return ctype_alnum($c) || $c == '_';
	}

	private const CONSTANT_NAME = [
		"PI", "E", "SQRT2", "SQRT1_2", "LN2",
		"LN10", "LOG2E", "LOG10E"
	];
	private const CONSTANT_VALUE = [
		M_PI, M_E, M_SQRT2, M_SQRT1_2, M_LN2, M_LN10,
		M_LOG2E, M_LOG10E
	];

	private const FUNCTION = [
		"POW", "ATAN2", "MIN", "MAX", "SIN", "COS",
		"TAN", "COT", "SEC", "CSC", "ASIN", "ACOS", "ATAN", "ACOT", "ASEC",
		"ACSC", "SINH", "COSH", "TANH", "COTH", "SECH", "CSCH", "ASINH",
		"ACOSH", "ATANH", "ACOTH", "ASECH", "ACSCH", "RANDOM", "CEIL", "FLOOR",
		"ROUND", "ABS", "SIGN", "EXP", "LOG", "SQRT"
	];

	private const T = "+-*/()[]{},^";
	private const R = '#';

	private function getToken()
	{
		if ($this->m_position == strlen($this->m_expression)) {
			$this->m_operator = OPERATOR_ENUM::END;
		} else if (($p = strpos(self::T, $this->m_expression[$this->m_position])) !== false) {
			$this->m_position++;
			$this->m_operator = $p;
		} else if ($this->isLetter()) {
			for (
				$i = $this->m_position++;
				$this->m_position < strlen($this->m_expression) && $this->isFunctionSymbol();
				$this->m_position++
			);
			$token =  substr($this->m_expression, $i, $this->m_position - $i);

			if ($token[0] == 'X' && strlen($token) > 1 && ctype_digit($token[1])) {
				$j = (int)substr($token, 1);
				if ($this->m_arguments < $j + 1) {
					$this->m_arguments = $j + 1;
				}
				$this->m_operator = OPERATOR_ENUM::X;
				$this->m_tokenValue = $j;
			} else {
				$i = array_search($token, self::FUNCTION);
				if ($i === false) {
					$i = array_search($token, self::CONSTANT_NAME);
					if ($i === false) {
						throw new Error("unknown keyword \"$token\"");
					}
					$this->m_tokenValue = self::CONSTANT_VALUE[$i];
					$this->m_operator = OPERATOR_ENUM::NUMBER;
				} else {
					$this->m_operator = $i + OPERATOR_ENUM::POW;
				}
			}
		} else if ($this->isDigitOrPoint()) {
			$this->m_operator = OPERATOR_ENUM::NUMBER;
			$p = substr($this->m_expression, $this->m_position);
			//binary, octal, hex numbers
			if (
				$p[0] == '0' && $this->m_position + 1 < strlen($this->m_expression)
				&& ($n = strpos("BOX", $p[1])) !== false
			) {
				$r = [2, 8, 16][$n];
				$er = false;
				$token = ['', ''];
				$i = 0;
				for (
					$this->m_position += 2;
					$this->m_position < strlen($this->m_expression) && ($this->isFunctionSymbol() || $this->isPoint());
					$this->m_position++
				) {
					if ($this->isPoint()) {
						if (++$i > 1) {
							$er = true;
							break;
						}
					} else {
						$c = $this->m_expression[$this->m_position];
						//intval("1ff",8)=1
						if ($r == 16 && !preg_match("/[\dA-F]/", $c) || $r != 16 && ord($c) - ord('0') >= $r) {
							$er = true;
							break;
						}
						$token[$i] .= $c;
					}
				}
				if ($er || (empty($token[0]) && empty($token[1]))) {
					throw new Error("invalid " . ["binary", "octal", "hex"][$n]	. " constant");
				}
				$this->m_tokenValue = 0;
				for ($i = 0; $i < 2; $i++) {
					if (!empty($token[$i])) {
						$d = $i ? $r ** strlen($token[1]) : 1;
						$this->m_tokenValue += intval($token[$i], $r) / $d;
					}
				}
			} else {
				$c = 0;
				for ($i = $this->m_position++; $this->m_position < strlen($this->m_expression) && ($this->isDigitOrPoint() || $this->m_expression[$this->m_position] == 'E'
					|| $this->m_expression[$this->m_position - 1] == 'E' && in_array($this->m_expression[$this->m_position], ["+", "-"])); $this->m_position++) {
					if ($this->isPoint()) {
						if (++$c > 1) {
							break;
						}
					}
				}
				if ($c > 1) {
					throw new Error("invalid number");
				}

				$this->m_tokenValue = (float)substr($this->m_expression, $i, $this->m_position - $i);
			}
		} else {
			throw new Error("unknown symbol " . $this->m_expression[$this->m_position]);
		}
	}

	private function parse()
	{
		$node = $this->parse1();
		while (
			$this->m_operator == OPERATOR_ENUM::PLUS
			|| $this->m_operator == OPERATOR_ENUM::MINUS
		) {
			$node = new Node($this, $this->m_operator, $node);
			$this->getToken();
			if (
				$this->m_operator == OPERATOR_ENUM::PLUS
				|| $this->m_operator == OPERATOR_ENUM::MINUS
			) {
				throw new Error("two operators in a row");
			}
			$node->m_right = $this->parse1();
		}
		return $node;
	}

	private function parse1()
	{
		$node = $this->parse2();
		while (
			$this->m_operator == OPERATOR_ENUM::MULTIPLY
			|| $this->m_operator == OPERATOR_ENUM::DIVIDE
			|| $this->m_operator == OPERATOR_ENUM::POW
		) {
			$node = new Node($this, $this->m_operator, $node);
			$this->getToken();
			if (
				$this->m_operator == OPERATOR_ENUM::PLUS
				|| $this->m_operator == OPERATOR_ENUM::MINUS
			) {
				throw new Error("two operators in a row");
			}
			$node->m_right = $this->parse2();
		}
		return $node;
	}

	private function parse2()
	{
		if ($this->m_operator == OPERATOR_ENUM::MINUS) {
			$this->getToken();
			$node = new Node($this, OPERATOR_ENUM::UNARY_MINUS, $this->parse3());
		} else {
			if ($this->m_operator == OPERATOR_ENUM::PLUS) {
				$this->getToken();
			}
			$node = $this->parse3();
		}
		return $node;
	}

	private function parse3()
	{
		if ($this->m_operator >= OPERATOR_ENUM::POW && $this->m_operator <= OPERATOR_ENUM::SQRT) {
			if ($this->m_operator <= OPERATOR_ENUM::MAX) {
				$arguments = 2;
			} else {
				$arguments = $this->m_operator == OPERATOR_ENUM::RANDOM ? 0 : 1;
			}

			$node = new Node($this, $this->m_operator);
			$this->getToken();
			$open = $this->m_operator;
			if (
				$this->m_operator != OPERATOR_ENUM::LEFT_BRACKET
				&& $this->m_operator != OPERATOR_ENUM::LEFT_SQUARE_BRACKET
				&& $this->m_operator != OPERATOR_ENUM::LEFT_CURLY_BRACKET
			) {
				throw new Error("open bracket expected");
			}
			$this->getToken();

			if ($arguments > 0) {
				$node->m_left = $this->parse();

				if ($arguments == 2) {
					if ($this->m_operator != OPERATOR_ENUM::COMMA) {
						throw new Error("comma expected");
					}
					$this->getToken();
					$node->m_right = $this->parse();
				}
			}
			$this->checkBracketBalance($open);
		} else {
			switch ($this->m_operator) {

				case OPERATOR_ENUM::X:
				case OPERATOR_ENUM::NUMBER:
					$node = new Node($this, $this->m_operator, $this->m_tokenValue);
					break;

				case OPERATOR_ENUM::LEFT_BRACKET:
				case OPERATOR_ENUM::LEFT_SQUARE_BRACKET:
				case OPERATOR_ENUM::LEFT_CURLY_BRACKET:
					$open = $this->m_operator;
					$this->getToken();
					$node = $this->parse();
					$this->checkBracketBalance($open);
					break;

				default:
					throw new Error("unexpected operator");
			}
		}
		$this->getToken();
		return $node;
	}

	private static function calculate_static($s)
	{
		if (empty($s)) {
			throw new Error("use calculate without expression");
		}
		$estimator = new ExpressionEstimator();
		$estimator->compile($s[0]);
		$estimator->m_argument = NULL; //clear all arguments
		return $estimator->calculate_non_static();
	}

	function compile($expression, ...$variables)
	{
		$variables = self::modifyArguments($variables);
		$v = $variables;
		$s = preg_replace("/\\s+/", "", $expression);
		$s = preg_replace("/\\*{2}/", "^", $s);
		if (strpos($s, self::R) !== false) {
			throw new Error(self::R . " found in string");
		}
		if (!empty($v)) {
			sort($v);
			$i = 0;
			foreach ($v as $a) {
				$t = strtoupper($a);
				if (array_search($t, self::CONSTANT_NAME) !== false || array_search($t, self::FUNCTION) !== false) {
					throw new Error(
						"reserved word \"$t\" is used as variable"
					);
				}
				//also check empty
				if (!preg_match("~^_*[A-Za-z]\\w*$~", $a)) {
					throw new Error("invalid variable name \"$a\"");
				}
				if ($i > 0 && $a == $v[$i - 1]) {
					throw new Error("repeated variable \"$a\" in list");
				}
				$i++;
			}

			$i = 0;
			foreach ($variables as $e) {
				$s = preg_replace(self::R . "\\b$e\\b" . self::R, self::R . $i, $s);
				$i++;
			}

			if (preg_match(self::R . "\\b[xX]\\d*\\b" . self::R, $s, $m)) {
				throw new Error("unknown variable \"$m[0]\"");
			}

			$s = preg_replace("~" . self::R . "~", 'X', $s);
		}
		$this->m_position = 0;
		$this->m_arguments = count($v);
		$this->m_expression = strtoupper($s);
		$this->m_root = NULL;
		$this->m_argument = NULL;

		foreach ($variables as $e) {
			if (!is_string($e)) {
				throw new Error("variable \"$e\" is not a string");
			}
		}

		$this->getToken();
		if ($this->m_operator == OPERATOR_ENUM::END) {
			throw new Error("unexpected end of expression");
		}
		$this->m_root = $this->parse();
		if ($this->m_operator != OPERATOR_ENUM::END) {
			throw new Error("end of expression expected");
		}
	}

	private function calculate_non_static($values = [])
	{
		$this->m_argument = self::modifyArguments($values);

		if ($this->m_root == NULL) {
			throw new Error("using of calculate() without compile()");
		}

		if (count($values) != $this->m_arguments) {
			throw new Error("invalid number of expression arguments");
		}
		return $this->m_root->calculate();
	}

	private function checkBracketBalance($open)
	{
		if (($open == OPERATOR_ENUM::LEFT_BRACKET
				&& $this->m_operator != OPERATOR_ENUM::RIGHT_BRACKET)
			|| ($open == OPERATOR_ENUM::LEFT_SQUARE_BRACKET
				&& $this->m_operator != OPERATOR_ENUM::RIGHT_SQUARE_BRACKET)
			|| ($open == OPERATOR_ENUM::LEFT_CURLY_BRACKET
				&& $this->m_operator != OPERATOR_ENUM::RIGHT_CURLY_BRACKET)
		) {
			throw new Error(
				"close bracket expected or another type of close bracket"
			);
		}
	}

	function __construct(...$p)
	{
		if (!empty($p)) {
			$this->compile(...$p);
		}
	}

	function __call($name, $arguments)
	{
		if ($name === 'calculate') {
			return $this->calculate_non_static(self::modifyArguments($arguments));
			//return call_user_func([$this, 'calculate_non_static'], self::modifyArguments($arguments));
		} else {
			throw new Error();
		}
	}

	static function __callStatic($name, $arguments)
	{
		if ($name === 'calculate') {
			return self::calculate_static($arguments);
			//return call_user_func(['ExpressionEstimator', 'calculate_static'], self::modifyArguments($arguments));
		} else {
			throw new Error();
		}
	}

	//for Node class need public function
	function getArgument($n)
	{
		return $this->m_argument[$n];
	}

	private static function modifyArguments($arguments)
	{
		if (!empty($arguments) && is_array($arguments[0])) {
			$arguments = $arguments[0];
		}
		return $arguments;
	}
}
