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

void readData(string path, graphParameters& graph)
{
	ifstream inputFile(path);

	if (!inputFile)
	{
		cerr << "Ошибка при открытии файла! \n";
		exit(1);
	}

	inputFile >> graph.directed >> graph.weighted >> graph.self_loops;
	inputFile >> graph.Vmin >> graph.Vmax;
	inputFile >> graph.Emin >> graph.Emax;

	if (graph.weighted)
	{
		inputFile >> graph.Wmin >> graph.Wmax;
	}

	inputFile.close();
}

void savedAdjacencyList(vector<Edge> edgeList, int vertices, int directed, int weighted, string path)
{
	ofstream outFile(path);
	if (!outFile)
	{
		cerr << "Ошибка при открытии файла! \n";
		exit(1);
	}

	vector<vector<pair<int, int>>> adjList(vertices);

	for (size_t i = 0; i < edgeList.size(); i++)
	{
		const Edge& edge = edgeList[i]; // Получаем текущее ребро по индексу 

		adjList[edge.from].push_back({ edge.to, edge.weight });

		// Если граф неориентированный, добавляем обратное ребро
		if (!directed && edge.from != edge.to)
		{
			adjList[edge.to].push_back({ edge.from, edge.weight });
		}
	}

	for (int i = 0; i < vertices; ++i) {
		outFile << i << ": ";
		for (const auto& neighbor : adjList[i]) {
			if (weighted) {
				outFile << "(" << neighbor.first << ", " << neighbor.second << ") "; // вершина и вес
			}
			else {
				outFile << neighbor.first << " "; // только вершина
			}
		}
		outFile << "\n\n";
	}

	outFile.close();
}

void savedAdjacencyMatrix(vector<Edge>& edgeList, int vertices, int directed, int weighted, string path)
{
	ofstream outFile(path);
	if (!outFile)
	{
		cerr << "Ошибка при открытии файла! \n";
		exit(1);
	}

	vector<vector<int>> adjMatrix(vertices, vector<int>(vertices, 0));

	for (size_t i = 0; i < edgeList.size(); ++i) 
	{
		
		const Edge& edge = edgeList[i];

		// Записываем в матрицу смежности
		adjMatrix[edge.from][edge.to] = weighted ? edge.weight : 1;  // Если граф взвешенный, записываем вес, иначе 1

		// Если граф неориентированный, добавляем обратное ребро
		if (!directed && edge.from != edge.to)
		{
			adjMatrix[edge.to][edge.from] = weighted ? edge.weight : 1;
		}
	}

	for (size_t i = 0; i < vertices; i++)
	{
		for (size_t j = 0; j < vertices; j++)
		{
			outFile << adjMatrix[i][j] << " ";
		}
		outFile << "\n";
	}

	outFile.close();
}

void generateGraph(graphParameters& graph, vector<Edge>& edgeList, int& ver)
{
	srand(time(0));

	int vertices = graph.Vmin + rand() % (graph.Vmax - graph.Vmin + 1);
	ver = vertices;
	int edges = graph.Emin + rand() % (graph.Emax - graph.Emin + 1);

	cout << "Количество вершин: " << vertices << "\tКоличество рёбер: " << edges;

	set<pair<int, int>> edgeSet;

	for (size_t i = 0; i < edges; i++)
	{
		int from = rand() % vertices; // Генерирует случайный индекс вершины, который будет начальной вершиной (from) ребра
		int to = rand() % vertices; // Генерирует случайный индекс вершины, который будет конечной вершиной (to) ребра

		if (!graph.self_loops && from == to) // Если петли запрещены, то пересоздаём ребро
		{
			i--;
			continue;
		}

		if (!graph.directed && from > to) // Если граф неориентированный, сохраняем рёбра в одном направлении
		{
			swap(from, to);
		}

		if (edgeSet.count({ from, to })) // Проверка на дубликаты рёбер
		{
			i--;
			continue;
		}

		edgeSet.insert({ from, to });

		int weight = 0;
		if (graph.weighted)
		{
			weight = graph.Wmin + rand() % (graph.Wmax - graph.Wmin + 1);
		}

		edgeList.push_back({ from, to, weight });
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");
	
	string inputfilePath = "input.txt", outputFilePath = "output.txt";
	graphParameters graph;
	int vertices, answer;

	vector<Edge> edgeList; // список для хранения рёбер

	readData(inputfilePath, graph);

	generateGraph(graph, edgeList, vertices);

	cout << "\nВыберите способ представления сгенерированного графа: \n 0 - список смежности, 1 - матрица смежности |-> ";
	cin >> answer;
	if (answer)
		savedAdjacencyMatrix(edgeList, vertices, graph.directed, graph.weighted, outputFilePath);
	else
		savedAdjacencyList(edgeList, vertices, graph.directed, graph.weighted, outputFilePath);

	return 0;
}