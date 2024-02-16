<?php
class Data
{
	public $compile;
	public $variables;
	public $values;
	function set($s, $line)
	{
		$s = trim($s);
		if ($s[0] != '"' || substr($s, -1) != '"') {
			die("invalid pattern " . basename(__FILE__) . " line" . __LINE__);
		}
		$a = preg_split("~\"\\s+\"~", substr($s, 1, -1));
		$this->compile = $a[0];
		if (count($a) > 1) {
			$this->variables = strlen($a[1]) == 0 ? [] : preg_split("~\\s+~", $a[1]);
			if (count($a) > 2) {
				$this->values = preg_split("~\\s+~", $a[2]);
			}
		}
		else{
			$this->variables = [];
		}
	}
	public function __toString()
	{
		$s = "compile=" . $this->compile;
		if (!empty($this->variables)) {
			$s .= " variables=" . implode(' ', $this->variables) . " values=" . implode(' ', $this->values);
		}
		return $s;
	}
};
