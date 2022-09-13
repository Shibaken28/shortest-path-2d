#pragma once

# include "Vector2D.cpp"
# include "LineSegment2D.cpp"

namespace MyGeometry {
	using ObstacleShape = std::variant<Rect>;

	class Obstacle {
	public:
		virtual Array<Vector2D> getVertex() const = 0;
		virtual Array<LineSegment2D> getLineSegment() const = 0;
		virtual Array<Vector2D> getVertex(Vector2D stretch) const = 0;
		virtual void moved(Vector2D) = 0;
		virtual void resized(int, Vector2D) = 0;
		virtual Array<LineSegment2D> getLineSegment(Vector2D stretch) const = 0;
		virtual void draw() const = 0;

		virtual ObstacleShape getShapge() const = 0;

		bool intersects(LineSegment2D seg, Vector2D stretch) {
			auto segments = getLineSegment(stretch);
			bool intersects = false;
			/*
				様々なパターン
				0 跨いでいる(異符号である，0が存在していたら該当しない)
				1 ABに対してCDが正の方向 (0が存在してもよい)
				2 ABに対してCDが負の方向 (0が存在してもよい)
				3 完全に重なっている (両方0)
				* 図形の境界線に当たり判定はない

				1つでも0があれば，明らかに重なっているのでtrue
				そのほかの場合，
				- 3は辺の上にのっているだけなのでセーフ
				- 全て1の場合のみ，完全に含まれているためtrue
				- その他2のみ，1と2が混在している -> 重なっていないのでfalse

			*/
			bool inner = true; // 1以外でてこない
			String de = U"";
			for (auto& seg2 : segments) {
				int judge = seg2.intersects(seg);
				if (judge == 0) {
					intersects = true;
				}
				if (judge != 1) {

					inner = false;
				}
			}
			if (inner)intersects = true;
			return intersects;
		}
	};

	class Rectangle : public Obstacle {
	public:
		Vector2D leftUpper; //左上の座標
		Vector2D size; //幅と高さ
	public:
		Rectangle(Vector2D lu, Vector2D size) {
			this->leftUpper = lu;
			this->size = size;
		}
		Array<Vector2D> getVertex() const override {
			return getVertex(Vector2D(0, 0));
		}

		Array<LineSegment2D> getLineSegment() const override {
			return getLineSegment(Vector2D(0, 0));
		}
		Array<Vector2D> getVertex(Vector2D stretch) const override {
			Array<Vector2D> ret;
			//反時計回り
			ret.push_back(Vector2D(leftUpper.x - stretch.x, leftUpper.y - stretch.y));
			ret.push_back(Vector2D(leftUpper.x - stretch.x, leftUpper.y + size.y + stretch.y));
			ret.push_back(Vector2D(leftUpper.x + size.x + stretch.x, leftUpper.y + size.y + stretch.y));
			ret.push_back(Vector2D(leftUpper.x + size.x + stretch.x, leftUpper.y - stretch.y));
			return ret;
		}

		Array<LineSegment2D> getLineSegment(Vector2D stretch) const override {
			Array<LineSegment2D> ret;
			Array<Vector2D> vertex = this->getVertex(stretch);

			for (int i = 0; i < vertex.size(); i++) {
				int j = (i + 1) % vertex.size();

				ret.push_back(LineSegment2D(vertex[i], vertex[j]));
			}
			return ret;
		}
		ObstacleShape getShapge() const override {
			return Rect(leftUpper.x, leftUpper.y, size.x, size.y);
		}
		void moved(Vector2D vec)override {
			this->leftUpper += vec;
		}

		void resized(int t, Vector2D vec) override {
			Vector2D ds(0, 0);
			Vector2D dlu(0, 0);
			if (t == 0) {
				ds -= vec;
				dlu += vec;
			}
			else if (t == 1) {
				ds.x = -vec.x;
				ds.y = vec.y;
				dlu.x = vec.x;
			}
			else if (t == 2) {
				ds.x = vec.x;
				ds.y = vec.y;
			}
			else if (t == 3) {
				ds.x = vec.x;
				ds.y = -vec.y;
				dlu.y = vec.y;
			}
			size.x += ds.x;
			if (size.x <= 20) {
				size.x = 20;
			}
			else {
				leftUpper.x += dlu.x;
			}

			size.y += ds.y;
			if (size.y <= 20) {
				size.y = 20;
			}
			else {
				leftUpper.y += dlu.y;
			}
		}

		void draw() const override;

	};

	class Polygon : public Obstacle {
	public:
		int n;
		Array<Vector2D> vertex;
	public:
		Polygon() {
			vertex.clear();
			n = 0;
		}
		void addVertex(Vector2D add) {
			n++;
			vertex.push_back(add);
		}
		Array<Vector2D> getVertex() const override {
			return getVertex(Vector2D(0, 0));
		}

