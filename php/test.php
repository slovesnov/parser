<?php
// http://localhost/1/test.php

include('estimator/expressionEstimator.php');

$e = new ExpressionEstimator();
$e->compile('a+2*A', 'a', 'A');
$v = $e->calculate(1, 2);
echo "$v<br>";

$e->compile('a+2*A', ['a', 'A']);
$v = $e->calculate(1, 2);
echo "$v<br>";

$e->compile('a+2*A', 'a', 'A');
$v = $e->calculate([1, 2]);
echo "$v<br>";

$e->compile('a+2*A', ['a', 'A']);
$v = $e->calculate([1, 2]);
echo "$v<br>";
