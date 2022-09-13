#pragma once

# include "Vector2D.cpp"
# include "Vector3D.cpp"
# include "Graph.cpp"
# include "Obstacle.cpp"
# include "PolygonalLine.cpp"

namespace MyGeometry {

	class Field {
	public:
		Vector2D begin, end;
		Graph graph;
		ObstacleManager obstacleManager;
		Array<Vector2D> allVertex;
		Array<LineSegment2D> allEdge;
		//bool updated;
		int having = -1;
		//Vec2 offset;
	public:

		Field() {
			obstacleManager.clear();
		}
		void movable() {
			Circle b(begin.x, begin.y, 10);
			Circle e(end.x, end.y, 10);

			if (b.mouseOver() || e.mouseOver()) {
				Cursor::RequestStyle(CursorStyle::Hand);
				FontAsset(U"P")(U"({0},{1})"_fmt(end.x, end.y)).draw(Arg::bottomCenter = Vec2(end.x, end.y), ColorF(0.1f));
				FontAsset(U"P")(U"({0},{1})"_fmt(begin.x, begin.y)).draw(Arg::bottomCenter = Vec2(begin.x, begin.y), ColorF(0.1f));
			}
			if (b.leftClicked()) {
				having = 0;
			}
			if (e.leftClicked()) {
				having = 1;
			}
			if (having != -1 && MouseL.pressed()) {
				if (having == 0) {
					begin.x += Cursor::Delta().x;
					begin.y += Cursor::Delta().y;
				}
				if (having == 1) {
					end.x += Cursor::Delta().x;
					end.y += Cursor::Delta().y;
				}
			}
			if (MouseL.up()) {
				having = -1;
			}
		}
		void setBeginEnd(Vector2D b, Vector2D e) {
			this->begin = b;
			this->end = e;
		}
		void buildGraph() {
			buildGraph(Vector2D(0, 0));
		}
		void buildGraph(Vector2D stretch) {
			allVertex.clear();
			allEdge.clear();
			allVertex.push_back(begin);
			allVertex.push_back(end);
			for (Obstacle*& obs : obstacleManager.obstacles) {
				auto obsv = obs->getVertex(stretch);
				allVertex.insert(allVertex.end(), obsv.begin(), obsv.end());
			}
			int n = (int)allVertex.size();//頂点数
			graph.setSize(n);
			for (int a = 0; a < n; a++) {
				//if (allVertex[a].x < 0 || allVertex[a].x >= Scene::Width() || allVertex[a].y < 0 || allVertex[a].y >= Scene::Height())continue;
				for (int b = a + 1; b < n; b++) {
					//if (allVertex[b].x < 0 || allVertex[b].x >= Scene::Width() || allVertex[b].y < 0 || allVertex[b].y >= Scene::Height())continue;
					bool intersects = false;//障害物と重なっているか
					LineSegment2D seg(allVertex[a], allVertex[b]);
					for (Obstacle*& obs : obstacleManager.obstacles) {
						if (obs->intersects(seg, stretch)) intersects = true;
					}

					if (not intersects) {
						double distance = seg.getNorm();
						Edge e1(b, distance);
						Edge e2(a, distance);
						graph.addEdge(a, e1);
						graph.addEdge(b, e2);
						allEdge.push_back(seg);
					}
				}
			}
		}

		double searchPath(PolygonalLine& polygonalLine) {
			polygonalLine.clear();
			Array<int> vertex;
			double d = graph.Dijkstra(0, 1, vertex);
			for (auto& v : vertex) {
				polygonalLine.addVertex(allVertex[v]);
			}
			return d;
		}

		void draw();
	};

}
