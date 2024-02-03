<?php
// http://localhost/1/testcase.php
include('testcase/case.php');

echo "<p style='white-space:pre'>";

$q = [];
$r = [0, 0];
$li = [];
$t = new TCase();

$i = 0;
$line = 0;
$s = file_get_contents("testcases.txt");
$a = explode("\r\n", $s);
foreach ($a as $s) {
	$line++;
	//empty("0")=true
	if (!strlen($s)) {
		continue;
	}
	$q[$i] = $s;
	$li[$i] = $line;
	if ($i == 2) {
		$t->set($q, $li);
		$r[(int)$t->test()]++;
		$i = 0;
	} else {
		$i++;
	}
}

$j = $r[0] + $r[1];
$k = floor(log10($j)) + 1;
for ($i = 1; $i >= 0; $i--) {
	printf(
		"%-5s %" . $k . "d/%d=%5.1lf%%<br>",
		$i ? "ok" : "error",
		$r[$i],
		$j,
		$r[$i] * 100. / $j
	);
}
