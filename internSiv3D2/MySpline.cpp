#pragma once

namespace MyGeometry {
	struct MySpline {
		double a1, a2, a3, a4;
		MySpline(double fm1, double f0, double f1, double f2) {
			a4 = f0;
			a3 = (f1 - fm1) / 2;
			a1 = (f2 - f0) / 2 - 2 * f1 + a3 + 2 * a4;
			a2 = 3 * f1 - (f2 - f0) / 2 - 2 * a3 - 3 * a4;
		}
		double cal(double t) {
			return a1 * t * t * t + a2 * t * t + a3 * t + a4;
		}
	};
}
