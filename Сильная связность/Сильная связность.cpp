#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <set>
#include <stack>
using namespace std;

struct Edge {
    int from;
    int to;
    int weight;
};

struct graphParameters {
    int directed, weighted, self_loops;
    int Vmin, Vmax, Emin, Emax;
    int Wmin = 0, Wmax = 0;
};

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

    for (int i = 0; i < vertices; ++i) 
    {
        outFile << i << ": ";
        for (const auto& neighbor : adjList[i])
        {
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

void generateGraph(graphParameters& graph, vector<Edge>& edgeList, int& ver) 
{
    string matrixFile = "matrix.txt", listFile = "list.txt";
    srand(time(0));
    int vertices = graph.Vmin + rand() % (graph.Vmax - graph.Vmin + 1);
    ver = vertices;
    int edges = graph.Emin + rand() % (graph.Emax - graph.Emin + 1);

    set<pair<int, int>> edgeSet;

    for (size_t i = 0; i < edges; i++) 
    {
        int from = rand() % vertices;
        int to = rand() % vertices;

        if (!graph.self_loops && from == to) 
        {
            i--;
            continue;
        }

        if (!graph.directed && from > to) 
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
        if (graph.weighted) 
        {
            weight = graph.Wmin + rand() % (graph.Wmax - graph.Wmin + 1);
        }
        edgeList.push_back({ from, to, weight });
    }

    savedAdjacencyMatrix(edgeList, vertices, graph.directed, graph.weighted, matrixFile);

    savedAdjacencyList(edgeList, vertices, graph.directed, graph.weighted, listFile);
}

void buildAdjacencyLists(int vertices, const vector<Edge>& edgeList, vector<vector<int>>& g, vector<vector<int>>& gr, int directed) 
{
    g.assign(vertices, vector<int>());
    gr.assign(vertices, vector<int>());

    for (const auto& edge : edgeList) 
    {
        g[edge.from].push_back(edge.to);
        gr[edge.to].push_back(edge.from);
        if (!directed) 
        {
            g[edge.to].push_back(edge.from);
            gr[edge.from].push_back(edge.to);
        }
    }
}

void dfs1(int v, const vector<vector<int>>& g, vector<char>& used, vector<int>& order) 
{
    used[v] = true;
    for (int to : g[v]) 
    {
        if (!used[to]) 
        {
            dfs1(to, g, used, order);
        }
    }
    order.push_back(v);
}

void dfs2(int v, const vector<vector<int>>& gr, vector<char>& used, vector<int>& component) 
{
    used[v] = true;
    component.push_back(v);
    for (int to : gr[v]) 
    {
        if (!used[to]) 
        {
            dfs2(to, gr, used, component);
        }
    }
}

void findStronglyConnectedComponents(int vertices, const vector<vector<int>>& g, const vector<vector<int>>& gr) 
{
    vector<char> used(vertices, false);
    vector<int> order, component;

    // Первый проход — прямой граф
    for (int i = 0; i < vertices; ++i)
    {
        if (!used[i]) 
        {
            dfs1(i, g, used, order);
        }
    }

    // Второй проход — транспонированный граф
    used.assign(vertices, false);
    cout << "\nСильно связные компоненты:\n";
    for (int i = vertices - 1; i >= 0; --i)
    {
        int v = order[i];
        if (!used[v])
        {
            dfs2(v, gr, used, component);
            cout << "{ ";
            for (int node : component) 
            {
                cout << node << " ";
            }
            cout << "}\n";
            component.clear();
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    string inputfilePath = "input.txt";
    graphParameters graph;
    int vertices;
    vector<Edge> edgeList;

    // Считывание параметров графа
    readData(inputfilePath, graph);

    // Генерация графа
    generateGraph(graph, edgeList, vertices);

    // Создание списков смежности для прямого и транспонированного графа
    vector<vector<int>> g, gr;
    buildAdjacencyLists(vertices, edgeList, g, gr, graph.directed);

    // Нахождение и вывод сильно связных компонент
    findStronglyConnectedComponents(vertices, g, gr);

    return 0;
}
