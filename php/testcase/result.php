<?php
class ErrorCode
{
	const OK = 0;
	const COMPILE_ERROR = 1;
	const CALCULATE_ERROR = 2;
};

class Result
{
	const map = [
		"ok" => ErrorCode::OK,
		"compile_error" => ErrorCode::COMPILE_ERROR,
		"calculate_error" => ErrorCode::CALCULATE_ERROR
	];

	public $errorCode;
	public $value;
	//or set(code,value)
	function set($s, $line = 0)
	{
		if (is_string($s)) {
			if (array_key_exists($s, self::map)) {
				$this->errorCode = self::map[$s];
			} else {
				$this->errorCode = ErrorCode::OK;
				if (strcasecmp($s, "nan") == 0) {
					$s = NAN;
				} else if (!is_numeric($s)) {
					die("$s not numeric " . basename(__FILE__) . ":" . __LINE__);
				}
				$this->value = (float)$s;
			}
		} else {
			$this->errorCode = $s;
			$this->value = (float)$line;
		}
	}

	function equals($a)
	{
		if ($this->errorCode == ErrorCode::OK && $a->errorCode == ErrorCode::OK) {
			return abs($this->value - $a->value) < 9e-16
				|| (is_nan($this->value) && is_nan($a->value));
		} else {
			return $a->errorCode == $this->errorCode;
		}
	}

	public function __toString()
	{
		if ($this->errorCode == ErrorCode::OK) {
			return (string)$this->value;
		} else {
			return array_search($this->errorCode, self::map);
		}
	}
};
