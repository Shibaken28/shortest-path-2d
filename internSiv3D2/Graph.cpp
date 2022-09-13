#pragma once

namespace MyGeometry {
	class Edge {
	public:
		int to;
		double cost;
	public:
		Edge(int to, double cost) {
			this->to = to;
			this->cost = cost;
		}
	};

	struct costVertex {
		double cost;
		int vertex;
		costVertex(double c, int v) {
			cost = c;
			vertex = v;
		}
	};

	class Graph {
	public:
		int n = 0;
		Array<Array<Edge>> edges;
	public:
		Graph() {
			edges.clear();
		}
		void setSize(int s) {
			this->n = s;
			edges.clear();
			edges.resize(s);
		}
		void addEdge(int from, Edge e) {
			edges[from].push_back(e);
		}
		double Dijkstra(int begin, int end, Array<int>& path) {
			/*
			頂点番号がbeginからendまでの最短経路を求める
			*/
			std::priority_queue<costVertex, std::vector<costVertex>, std::function<bool(costVertex&, costVertex&)>> queue(
				[](costVertex& l, costVertex& r) {return l.cost < r.cost; }
			);
			Array<double> dis(n, 1e9);
			Array<int> pre(n, -1);//どこの頂点から来たか
			dis[begin] = 0;
			queue.push(costVertex(0, begin));
			while (!queue.empty()) {
				auto [cost, from] = queue.top();
				queue.pop();
				for (Edge e : edges[from]) {
					if (dis[from] + e.cost < dis[e.to]) {
						dis[e.to] = dis[from] + e.cost;
						queue.push(costVertex(dis[e.to], e.to));
						pre[e.to] = from;
					}
				}
			}
			path.clear();
			path.push_back(end);
			//経路復元
			while (path.back() >= 0) {
				path.push_back(pre[path.back()]);
			}
			path.pop_back();
			path.reverse();

			return dis[end];
		}

		void draw();

	};
}
