<?php
// http://localhost/1/test.php

include('estimator/expressionEstimator.php');

$e = new ExpressionEstimator();
$e->compile('x0b*2');
$v = $e->calculate(1);
echo "$v<br>";

$e->compile('X0b*2');
$v = $e->calculate(1);
echo "$v<br>";

