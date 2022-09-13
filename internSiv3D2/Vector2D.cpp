#pragma once

namespace MyGeometry {
	class Vector2D {
	public:
		double x, y;
	public:
		Vector2D() {
			this->x = 0;
			this->y = 0;
		}
		Vector2D(double x, double y) {
			this->x = x;
			this->y = y;
		}
		Vector2D(double x1, double y1, double x2, double y2) {
			this->x = x2 - x1;
			this->y = y2 - y1;
		}
		void operator = (Vector2D right) {
			this->x = right.x;
			this->y = right.y;
		}
		Vector2D operator - (Vector2D right) {
			return Vector2D(this->x - right.x, this->y - right.y);
		}
		Vector2D operator + (Vector2D right) {
			return Vector2D(this->x + right.x, this->y + right.y);
		}
		void operator += (Vector2D right) {
			this->x += right.x;
			this->y += right.y;
		}
		void operator -= (Vector2D right) {
			this->x -= right.x;
			this->y -= right.y;
		}
		Vector2D operator * (double right) {
			return Vector2D(this->x * right, this->y * right);
		}
		void operator = (double right) {
			this->x = right;
			this->y = right;
		}
		Vector2D rotate(double theta) {
			Vector2D rotated(0, 0);
			rotated.x = this->x * Cos(theta) - this->y * Sin(theta);
			rotated.y = this->x * Sin(theta) + this->y * Cos(theta);
			return rotated;
		}
		double getAngle(Vector2D v) {
			double a = this->x * v.x + this->y * v.y;
			double b = this->getNorm() * v.getNorm();
			return Acos(a / b);
		}
		double getNorm() {
			double dx = this->x;
			double dy = this->y;
			return sqrt(dx * dx + dy * dy);
		}
		Vector2D getUnit() {
			double n = this->getNorm();
			return Vector2D((this->x) / n, (this->y) / n);
		}
		void draw();

	};
}