		Array<LineSegment2D> getLineSegment() const override {
			return getLineSegment(Vector2D(0, 0));
		}
		Array<Vector2D> getVertex(Vector2D stretch) const override {
			Array<Vector2D> ret;
			double r = stretch.x;
			for (int i = 0; i < vertex.size(); i++) {
				Vector2D a = vertex[i];
				Vector2D b = vertex[(i + 1) % n];
				Vector2D c = vertex[(i - 1 + n) % n];
				Vector2D ab = b - a;
				Vector2D ac = c - a;
				double d = ab.getAngle(ac);
				double t = Math::Pi - d / 2;
				ab = ab.rotate(t).getUnit();
				ret.push_back(a + ab * (r / Cos(t - Math::Pi / 2)));
			}

			return ret;
		}

		Array<LineSegment2D> getLineSegment(Vector2D stretch) const override {
			Array<LineSegment2D> ret;
			Array<Vector2D> vertex = this->getVertex(stretch);

			for (int i = 0; i < n; i++) {
				int j = (i + 1) % n;
				ret.push_back(LineSegment2D(vertex[i], vertex[j]));
			}

			return ret;
		}
		ObstacleShape getShapge() const override {
			return Rect(0, 0, 1, 1);
		}
		void moved(Vector2D vec) override {

		}
		void resized(int, Vector2D vec) override {

		}

		void draw() const override;

	};



	class ObstacleManager {
	public:
		int having = -1;
		int draft = -1;
		int place = 0;
		Vec2 offset;
		Array<Obstacle*> obstacles;
		void clear() {
			obstacles.clear();
		}
		void addObstacle(Obstacle* obs) {
			obstacles.push_back(obs);
		}
		void addRectangle(Vector2D lu, Vector2D s) {
			obstacles.push_back(new Rectangle(lu, s));
		}
		void mouse() {
			bool mouseoverd = false;
			for (int i = 0; i < obstacles.size(); i++) {
				Obstacle*& obs = obstacles[i];
				ObstacleShape shape = obs->getShapge();
				if (std::holds_alternative<Rect>(shape)) {
					if (i == draft) {
						if (SimpleGUI::Button(U"delete", offset))
						{
							draft = -1;
							obstacles.erase(obstacles.begin() + i);
							break;
						}
					}

					// 保持しているRect型オブジェクトを取得
					Rect rect = std::get<Rect>(shape);
					if (rect.mouseOver() && not mouseoverd) {
						mouseoverd = true;
						Circle(rect.x, rect.y, 10).draw(ColorF(0.0f));
						FontAsset(U"P")(U"({0},{1})"_fmt(rect.x, rect.y)).draw(Arg::bottomLeft = Vec2(rect.x, rect.y), ColorF{ 0.1f,0.1f,0.1f });
						FontAsset(U"P")(U"横{0},縦{1}"_fmt(rect.w, rect.h)).draw(rect.x + rect.w, rect.y + rect.h, ColorF{ 0.1f,0.1f,0.1f });
					}
					else if (i == having) {
						mouseoverd = true;
						Circle(rect.x, rect.y, 10).draw(ColorF(0.0f));
						FontAsset(U"P")(U"({0},{1})"_fmt(rect.x, rect.y)).draw(Arg::bottomLeft = Vec2(rect.x, rect.y), ColorF{ 0.1f,0.1f,0.1f });
						FontAsset(U"P")(U"横{0},縦{1}"_fmt(rect.w, rect.h)).draw(rect.x + rect.w, rect.y + rect.h, ColorF{ 0.1f,0.1f,0.1f });
					}
					if (rect.leftClicked()) {
						place = 0;
						having = i;
						offset = Cursor::Pos();
					}
					if (rect.rightClicked()) {
						draft = i;
						offset = Cursor::Pos();
					}
					if (rect.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::Hand);
					}

					Circle circle(rect.x, rect.y, 15);
					if (circle.leftClicked()) {
						place = 1;
						having = i;
						offset = Cursor::Pos();
					}
					if (circle.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::ResizeNWSE);
					}

					Circle circle2(rect.x, rect.y + rect.h, 15);
					if (circle2.leftClicked()) {
						place = 2;
						having = i;
						offset = Cursor::Pos();
					}
					if (circle2.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::ResizeNESW);
					}

					Circle circle3(rect.x + rect.w, rect.y + rect.h, 15);
					if (circle3.leftClicked()) {
						place = 3;
						having = i;
						offset = Cursor::Pos();
					}
					if (circle3.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::ResizeNWSE);
					}


					Circle circle4(rect.x + rect.w, rect.y, 15);
					if (circle4.leftClicked()) {
						place = 4;
						having = i;
						offset = Cursor::Pos();
					}
					if (circle4.mouseOver()) {
						Cursor::RequestStyle(CursorStyle::ResizeNESW);
					}
				}
			}
			if (MouseL.pressed() && having != -1) {
				draft = -1;
				if (place == 0) {
					Vec2 v = Cursor::Pos() - offset;
					offset = Cursor::Pos();
					obstacles[having]->moved(Vector2D(v.x, v.y));
				}
				else if (place >= 1) {
					Vec2 v = Cursor::Pos() - offset;
					offset = Cursor::Pos();
					obstacles[having]->resized(place - 1, Vector2D(v.x, v.y));
				}
			}
			if (MouseL.up()) {
				having = -1;
			}
		}
	};

}
