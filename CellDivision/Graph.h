#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <climits>
#include "Vertex.h"
#include "Edge.h"
#include "BinaryHeapPQ.h"

using namespace std;

class Edge;
class Vertex;

class Graph {

private:
	vector<Vertex*> vertList;
	vector<Edge*> edgeList;
public:
	Graph(vector<Vertex*> vl, vector<Edge*> el) {
		vertList = vl;
		edgeList = el;
	}

	Graph(string filename) {
		ifstream ifs(filename.c_str());
		int vertnum;
		ifs >> vertnum;
		for (int i = 0; i<vertnum; i++) // push all vertices into the vertex vector before assigning the in and out list
		{
			Vertex *vert = new Vertex(i + 1);
			vertList.push_back(vert);
		}
		int svert, evert, weit;
		ifs >> svert;
		while (svert != -1) {
			ifs >> evert;
			while (evert != -1) {
				ifs >> weit; // one weight means one edge
				Edge *tempedge = new Edge(vertList[svert - 1], vertList[evert - 1], weit); // new edge
				edgeList.push_back(tempedge);
				vertList[svert - 1]->outList.push_back(tempedge);
				vertList[evert - 1]->inList.push_back(tempedge);
				ifs >> evert;
			}
			ifs >> svert;
		}
		ifs.close();
	}
	~Graph() {
		for (int i = 0; i < vertList.size(); i++)
			delete vertList[i];
		for (int i = 0; i < edgeList.size(); i++)
			delete edgeList[i];
	}
	vector<Vertex*> getVertices() const {
		return vertList;
	}


	friend ostream& operator<<(ostream& out, Graph& g);

	// print out all the edges and associated vertices in the entire graph
	void PrintGraph(string stream) {
		if (stream == "ofs") {
			string filename = "graph_print_out.txt";
			ofstream ofs(filename.c_str());
			ofs << *this;
			cout << "Graph printed to graph_print_out.txt" << endl;
		}
		else if (stream == "cout") cout << endl << *this << endl;
		else {
			cout << "Invalid print destination, graph not printed!" << endl;
		}
	}

	int addVertex() {
		Vertex* v = new Vertex(vertList.size() + 1);
		vertList.push_back(v);
		return v->id;
	}

	void addEdge(int s, int e, int w) {
		if (s < 1 || s > vertList.size()
			|| e < 1 || e > vertList.size()) throw VertexNotInGraph();
		Edge* edge = new Edge(vertList[s - 1], vertList[e - 1], w);
		vertList[s - 1]->outList.push_back(edge);
		vertList[e - 1]->inList.push_back(edge);
	}

	void listAllVertices() {
		cout << "Vertices in Graph: ";
		for (int i = 0; i < vertList.size(); ++i) {
			cout << vertList[i]->id << " ";
		}
		cout << endl;
	}

	void listAdjacentVertices() {
		int v;
		cout << "Enter vertex to find adjacent vertices for: ";
		cin >> v;
		Vertex* vert = vertList[v - 1];
		cout << "Adjacent Vertices: ";
		for (int i = 0; i < vert->outList.size(); ++i) {
			cout << vert->outList[i]->eVertP->id << " ";
		}
		cout << endl;
	}

	vector<int> dijkstra(int start, int finish, string pq = "heap") {
		if (start < 1 || start > vertList.size()
			|| finish < 1 || finish > vertList.size()) throw VertexNotInGraph();

		vector<int> sp;

		BinaryHeapPQ<Vertex*> Q;
		if (pq == "array") {
			delete &Q;
			//ArrayPQ<Vertex*> Q;
		}
		else if (pq == "heap");
		else {
			cout << "Invalid PQ selection!" << endl;
			return sp;
		}

		vector<int> loc(vertList.size() + 1);
		vector<int> dist(vertList.size() + 1);
		vector<Vertex*> prev(vertList.size() + 1);
		int comparisons = 0;

		int key;
		for (int i = 0; i < vertList.size(); ++i) {
			if (vertList[i]->id == start) key = 0;
			else key = INT_MAX;

			loc[vertList[i]->id] = Q.insert(key, vertList[i]);
			dist[vertList[i]->id] = key;
			prev[vertList[i]->id] = NULL;
		}

		while (!Q.isEmpty()) {
			Vertex* v = Q.minElement();
			int distance = Q.minKey();
			Q.remove(loc[v->id]);

			vector<Edge*> edges = v->getOutEdges();
			for (int i = 0; i < edges.size(); ++i) {
				++comparisons;
				int alt = distance + edges[i]->getWeight();
				if (alt < dist[edges[i]->geteVertP()->id]) {
					dist[edges[i]->geteVertP()->id] = alt;
					prev[edges[i]->geteVertP()->id] = v;
					Q.decreaseKey(loc[edges[i]->geteVertP()->id], alt);
					++comparisons;
				}
			}
		}

		if (dist[finish] == INT_MAX) throw NoPath();

		//cout << "Number of Comparisons: " << comparisons << endl;
		//cout << "Shortest distance from vertex " << start << " to vertex "
			//<< finish << " is " << dist[finish] << endl;
		//cout << "Shortest path from vertex " << start << " to vertex "
		//	<< finish << " is (start vertex --(weight)--> end vertex)" << endl;

		Vertex* vert = prev[finish];
		vector<Vertex*> path;
		vector<int> weight;
		while (vert->id != start) {
			path.push_back(vert);
			vert = prev[vert->id];
		}
		path.push_back(vert);
		reverse(path.begin(), path.end());
		Vertex *temp_finish = new Vertex(finish);
		path.push_back(temp_finish);



		for (int i = 0; i < path.size() - 1; ++i) {
			for (int j = 0; j < path[i]->outList.size(); ++j) {
				if (path[i]->outList[j]->eVertP->id == path[i + 1]->id) {
					weight.push_back(path[i]->outList[j]->weight);
					break;
				}
			}
		}

		cout << "Shortest Path: ";
		for (int i = 0; i < path.size(); ++i) {
			cout << path[i]->id-1;
			sp.push_back(path[i]->id - 1);
			if (i < path.size() - 1) {
				cout << " --> ";
			}
		}
		cout << endl;

		return sp;
	}
};

ostream& operator<<(ostream& out, Graph& g) {
	out << g.vertList.size() << endl;
	for (int i = 0; i < g.vertList.size(); ++i) {
		out << g.vertList[i]->getID() << " ";
		for (int j = 0; j < g.vertList[i]->getOutEdges().size(); ++j) {
			out << g.vertList[i]->getOutEdges()[j]->geteVertP()->getID() << " " << g.vertList[i]->getOutEdges()[j]->getWeight() << " ";
		}
		out << "-1" << endl;

	}
	out << "-1" << endl;
	return out;
}

#endif
