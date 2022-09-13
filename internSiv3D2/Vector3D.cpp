#pragma once

# include "Vector2D.cpp"

namespace MyGeometry {
	class Vector3D {
	public:
		double x, y, z;
	public:
		Vector3D(Vector2D p) {
			this->x = p.x;
			this->y = p.y;
			this->z = 0;
		}
		Vector3D(double x, double y, double z) {
			this->x = x;
			this->y = y;
			this->z = z;
		}
		Vector3D(double x0, double y0, double z0, double x1, double y1, double z1) {
			this->x = x1 - x0;
			this->y = y1 - y0;
			this->z = z1 - z0;
		}
		void operator = (Vector3D right) {
			this->x = right.x;
			this->y = right.y;
			this->z = right.z;
		}
		Vector3D operator - (Vector3D right) {
			return Vector3D(right.x - this->x, right.y - this->y, right.z - this->z);
		}
		//外積
		Vector3D cross(const Vector3D& a) {
			return Vector3D(this->y * a.z - this->z * a.y, this->z * a.x - this->x * a.z, this->x * a.y - this->y * a.x);
		}
	};
}

