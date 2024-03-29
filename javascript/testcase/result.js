const ErrorCode = new Enum(
	'OK',
	'COMPILE_ERROR',
	'CALCULATE_ERROR'
);

class Result {

	// ErrorCode errorCode;
	// double value;

	//set(ErrorCode code, double v) {
	//set(String s, int line) {

	set(code, v = 0) {
		if (typeof code == 'string') {
			this.errorCode = ErrorCode[code.toUpperCase()];
			if (this.errorCode === undefined) {
				this.errorCode = ErrorCode.OK;
				this.value = parseFloat(code);
			}
		}
		else {
			this.errorCode = code;
			this.value = v;
		}
	}

	equals(a) {
		if (this.errorCode == ErrorCode.OK && a.errorCode == ErrorCode.OK) {
			return Math.abs(this.value - a.value) < 9e-16 || Number.isNaN(this.value) && Number.isNaN(a.value);
		}
		else {
			return a.errorCode == this.errorCode;
		}
	}

	toString() {
		if (this.errorCode == ErrorCode.OK) {
			return this.value + "";
		}
		else {
			return ErrorCode.toString(this.errorCode);
		}
	}

}
