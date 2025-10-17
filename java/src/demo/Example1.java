package demo;

import estimator.ExpressionEstimator;

public class Example1 {

	public static void main(String[] args) {
		final String a[] = { "sin(pi/4)", "pow( sin(pi/10)*4+1 , 2)", "(sqrt(28/27)+1)^(1/3)-(sqrt(28/27)-1)^(1/3)",
				"sqrt(28/3)*sin(asin( sqrt(243/343)) /3 )" };

		try {
			for (String e : a) {
				System.out.println(ExpressionEstimator.calculate(e));
			}
		} catch (Exception ex) {
			System.out.println(ex.getMessage());
		}

	}

}