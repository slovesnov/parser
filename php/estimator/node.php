<?php
include('operatorEnum.php');

class Node
{
	public $m_estimator;
	public $m_operator;
	public $m_value;
	public $m_left, $m_right;

	public function __construct($estimator, $_operator, $p=NULL)
	{
		if (is_numeric($p)) {
			$this->init($estimator, $_operator, $p, NULL);
		} else {
			$this->init($estimator, $_operator, 0, $p);
		}
	}

	function init(
		$estimator,
		$_operator,
		$value,
		$left
	) {
		$this->m_estimator = $estimator;
		$this->m_operator = $_operator;
		$this->m_value = $value;
		$this->m_left = $left;
		$this->m_right = NULL;
	}

	function calculate()
	{
		$l = $this->m_left ? $this->m_left->calculate() : 0;
		$r = $this->m_right ? $this->m_right->calculate() : 0;
		switch ($this->m_operator) {

			case OPERATOR_ENUM::NUMBER:
				return $this->m_value;

			case OPERATOR_ENUM::PLUS:
				return $l + $r;

			case OPERATOR_ENUM::MINUS:
				return $l - $r;

			case OPERATOR_ENUM::MULTIPLY:
				return $l * $r;

			case OPERATOR_ENUM::DIVIDE:
				return $l / $r;

			case OPERATOR_ENUM::UNARY_MINUS:
				return -$l;

			case OPERATOR_ENUM::SIN:
				return sin($l);

			case OPERATOR_ENUM::COS:
				return cos($l);

			case OPERATOR_ENUM::TAN:
				return tan($l);

			case OPERATOR_ENUM::COT:
				return 1 / tan($l);

			case OPERATOR_ENUM::SEC:
				return 1 / cos($l);

			case OPERATOR_ENUM::CSC:
				return 1 / sin($l);

			case OPERATOR_ENUM::ASIN:
				return asin($l);

			case OPERATOR_ENUM::ACOS:
				return acos($l);

			case OPERATOR_ENUM::ATAN:
				return atan($l);

			case OPERATOR_ENUM::ACOT:
				return M_PI / 2 - atan($l);

			case OPERATOR_ENUM::ASEC:
				return acos(1 / $l);

			case OPERATOR_ENUM::ACSC:
				return asin(1 / $l);

			case OPERATOR_ENUM::SINH:
				return sinh($l);

			case OPERATOR_ENUM::COSH:
				return cosh($l);

			case OPERATOR_ENUM::TANH:
				return tanh($l);

			case OPERATOR_ENUM::COTH:
				return 1 / tanh($l);

			case OPERATOR_ENUM::SECH:
				return 1 / cosh($l);

			case OPERATOR_ENUM::CSCH:
				return 1 / sinh($l);

			case OPERATOR_ENUM::ASINH:
				return asinh($l);

			case OPERATOR_ENUM::ACOSH:
				return acosh($l);

			case OPERATOR_ENUM::ATANH:
				return atanh($l);

			case OPERATOR_ENUM::ACOTH:
				return atanh(1 / $l);

			case OPERATOR_ENUM::ASECH:
				return acosh(1 / $l);

			case OPERATOR_ENUM::ACSCH:
				return asinh(1 / $l);

			case OPERATOR_ENUM::RANDOM:
				return mt_rand() / (mt_getrandmax() + 1);

			case OPERATOR_ENUM::CEIL:
				return ceil($l);

			case OPERATOR_ENUM::FLOOR:
				return floor($l);

			case OPERATOR_ENUM::ROUND:
				return round($l);

			case OPERATOR_ENUM::ABS:
				return abs($l);

			case OPERATOR_ENUM::EXP:
				return exp($l);

			case OPERATOR_ENUM::LOG:
				return log($l);

			case OPERATOR_ENUM::SQRT:
				return sqrt($l);

			case OPERATOR_ENUM::POW:
				return pow($l, $r);

			case OPERATOR_ENUM::ATAN2:
				return atan2($l, $r);

			case OPERATOR_ENUM::MIN:
				return min($l, $r);

			case OPERATOR_ENUM::MAX:
				return max($l, $r);

			case OPERATOR_ENUM::X:
				return $this->m_estimator->getArgument($this->m_value);

			default:
				throw new Exception('Node.calculate error');
		}
	}
}
