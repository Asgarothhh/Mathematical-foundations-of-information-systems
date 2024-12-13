#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <ctime>
#include <set>
#include <algorithm>
using namespace std;

struct Edge
{
	int u;
	int v;
	int weight;
	bool operator<(Edge const& other) 
	{
		return weight < other.weight;
	}
};

struct graphParameters
{
	int Vmin, Vmax;
	int Emin, Emax;
	int Wmin, Wmax;
};

void readData(string path, graphParameters& graph)
{
	ifstream inputFile(path);

	if (!inputFile)
	{
		cerr << "Ошибка при открытии файла! \n";
		exit(1);
	}

	inputFile >> graph.Vmin >> graph.Vmax;
	inputFile >> graph.Emin >> graph.Emax;
	inputFile >> graph.Wmin >> graph.Wmax;

	inputFile.close();
}

void savedEdgeList(vector<Edge> edgeList, int vertices, string path)
{
	ofstream outFile(path);
	if (!outFile)
	{
		cerr << "Ошибка при открытии файла! \n";
		exit(1);
	}

	for (const auto& edge : edgeList)
	{
		outFile << edge.u << " " << edge.v << " " << edge.weight << "\n";
		outFile << edge.v << " " << edge.u << " " << edge.weight << "\n";
	}

	outFile.close();
}

void generateGraph(int& vertices)
{
	string inputfilePath = "input.txt", listFile = "list.txt";
	vector<Edge> edgeList;
	graphParameters graph;

	readData(inputfilePath, graph);

	srand(time(0));

	vertices = graph.Vmin + rand() % (graph.Vmax - graph.Vmin + 1);
	int edges = graph.Emin + rand() % (graph.Emax - graph.Emin + 1);

	if (edges > vertices * (vertices - 1) / 2)
	{
		edges = vertices * (vertices - 1) / 2;
	}

	cout << "Количество вершин: " << vertices << "\tКоличество рёбер: " << edges << endl;

	set<pair<int, int>> edgeSet;

	for (size_t i = 0; i < edges; i++)
	{
		int from = rand() % vertices;
		int to = rand() % vertices;
		if (from == to)
		{
			i--;
			continue;
		}

		if (from > to)
		{
			swap(from, to);
		}

		if (edgeSet.count({ from, to }))
		{
			i--;
			continue;
		}

		edgeSet.insert({ from, to });

		int weight = 0;
		weight = graph.Wmin + rand() % (graph.Wmax - graph.Wmin + 1);

		edgeList.push_back({ from, to, weight });
	}

	savedEdgeList(edgeList, vertices, listFile);
}

void readEdgeList(vector<Edge>& edgeList, int& vertices, string path) 
{
    ifstream inFile(path);
    if (!inFile) 
    {
        cerr << "Ошибка при открытии файла! \n";
        exit(1);
    }

    string line;
    vertices = 0; 

    while (getline(inFile, line)) {
        istringstream iss(line);
        Edge edge;

        if (!(iss >> edge.u >> edge.v >> edge.weight)) 
        {
            cerr << "Ошибка в формате данных в строке: " << line << "\n";
            continue; 
        }

        edgeList.push_back(edge);

        vertices = max(vertices, max(edge.u, edge.v) + 1); 
    }

    inFile.close();
}

void make_set(int v, vector<int>& parent, vector<int>& rank)
{
	parent[v] = v;
	rank[v] = 0;
}

int find(int v, vector<int>& parent)
{
	if (v == parent[v])
		return v;
	return parent[v] = find(parent[v], parent);
}

void union_sets(int a, int b, vector<int>& parent, vector<int>& rank)
{
	a = find(a, parent);
	b = find(b, parent);
	if (a != b) {
		if (rank[a] < rank[b])
			swap(a, b);
		parent[b] = a;
		if (rank[a] == rank[b])
			rank[a]++;
	}
}


int main()
{
    setlocale(LC_ALL, "Russian");

    vector<Edge> edgeList;
    int vertices;
    string edgelist = "list.txt";

    generateGraph(vertices);
    readEdgeList(edgeList, vertices, edgelist);

	cout << "\nСписок рёбер сгенерированного графа: \n";
    for(const auto & edge : edgeList) 
	{
		cout << edge.u << " " << edge.v << " " << edge.weight << endl;
	}

	int cost = 0;
	vector<Edge> result;
	vector<int> parent(vertices);
	vector<int> rank(vertices);
	for (int i = 0; i < vertices; i++)
		make_set(i, parent, rank);

	sort(edgeList.begin(), edgeList.end());

	for (Edge e : edgeList) 
	{
		if (find(e.u, parent) != find(e.v, parent)) 
		{
			cost += e.weight;
			result.push_back(e);
			union_sets(e.u, e.v, parent, rank);
		}
	}
	
	cout << "\nМинимальная стоимость остовного дерева: " << cost << endl;
	cout << "Минимальное остовное дерево (MST):" << endl;
	for (const auto& edge : result) 
	{
		cout << edge.u << " -- " << edge.v << " [вес: " << edge.weight << "]" << endl;
	}

	return 0;
}