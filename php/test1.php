<?php
include('estimator/expressionEstimator.php');

$v = ExpressionEstimator::calculate("sin(pi/4)");
echo "$v<br>";
$v = ExpressionEstimator::calculate("pow( sin(pi/10)*4+1 , 2)");
echo "$v<br>";
