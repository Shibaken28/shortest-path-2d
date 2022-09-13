#pragma once

# include <Siv3D.hpp> // OpenSiv3D v0.6.2
# include "Vector2D.cpp"
# include "MySpline.cpp"
# include "Vector3D.cpp"
# include "PolygonalLine.cpp"
# include "LineSegment2D.cpp"
# include "Graph.cpp"
# include "Obstacle.cpp"
# include "Field.cpp"


namespace MyGeometry {
	void LineSegment2D::draw() {
		Line(point[0].x, point[0].y, point[1].x, point[1].y).draw(2,Palette::Black);
		
	}

	void Vector2D::draw() {
		Circle(x, y, 10).draw(Palette::Green);
	}

	void Rectangle::draw() const {
		Rect rect((int)leftUpper.x, (int)leftUpper.y, (int)size.x, (int)size.y);
		rect.draw(ColorF(0.1f, 0.1f, 0.9f, 0.1f)).drawFrame(3, 0, Palette::Blue);
	}

	void Polygon::draw() const {
		auto segment = this->getLineSegment();
		for (auto&seg:segment) {
			seg.draw();
		}
	}

	void Field::draw() {
		for (Obstacle*& obs : obstacleManager.obstacles) {
			obs->draw();
		}
		/*
		for (LineSegment2D& seg : allEdge) {
			seg.draw();
		}*/
		begin.draw();
		end.draw();
		
		
	}

}


void Main()
{
	FontAsset::Register(U"P", 40, Typeface::Medium);
	FontAsset::Register(U"pico", 20, Typeface::Medium);

	// 背景の色を設定 | Set background color
	Scene::SetBackground(ColorF{ 0.8, 0.9, 1.0 });
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::SetStyle(WindowStyle::Sizable);
	MyGeometry::Vector2D begin(100, 100), end(700, 500);
	MyGeometry::Field field;
	field.setBeginEnd(begin, end);

	/*
	MyGeometry::Polygon rect7;
	int x = 600, y = 60;
	rect7.addVertex(MyGeometry::Vector2D(x+80, y+ 0));
	rect7.addVertex(MyGeometry::Vector2D(x - 40, y + 450));
	rect7.addVertex(MyGeometry::Vector2D(x + 120, y + 400));
	//field.obstacleManager.addObstacle(&rect7);
	
	MyGeometry::Polygon rect8;
	x = 100; y = 100;
	rect8.addVertex(MyGeometry::Vector2D(x + 150, y + 0));
	rect8.addVertex(MyGeometry::Vector2D(x + 75, y + 100));
	rect8.addVertex(MyGeometry::Vector2D(x + 190, y + 130));
	rect8.addVertex(MyGeometry::Vector2D(x + 450, y + 110));
	rect8.addVertex(MyGeometry::Vector2D(x + 250, y + 0));
	//field.obstacleManager.addObstacle(&rect8);
	*/	


	MyGeometry::Rectangle rect6(MyGeometry::Vector2D(20, 280), MyGeometry::Vector2D(430, 180));

	
	MyGeometry::LineSegment2D a(0, 0, 100, 100);
	MyGeometry::LineSegment2D b(0, 0, 100, 200);
	MyGeometry::PolygonalLine line;

	MyGeometry::Vector2D ArmSize(10,0);

	bool automatic = true;
	bool cameraOn = true;
	bool spline = false;
	bool app = false;

	Camera2D camera{ Scene::Center(), 1.0 };

	double d = -1;

	double splined = 10;

	int time = 0;
	bool tansaku = false;


	while (System::Update())
	{
		ClearPrint();
		camera.update();
		{

			const auto t = camera.createTransformer();
			if (not cameraOn) {
				camera.setTargetCenter(Vec2{ 400, 300 });
				camera.setTargetScale(1.0);
			}
			field.obstacleManager.mouse();

			field.draw();

			if (spline) {
				line.drawSpline(ArmSize.x * 2 + 1, splined);
			}
			else {
				line.draw(ArmSize.x * 2 + 1);
			}
			line.mouseOver();

			field.movable();

		}
		if (camera.getTargetScale() > 3.0) {
			camera.setTargetScale(3.0);
		}
		if (camera.getTargetScale() < 0.25f) {
			camera.setTargetScale(0.25f);
		}

		SimpleGUI::CheckBox(app, U"隠す", Vec2{ Scene::Width() - 95 , Scene::Height() - 50 });
		if (not app) {


			if (SimpleGUI::Button(U"初期位置に移動", Vec2{ 20, Scene::Height() - 90 }))
			{
				// 中心とズームアップ倍率の目標値をセットして、時間をかけて変更する
				camera.setTargetCenter(Scene::Center());
				camera.setTargetScale(1.0);
			}
			if (SimpleGUI::Button(U"障害物を追加", Vec2{ 210, Scene::Height() - 90 }, unspecified,field.obstacleManager.obstacles.size()<50))
			{
				MyGeometry::Rectangle* rect = new MyGeometry::Rectangle(MyGeometry::Vector2D(RandomUint16() % 550 + 50, RandomUint16() % 350 + 50), MyGeometry::Vector2D(100, 100));
				field.obstacleManager.addObstacle(rect);
			}
			SimpleGUI::Slider(U"直径 {:.1f}"_fmt(ArmSize.x * 2), ArmSize.x, 0.0, 40.0, Vec2{ 390,Scene::Height() - 90 }, 100, 100);
			ArmSize.y = ArmSize.x;

			SimpleGUI::CheckBox(automatic, U"リアルタイム探索", Vec2{ 20, Scene::Height() - 50 });


			tansaku = SimpleGUI::Button(U"経路探索", Vec2{ 240,  Scene::Height() - 50 }, unspecified, not automatic);

			SimpleGUI::CheckBox(spline, U"曲線(β)", Vec2{ 400, Scene::Height() - 50 });
			SimpleGUI::Slider(U"滑らかさ {}"_fmt((int)splined), splined, 1, 9, Vec2{ 520, Scene::Height() - 50 }, 100, 50, spline);
			splined = (int)splined;
		}

		if(tansaku || automatic)
		{
			Stopwatch stopwatch;
			stopwatch.start();
			field.buildGraph(ArmSize);
			d = field.searchPath(line);
			time = stopwatch.ms();
			if (time > 50)automatic = false;
		}

		if (d >= 0 && d < 1e9 && not spline) {
			FontAsset(U"P")(U"長さ = {:.1f}"_fmt(d)).draw(0, 0, ColorF{ 0.9f,0.1f,0.1f });
		}
		else {
			//FontAsset(U"P")(U"経路なし").draw(0, 0, ColorF{ 0.9f,0.1f,0.1f });
		}


		FontAsset(U"pico")(U"処理 {}[ms]"_fmt(time)).draw(Arg::topRight = Vec2(Scene::Width(), 0), ColorF{ 0.1f,0.1f,0.1f });
		FontAsset(U"pico")(U"障害物数 {}"_fmt(field.obstacleManager.obstacles.size())).draw(Arg::topRight = Vec2(Scene::Width(), 24), ColorF{ 0.1f,0.1f,0.1f });
		camera.draw(Palette::Orange);
	}
}

