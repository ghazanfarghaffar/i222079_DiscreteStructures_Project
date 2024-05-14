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

    int N, R, C1, C2, P, start, destination, tourists;

    ofstream outputFile("data.txt");
    if (!outputFile)
    {
        cerr << "Failed to open output file!" << endl;
        return 1;
    }

    cout << "Enter the number of cities(0 FOR SAVED GRAPH): ";
    cin >> N;
    while (N < 0)
    {

        cerr << "Invalid input! Please enter a positive integer for the number of cities." << endl;
        cin >> N;
    }
    if (N == 0)
    {
        goto label;
    }
    outputFile << N << " ";

    while (true)
    {
        cout << "Enter the number of road segments: ";
        if (!(cin >> R) || R < 0)
        {
            cerr << "Invalid input! Please enter a non-negative integer for the number of road segments." << endl;
        }
        else
        {
            break;
        }
    }
    outputFile << R << " ";
    outputFile << endl;

    for (int i = 0; i < R; ++i)
    {
        while (true)
        {
            int C1, C2, P;
            cout << "Enter the city pair and capacity of road segment " << i + 1 << ": ";
            if (!(cin >> C1 >> C2 >> P) || C1 < 1 || C1 > N || C2 < 1 || C2 > N || P <= 0)
            {
                cerr << "Invalid input! Please enter valid city identifiers and positive capacities." << endl;
            }
            else
            {
                outputFile << C1 << " " << C2 << " " << P << " ";
                outputFile << endl;
                break;
            }
        }
    }

    while (true)
    {
        cout << "Enter the starting city: ";
        if (!(cin >> start) || start < 1 || start > N)
        {
            cerr << "Invalid input! Please enter a valid city identifier for the starting city." << endl;
        }
        else
        {
            break;
        }
    }
    outputFile << start << " ";

    while (true)
    {
        cout << "Enter the destination city: ";
        if (!(cin >> destination) || destination < 1 || destination > N)
        {
            cerr << "Invalid input! Please enter a valid city identifier for the destination city." << endl;
        }
        else
        {
            break;
        }
    }
    outputFile << destination << " ";

    while (true)
    {
        cout << "Enter the number of tourists: ";
        if (!(cin >> tourists) || tourists < 0)
        {
            cerr << "Invalid input! Please enter a non-negative integer for the number of tourists." << endl;
        }
        else
        {
            break;
        }
    }
    outputFile << tourists << " ";

    outputFile.close();

label:
    ifstream inputFile("data.txt");
    if (!inputFile)
    {
        cerr << "Failed to open input file!" << endl;
        return 1;
    }

    inputFile >> N >> R;

    vector<vector<Edge>> graph(N + 1); // 1-based indexing for cities

    // Input road segments
    for (int i = 0; i < R; ++i)
    {
        inputFile >> C1 >> C2 >> P;
        graph[C1].push_back({C2, P});
        graph[C2].push_back({C1, P}); // Since the graph is undirected
    }

    printGraph(graph); // Print the constructed graph

    inputFile >> start >> destination >> tourists;

    inputFile.close();

    int minTripsRequired = minTrips(graph, start, destination, tourists);
    cout << "Minimum number of trips required: " << minTripsRequired << endl;

    return 0;
}