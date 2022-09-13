#pragma once

# include "Vector2D.cpp"
# include "MySpline.cpp"

namespace MyGeometry {
	struct PolygonalLine {
		Array<Vector2D> vertex;
		PolygonalLine() {}
		void addVertex(Vector2D v) {
			vertex.push_back(v);
		}
		void clear() {
			vertex.clear();
		}
		void mouseOver() {
			for (int i = 1; i < (int)vertex.size() - 1; i++) {
				Circle circle(vertex[i].x, vertex[i].y, 10);
				if (circle.mouseOver()) {
					circle.draw(ColorF(0.1f));
					FontAsset(U"P")(U"({0},{1})"_fmt((int)vertex[i].x, (int)vertex[i].y)).draw(Arg::bottomLeft = Vec2(vertex[i].x, vertex[i].y), ColorF{ 0.1f,0.1f,0.1f });
				}
			}

		}
		void draw(int weight) {
			for (int i = 0; i < (int)vertex.size() - 1; i++) {
				Line(vertex[i].x, vertex[i].y, vertex[i + 1].x, vertex[i + 1].y).draw(LineStyle::RoundCap, weight, ColorF(0.9f, 0.1, 0.1f, 0.5f));
			}
		}
		void drawSpline(int weight, double div) {
			if (div == 0)return;
			int n = vertex.size();
			for (int i = 0; i < (int)vertex.size() - 1; i++) {
				int a0 = i;
				int am1 = Max(i - 1, 0);
				int a1 = Min(i + 1, n - 1);
				int a2 = Min(i + 2, n - 1);
				MySpline spx(vertex[am1].x, vertex[a0].x, vertex[a1].x, vertex[a2].x);
				MySpline spy(vertex[am1].y, vertex[a0].y, vertex[a1].y, vertex[a2].y);
				double dt = 1.0f / div;
				for (double t = 0; t < 1.0f; t += dt) {
					Line line(spx.cal(t), spy.cal(t), spx.cal(t + dt), spy.cal(t + dt));
					line.draw(LineStyle::RoundCap, weight, ColorF(0.9f, 0.1, 0.1f));
				}
			}
		}
	};
}
