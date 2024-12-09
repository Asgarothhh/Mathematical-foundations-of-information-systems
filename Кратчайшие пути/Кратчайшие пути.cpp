#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <limits> 
#include "initGraph.h"
using namespace std;

void readAdjacencyList(vector<vector<pair<int, int>>>& adjList, int vertices, string fileName) {
    ifstream inFile(fileName);
    if (!inFile)
    {
        cerr << "\nОшибка при открытии файла: " << fileName << "\n";
        exit(1);
    }

    adjList.resize(vertices); // Инициализируем вектор для хранения смежностей

    string line;
    int vertex;

    while (getline(inFile, line))
    {
        if (line.empty()) continue; // Пропуск пустых строк

        stringstream ss(line);
        ss >> vertex; // Читаем номер вершины
        char colon;
        ss >> colon; // Читаем символ ':'

        if (ss.fail() || colon != ':') {
            cerr << "Ошибка: некорректный формат строки: " << line << "\n";
            exit(1);
        }

        // Читаем пары вида (сосед, вес)
        char openBracket, closeBracket, comma;
        int neighbor, weight;
        while (ss >> openBracket >> neighbor >> comma >> weight >> closeBracket) 
        {
            if (openBracket != '(' || comma != ',' || closeBracket != ')') {
                cerr << "Ошибка: некорректный формат пары в строке: " << line << "\n";
                exit(1);
            }
            adjList[vertex].emplace_back(neighbor, weight); // Добавляем (сосед, вес) в список
        }

        if (!ss.eof() && ss.fail()) {
            cerr << "Ошибка: некорректный формат строки: " << line << "\n";
            exit(1);
        }
    }

    inFile.close();
}


void readAdjacencyMatrix(vector<vector<int>>& adjMatrix, int vertices, string fileName)
{
    ifstream inFile(fileName);
    if (!inFile) 
    {
        cerr << "\nОшибка при открытии файла: " << fileName << "\n";
        exit(1);
    }

    adjMatrix.resize(vertices, vector<int>(vertices, 0)); // Инициализируем матрицу

    for (int i = 0; i < vertices; i++) 
    {
        for (int j = 0; j < vertices; j++)
        {
            inFile >> adjMatrix[i][j];
        }
    }
    inFile.close();
}

void dijkstra(const vector<vector<pair<int, int>>>& adjList, vector<int>& distance, int vertices, int startVer)
{
    priority_queue<pair<int, int>> q;
    vector<bool> processed(vertices, false);

    distance.assign(vertices, numeric_limits<int>::max());
    distance[startVer] = 0;

    q.push({ 0, startVer });
    while (!q.empty()) 
    {
        int a = q.top().second;
        q.pop();

        if (processed[a]) continue;
        processed[a] = true;

        for (auto u : adjList[a]) 
        {
            int b = u.first, w = u.second;
            if (distance[a] + w < distance[b]) 
            {
                distance[b] = distance[a] + w;
                q.push({ -distance[b], b });
            }
        }
    }
}

void floydWarshall(const vector<vector<int>> adjMatrix, vector<vector<int>>& distance, int vertices)
{
    const int INF = numeric_limits<int>::max();
    distance.assign(vertices, vector<int>(vertices, INF));

    for (int i = 0; i < adjMatrix.size(); i++) 
    {
        for (int j = 0; j < adjMatrix.size(); j++) 
        {
            if (i == j) distance[i][j] = 0;
            else if (adjMatrix[i][j]) distance[i][j] = adjMatrix[i][j];
            else distance[i][j] = INF;
        }
    }

    for (int k = 0; k < adjMatrix.size(); k++) 
    {
        for (int i = 0; i < adjMatrix.size(); i++)
        {
            for (int j = 0; j < adjMatrix.size(); j++)
            {
                distance[i][j] = min(distance[i][j], distance[i][k] + distance[k][j]);
            }
        }
    }
}

int main() 
{
    setlocale(LC_ALL, "Russian");

    string list = "list.txt", matrix = "matrix.txt";
    vector<vector<pair<int, int>>> adjList;
    vector<vector<int>> adjMatrix;
    int startVer, toVer, vertices;

    generateGraph(vertices);

    readAdjacencyList(adjList, vertices, list);
    readAdjacencyMatrix(adjMatrix, vertices, matrix);

    vector<int> distance(vertices);
    vector<vector<int>> distance1(vertices, vector<int>(vertices, 0));

    cout << "\nВведите стартовую вершину для поиска кратчайших путей в алгоритме Дейкстры: ";
    cin >> startVer;

    if (startVer < 0 || startVer >= vertices) 
    {
        cerr << "Ошибка: неверная стартовая вершина.\n";
        return 1;
    }

    dijkstra(adjList, distance, vertices, startVer);

    cout << "\nРасстояния от вершины " << startVer << " до:\n";
    for (int i = 0; i < vertices; i++) {
        if (distance[i] == numeric_limits<int>::max()) 
        {
            cout << "Вершина " << i << ": недостижима\n";
        }
        else 
        {
            cout << "Вершины " << i << ": " << distance[i] << "\n";
        }
    }

    cout << "\nРасстояние до какой вершины вы хотите узнать? ";
    cin >> toVer;

    if (toVer < 0 || toVer >= vertices) 
    {
        cerr << "Ошибка: неверная конечная вершина.\n";
        return 1;
    }

    if (distance[toVer] == numeric_limits<int>::max()) 
    {
        cout << "Вершина " << toVer << " недостижима из вершины " << startVer << ".\n";
    }
    else 
    {
        cout << "Расстояние от вершины " << startVer << " до вершины " << toVer << " равно " << distance[toVer] << ".\n";
    }

    floydWarshall(adjMatrix, distance1, vertices);
    cout << "\nРезультат работы алгоритма Флойда–Уоршелла: \n";
    for (int i = 0; i < adjMatrix.size(); i++) 
    {
        for (int j = 0; j < adjMatrix.size(); j++) 
        {
            if (distance1[i][j] == numeric_limits<int>::max())
                cout << "INF ";
            else
                cout << distance1[i][j] << " ";
        }
        cout << "\n";
    }

    return 0;
}
