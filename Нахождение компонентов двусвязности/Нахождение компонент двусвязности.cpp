#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <stack>
#include <algorithm>
#include <ctime>
#include <string>

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

// Переменные для поиска компонент двусвязности
vector<int> tin, low;
vector<bool> visited;
stack<Edge> st;
int timer;

void readData(const string& path, graphParameters& graph)
{
    ifstream inputFile(path);

    if (!inputFile)
    {
        cerr << "Ошибка при открытии файла! \n";
        exit(1);
    }

    if (!(inputFile >> graph.directed >> graph.weighted >> graph.self_loops))
    {
        cerr << "Ошибка: неверный формат данных графа.\n";
        exit(1);
    }

    if (!(inputFile >> graph.Vmin >> graph.Vmax >> graph.Emin >> graph.Emax))
    {
        cerr << "Ошибка: неверные параметры вершин или рёбер.\n";
        exit(1);
    }

    if (graph.weighted)
    {
        if (!(inputFile >> graph.Wmin >> graph.Wmax))
        {
            cerr << "Ошибка: неверные параметры весов рёбер.\n";
            exit(1);
        }
    }

    inputFile.close();
}

void savedAdjacencyList(const vector<Edge>& edgeList, int vertices, int directed, int weighted, const string& path)
{
    ofstream outFile(path);
    if (!outFile)
    {
        cerr << "Ошибка при открытии файла! \n";
        exit(1);
    }

    vector<vector<pair<int, int>>> adjList(vertices);

    for (const auto& edge : edgeList)
    {
        adjList[edge.from].push_back({ edge.to, edge.weight });

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
            if (weighted)
            {
                outFile << "(" << neighbor.first << ", " << neighbor.second << ") ";
            }
            else
            {
                outFile << neighbor.first << " ";
            }
        }
        outFile << "\n\n";
    }

    outFile.close();
}

void generateGraph(graphParameters& graph, vector<Edge>& edgeList, int& ver)
{
    srand(static_cast<unsigned int>(time(0)));

    int vertices = graph.Vmin + rand() % (graph.Vmax - graph.Vmin + 1);
    ver = vertices;
    int edges = graph.Emin + rand() % (graph.Emax - graph.Emin + 1);

    cout << "Количество вершин: " << vertices << "\tКоличество рёбер: " << edges << endl;

    set<pair<int, int>> edgeSet;

    for (int i = 0; i < edges; i++)
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
}


void readAdjacencyList(const string& path, vector<vector<int>>& adjList)
{
    ifstream inputFile(path);
    if (!inputFile) {
        cerr << "Ошибка при открытии файла для чтения списка смежности! \n";
        exit(1);
    }

    adjList.clear();
    string line;
    while (getline(inputFile, line)) {
        if (line.empty()) continue;

        size_t pos = line.find(":");
        if (pos == string::npos) continue;

        int vertex = stoi(line.substr(0, pos));
        adjList.resize(max((int)adjList.size(), vertex + 1));

        string neighbors = line.substr(pos + 1);
        size_t start = 0, end;
        while ((end = neighbors.find(" ", start)) != string::npos)
        {
            string neighborStr = neighbors.substr(start, end - start);
            if (!neighborStr.empty())
            {
                int neighbor = stoi(neighborStr);
                adjList[vertex].push_back(neighbor);
            }
            start = end + 1;
        }
    }

    inputFile.close();
}

// Алгоритм поиска компонент двусвязности
void dfs(int v, int parent, vector<vector<int>>& adjList)
{
    visited[v] = true;
    tin[v] = low[v] = timer++;
    int children = 0;

    for (int to : adjList[v])
    {
        if (to == parent) continue;

        if (visited[to])
        {
            low[v] = min(low[v], tin[to]);
            if (tin[to] < tin[v])
            {
                st.push({ v, to });
            }
        }
        else
        {
            st.push({ v, to });
            dfs(to, v, adjList);
            low[v] = min(low[v], low[to]);

            if (low[to] >= tin[v] && parent != -1)
            {
                cout << "Точка сочленения: " << v << endl;

                while (!st.empty())
                {
                    Edge e = st.top();
                    st.pop();
                    cout << "(" << e.from << ", " << e.to << ") ";
                    if (e.from == v && e.to == to) break;
                }
                cout << endl;
            }

            ++children;
        }
    }

    if (parent == -1 && children > 1)
    {
        cout << "Точка сочленения: " << v << endl;
        while (!st.empty())
        {
            Edge e = st.top();
            st.pop();
            cout << "(" << e.from << ", " << e.to << ") ";
        }
        cout << endl;
    }
}

void findBiconnectedComponents(int n, vector<vector<int>>& adjList)
{
    tin.assign(n, -1);
    low.assign(n, -1);
    visited.assign(n, false);
    timer = 0;

    for (int i = 0; i < n; ++i)
    {
        if (!visited[i])
        {
            dfs(i, -1, adjList);
        }
    }

    if (!st.empty()) {
        cout << "Оставшаяся двусвязная компонента: ";
        while (!st.empty())
        {
            Edge e = st.top();
            st.pop();
            cout << "(" << e.from << ", " << e.to << ") ";
        }
        cout << endl;
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    string inputfilePath = "input.txt", outputFilePath = "output.txt";
    graphParameters graph;
    int vertices;

    vector<Edge> edgeList;
    vector<vector<int>> adjList;

    readData(inputfilePath, graph);

    generateGraph(graph, edgeList, vertices);

    savedAdjacencyList(edgeList, vertices, graph.directed, graph.weighted, outputFilePath);

    // Читаем сгенерированный граф
    readAdjacencyList(outputFilePath, adjList);

    // Находим компоненты двусвязности
    cout << "\nКомпоненты двусвязности:\n";
    findBiconnectedComponents(vertices, adjList);

    return 0;
}