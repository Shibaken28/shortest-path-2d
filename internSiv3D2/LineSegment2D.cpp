
#pragma once

# include "Vector2D.cpp"
# include "Vector3D.cpp"

namespace MyGeometry {
	class LineSegment2D {
	private:
		Array<Vector2D> point;
	public:
		LineSegment2D(Vector2D p1, Vector2D p2) {
			point.clear();
			point.push_back(p1);
			point.push_back(p2);
		}
		LineSegment2D(double x0, double y0, double x1, double y1) {
			point.clear();
			point.push_back(Vector2D(x0, y0));
			point.push_back(Vector2D(x1, y1));
		}
		int intersects(LineSegment2D line) {
			Vector3D A(this->point[0]);
			Vector3D B(this->point[1]);
			Vector3D C(line.point[0]);
			Vector3D D(line.point[1]);
			Vector3D AB = B - A;
			Vector3D AC = C - A;
			Vector3D AD = D - A;
			Vector3D CD = D - C;
			Vector3D CA = A - C;
			Vector3D CB = B - C;
			double a = AB.cross(AC).z;
			double b = AB.cross(AD).z;
			double c = CD.cross(CA).z;
			double d = CD.cross(CB).z;
			/*
				様々なパターン
				0 跨いでいる(異符号である，0が存在していたら該当しない)
				1 ABに対してCDが正の方向 (0が存在してもよい)
				2 ABに対してCDが負の方向 (0が存在してもよい)
				3 完全に重なっている (全て0)
			*/
			if (a * b < 0 && c * d < 0) {
				return 0;
			}
			else if (a == 0 && b == 0 && c == 0 && d == 0) {
				return 3;
			}
			else if (a <= 0 && b <= 0) {
				return 1;
			}
			else {
				return 2;
			}
		}
		double getNorm() {
			double dx = this->point[0].x - this->point[1].x;
			double dy = this->point[0].y - this->point[1].y;
			return sqrt(dx * dx + dy * dy);
		}
		String getStr() {
			return U"({0},{1}),({2},{3})"_fmt(point[0].x, point[0].y, point[1].x, point[1].y);
		}
		void draw();

	};
}
