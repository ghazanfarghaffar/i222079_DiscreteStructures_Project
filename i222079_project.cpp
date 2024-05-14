#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

// Structure to represent an edge between two cities
struct Edge
{
    int destination;
    int capacity;
};

// Function to print the graph
void printGraph(const vector<vector<Edge>> &graph)
{
    cout << "Graph:" << endl;
    for (int i = 1; i < graph.size(); ++i)
    {
        cout << "City " << i << ": ";
        for (const Edge &edge : graph[i])
        {
            cout << "(" << edge.destination << ", " << edge.capacity << ") ";
        }
        cout << endl;
    }
}

// Function to perform BFS and find the minimum number of trips
int minTrips(const vector<vector<Edge>> &adjList, int start, int end, int tourists)
{
    int n = adjList.size();
    vector<int> maxFlow(n, 0);
    maxFlow[start] = INT_MAX;

    priority_queue<pair<int, int>> pq;
    pq.push({INT_MAX, start});

    while (!pq.empty())
    {
        int flow = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        for (const Edge &edge : adjList[u])
        {
            int v = edge.destination;
            int capacity = edge.capacity;
            int newFlow = min(flow, capacity);

            if (newFlow > maxFlow[v])
            {
                maxFlow[v] = newFlow;
                pq.push({newFlow, v});
            }
        }
    }

    int maxTouristsPerTrip = maxFlow[end];
    if (maxTouristsPerTrip == 0)
        return -1; // No path found

    return (tourists + maxTouristsPerTrip - 1) / maxTouristsPerTrip; // Ceiling division
}

int main()
{
    ifstream inputFile("data.txt");
    if (!inputFile)
    {
        cerr << "Failed to open input file!" << endl;
        return 1;
    }

    int N, R;
    inputFile >> N >> R;

    vector<vector<Edge>> graph(N + 1); // 1-based indexing for cities

    // Input road segments
    for (int i = 0; i < R; ++i)
    {
        int C1, C2, P;
        inputFile >> C1 >> C2 >> P;
        graph[C1].push_back({C2, P});
        graph[C2].push_back({C1, P}); // Since the graph is undirected
    }

    printGraph(graph); // Print the constructed graph

    int start, destination, tourists;
    inputFile >> start >> destination >> tourists;

    inputFile.close();

    int minTripsRequired = minTrips(graph, start, destination, tourists);
    cout << "Minimum number of trips required: " << minTripsRequired << endl;

    return 0;
}