#include <bits/stdc++.h>

#define pb push_back
#define F first
#define S second

using namespace std;

typedef unsigned long long int ulli;

vector<vector<pair<ulli, ulli>>> generateRandomGraph(int numOfVerteces, int numOfEdges, bool completeGraph) {
    vector<vector<ulli>> adjacencyMatrix(numOfVerteces, vector<ulli>(numOfVerteces, INT_MAX));
    if(numOfEdges > 0 || completeGraph) {
        for (int i = 0; i < numOfVerteces; ++i) {
            for (int j = i; j < numOfVerteces; ++j) {
                if (rand() % 2 || completeGraph) {
                    adjacencyMatrix[i][j] = rand() % 100;
                    numOfEdges--;
                } else {
                    adjacencyMatrix[i][j] = INT_MAX;
                }
                if (i == j)
                    adjacencyMatrix[i][j] = 0;
                if (numOfEdges <= 0 && !completeGraph)
                    break;
            }
            if (numOfEdges <= 0 && !completeGraph)
                break;
        }
    }
    vector<vector<pair<ulli, ulli>>> adjacencyList(numOfVerteces);
    for (int i = 0; i < numOfVerteces; ++i) {
        for (int j = i; j < numOfVerteces; ++j) {
            if(adjacencyMatrix[i][j] != INT_MAX)
                adjacencyList[i].pb({j, adjacencyMatrix[i][j]});
        }
    }

    return adjacencyList;
}
vector<ulli> minDistToEachVertex(ulli& op, vector<vector<pair<ulli, ulli>>>& g) {
    vector<ulli> minDist(g.size(), ULLONG_MAX); //array of minimum distances to each vertex
    minDist[0] = 0; op++;
    multiset<pair<ulli, ulli>> q;
    q.insert({minDist[0], 0}); //pair<distance, vertex>
    if(!q.empty()) op += ulli(log2l(q.size()));
    while(!q.empty()) {
        ulli vertex = q.begin()->second; op++;
        q.erase(q.begin()); if(!q.empty()) op += ulli(log2l(q.size()));
        for (size_t j = 0; j < g[vertex].size(); ++j, op += 2) {
            ulli to = g[vertex][j].first; op++;
            ulli distance = g[vertex][j].second; op++;
            op += 2;
            if (minDist[vertex] + distance < minDist[to]) {
                q.erase({minDist[to], to}); if(!q.empty()) op += ulli(log2l(q.size()));
                minDist[to] = minDist[vertex] + distance; op += 2;
                q.insert({minDist[to], to}); if(!q.empty()) op += ulli(log2l(q.size()));
            }
        }
    }
    return minDist;
}
void printAdjacencyList(vector<vector<pair<ulli, ulli>>>& g) {
    for (vector<pair<ulli, ulli>>& v : g) {
        for (pair<ulli, ulli>& nb : v)
            cout << nb.F << ' ';
        if(!v.empty())
            cout << '\n';
    }
}
void foutOpForRandomGraphs(int& numOfVerteces, int& numOfGenerations) {
    srand(time(nullptr));
    vector<vector<pair<ulli, ulli>>> g;
    ulli op;
    ulli maxOP = 0;
    ulli minOP = ULONG_LONG_MAX;
    ulli sumOP = 0;
    long double avgOP;
    for (int j = 0; j < numOfGenerations; ++j) {
        op = 0;
        g.clear();

        int numOfEdges = rand() % (((numOfVerteces * numOfVerteces - numOfVerteces) / 2) + 1);
        g = generateRandomGraph(numOfVerteces, numOfEdges, false);
        minDistToEachVertex(op, g);

        if(op < minOP)
            minOP = op;

        sumOP += op;
    }
    avgOP = sumOP/(double)numOfGenerations;
    op = 0;
    g.clear();
    g = generateRandomGraph(numOfVerteces, 0, true);
    minDistToEachVertex(op, g);
    maxOP = op;
    ofstream fout("../output.txt", ios::app);
    fout << numOfVerteces << ' ' << minOP << ' ' << avgOP << ' ' << maxOP << '\n';
    fout.close();
}
int main() {
    int maxNumOfVerteces = 20;
    int numOfGraphGenerations = 1000;
    cout << "Enter the maximum number of vertices in the graph:\n";
    cin >> maxNumOfVerteces;
    cout << "Enter the number of generations of random graphs for each number of vertices:\n";
    cin >> numOfGraphGenerations;
    ofstream fout("../output.txt");
    fout.close();
    for (int numOfVerteces = 2; numOfVerteces <= maxNumOfVerteces; ++numOfVerteces) {
        foutOpForRandomGraphs(numOfVerteces, numOfGraphGenerations);
    }
    return 0;
}
