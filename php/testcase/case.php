<?php
include('data.php');
include('action.php');
include('result.php');
//If you don't use __DIR__ then if you include this file from testcase.php the path will be incorrect.
include(__DIR__ . '/../estimator/expressionEstimator.php');

class TCase
{
	public $line;
	public $data;
	public $action;
	public $result;
	public $e;

	function __construct()
	{
		$this->data = new Data();
		$this->action = new Action();
		$this->result = new Result();
		$this->e = new ExpressionEstimator();
	}

	function set($s, $lines)
	{
		$this->line = $lines[0];
		$this->data->set($s[0], $lines[0]);
		$this->action->set($s[1], $lines[1]);
		$this->result->set($s[2], $lines[2]);
	}

	function test()
	{
		$r = new Result();
		if ($this->action->action == ActionEnum::STATIC_COMPILE) {
			try {
				$v = ExpressionEstimator::calculate($this->data->compile);
				$r->set(ErrorCode::OK, $v);
			} catch (Error $ex) {
				$r->set(ErrorCode::COMPILE_ERROR);
			}
		} else if ($this->action->action == ActionEnum::COMPILE_CALCULATE) {
			try {
				$this->e->compile($this->data->compile, $this->data->variables);
				try {
					$v = $this->e->calculate($this->data->values);
					$r->set(ErrorCode::OK, $v);
				} catch (Error $ex) {
					// echo $ex;
					$r->set(ErrorCode::CALCULATE_ERROR);
				}
			} catch (Error $ex) {
				// echo $ex;
				$r->set(ErrorCode::COMPILE_ERROR);
			}
		} else {
			die(basename(__FILE__) . __LINE__);
		}

		$b = $r->equals($this->result);
		if (!$b) {
			//r what we got here,result is data from file
			echo "$this->data $r file:$this->line $this->result<br>";
		}
		return $b;
	}
};
