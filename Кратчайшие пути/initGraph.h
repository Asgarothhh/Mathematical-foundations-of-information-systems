#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <set>
using namespace std;

struct Edge
{
	int from;
	int to;
	int weight;
};

struct graphParameters
{
	int directed, weighted, self_loops;
	int Vmin, Vmax, Emin, Emax;
	int Wmin = 0, Wmax = 0;
};

void readData(string path, graphParameters& graph);

void savedAdjacencyList(vector<Edge> edgeList, int vertices, int directed, int weighted, string path);

void savedAdjacencyMatrix(vector<Edge>& edgeList, int vertices, int directed, int weighted, string path);

void generateGraph(int& vertices);