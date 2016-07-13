/****
	class ClassA {
		public ClassA();
		public string toString();
		public string echo(str = "echo");
	}
****/
var ClassA = (function () {
	function ClassA() {
		console.log("ClassA::ClassA()");
	}
	ClassA.prototype.toString = function () {
		return "class ClassA";
	};
	ClassA.prototype.echo = function(str) {
		if (typeof str === "undefined") str = "echo";
		return "(" + str + ")";
	};
	return ClassA;
})();

/****
	namespace kjm {
		class ClassA {
			public ClassA();
			public string toString();
			public string echo(str = "echo");
		}
	}
****/
var kjm;
(function (kjm) {
	var ClassA = (function () {
		function ClassA() {
			console.log("kjm::ClassA::ClassA()");
		}
		ClassA.prototype.toString = function () {
			return "class kjm::ClassA";
		}
		ClassA.prototype.echo = function(str) {
			if (typeof str === "undefined") str = "echo";
			return "[" + str + "]";
		}
		return ClassA;
	})();
	kjm.ClassA = ClassA;
})(kjm || (kjm = {}));
