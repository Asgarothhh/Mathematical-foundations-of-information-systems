#include "initGraph.h"

void readData(string path, graphParameters& graph)
{
	ifstream inputFile(path);

	if (!inputFile)
	{
		cerr << "������ ��� �������� �����! \n";
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
		cerr << "������ ��� �������� �����! \n";
		exit(1);
	}

	vector<vector<pair<int, int>>> adjList(vertices);

	for (size_t i = 0; i < edgeList.size(); i++)
	{
		const Edge& edge = edgeList[i]; // �������� ������� ����� �� ������� 

		adjList[edge.from].push_back({ edge.to, edge.weight });

		// ���� ���� �����������������, ��������� �������� �����
		if (!directed && edge.from != edge.to)
		{
			adjList[edge.to].push_back({ edge.from, edge.weight });
		}
	}

	for (int i = 0; i < vertices; ++i) {
		outFile << i << ": ";
		for (const auto& neighbor : adjList[i]) {
			if (weighted) {
				outFile << "(" << neighbor.first << ", " << neighbor.second << ") "; // ������� � ���
			}
			else {
				outFile << neighbor.first << " "; // ������ �������
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
		cerr << "������ ��� �������� �����! \n";
		exit(1);
	}

	vector<vector<int>> adjMatrix(vertices, vector<int>(vertices, 0));

	for (size_t i = 0; i < edgeList.size(); ++i)
	{

		const Edge& edge = edgeList[i];

		// ���������� � ������� ���������
		adjMatrix[edge.from][edge.to] = weighted ? edge.weight : 1;  // ���� ���� ����������, ���������� ���, ����� 1

		// ���� ���� �����������������, ��������� �������� �����
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

void generateGraph(int& vertices)
{
	string inputfilePath = "input.txt", matrixFile = "matrix.txt", listFile = "list.txt";
	vector<Edge> edgeList; // ������ ��� �������� ����
	graphParameters graph;

	readData(inputfilePath, graph);

	srand(time(0));

	vertices = graph.Vmin + rand() % (graph.Vmax - graph.Vmin + 1);
	int edges = graph.Emin + rand() % (graph.Emax - graph.Emin + 1);

	cout << "���������� ������: " << vertices << "\t���������� ����: " << edges;

	set<pair<int, int>> edgeSet;

	for (size_t i = 0; i < edges; i++)
	{
		int from = rand() % vertices; // ���������� ��������� ������ �������, ������� ����� ��������� �������� (from) �����
		int to = rand() % vertices; // ���������� ��������� ������ �������, ������� ����� �������� �������� (to) �����

		if (!graph.self_loops && from == to) // ���� ����� ���������, �� ���������� �����
		{
			i--;
			continue;
		}

		if (!graph.directed && from > to) // ���� ���� �����������������, ��������� ���� � ����� �����������
		{
			swap(from, to);
		}

		if (edgeSet.count({ from, to })) // �������� �� ��������� ����
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

	savedAdjacencyMatrix(edgeList, vertices, graph.directed, graph.weighted, matrixFile);

	savedAdjacencyList(edgeList, vertices, graph.directed, graph.weighted, listFile);
}