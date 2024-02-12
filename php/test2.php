<?php
include('estimator/expressionEstimator.php');

$e = new ExpressionEstimator();
$e->compile('x0+2*sin(pi*x1)');
$v = $e->calculate(1, .25);
echo "$v<br>";
$v = $e->calculate([1, 1 / 6]);
echo "$v<br>";

$e->compile('x0+2*X1'); //case insensitive variable names
$v = $e->calculate(1, .25);
echo "$v<br>";

$e = new ExpressionEstimator('x0+2*x1');
$v = $e->calculate(1, 3);
echo "$v<br>";

$e->compile('a+2*b', 'a', 'b'); //or $e->compile('a+2*b',['a','b']);
$v = $e->calculate(1, 1);
echo "$v<br>";

$e = new ExpressionEstimator('a+2*A', 'a', 'A'); //case sensitive variable names
//or $e = new ExpressionEstimator('a+2*A',['a','A']);
$v = $e->calculate(1, 2);
echo "$v<br>";
