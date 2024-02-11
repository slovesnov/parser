<?php
include('estimator/expressionEstimator.php');

$a = [
	'sin(pi/4)'
		, 'pow( sin(pi/10)*4+1 , 2)'
		, '(sqrt(28/27)+1)^(1/3)-(sqrt(28/27)-1)^(1/3)'
		, 'sqrt(28/3)*sin(asin( sqrt(243/343)) /3 )'
];

foreach ($a as $s) {
	echo ExpressionEstimator::calculate($s) . "<br>";
}