<?php
class ActionEnum
{
	const STATIC_COMPILE = 0;
	const COMPILE_CALCULATE = 1;
}
class Action
{
	public $action;

	public function set($s, $line)
	{
		if ($s == "static_compile") {
			$this->action = ActionEnum::STATIC_COMPILE;
		} else if ($s == "compile_calculate") {
			$this->action = ActionEnum::COMPILE_CALCULATE;
		} else {
			die("invalid string '$s' at line$line file:" . basename(__FILE__));
		}
	}

	public function __toString()
	{
		return $this->action == ActionEnum::STATIC_COMPILE ?
			"static_compile" : "compile_calculate";
	}
}
