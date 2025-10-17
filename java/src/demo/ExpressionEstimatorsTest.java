package demo;

import estimator.ExpressionEstimator;

public class ExpressionEstimatorsTest {

	static String fromBoolean(boolean b) {
		return b ? "ok" : "error";
	}

	public static void main(String[] args) throws Exception {
		ExpressionEstimator e = new ExpressionEstimator();
		e.compile("a+b", new String[] { "a", "b" });
		System.out.println(e.calculate(1, 2));
		System.out.println(e.calculate(new double[] { 1, 2 }));
		ExpressionEstimator e1 = new ExpressionEstimator("a*b", new String[] { "a", "b" });
		System.out.println(e1.calculate(1, 2));
		System.out.println(e1.calculate(new double[] { 1, 2 }));
	}

}
