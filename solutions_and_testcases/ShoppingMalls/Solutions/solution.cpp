#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

const int MAXPLACES = 1000;
const double INF = 848238233;

struct Place {
    int x, y, floor;
    Place(int x, int y, int floor) : x(x), y(y), floor(floor) {};
    Place() : x(0), y(0), floor(0) {};
};
vector<Place> places(MAXPLACES);

// Structures for Floyd-Warshall
vector<vector<double> > dist(MAXPLACES, vector<double>(MAXPLACES, INF));
vector<vector<int> > parent(MAXPLACES, vector<int>(MAXPLACES, -1));

// Euclidean distance between 2 places
double euc(const struct Place& a, const struct Place& b) {
    int difx = a.x - b.x;
    int dify = a.y - b.y;
    int diff = 5*(a.floor - b.floor);
    return sqrt(difx*difx + dify*dify + diff*diff);
}

int main(int argc, char *argv[]) {
    int N, M, a, b;
    string name, type;

    cin >> N >> M;
    // Read positions
    for (int i = 0; i < N; ++i) {
        cin >> places[i].floor >> places[i].x >> places[i].y;
        dist[i][i] = 0;
    }

    // Read connections, keeps the minimum for each pair of nodes
    for (int i = 0; i < M; ++i) {
        cin >> a >> b >> type;
        double d = euc(places[a], places[b]);
        if (type[1] == 's') { // escalator
            dist[a][b] = min(dist[a][b], 1.);
            dist[b][a] = min(dist[b][a], d*3);
        } else if (type[0] == 'l') { // lift
            dist[a][b] = min(dist[a][b], 1.);
            dist[b][a] = min(dist[b][a], 1.);
        } else { // walking, stairs
            dist[a][b] = min(dist[a][b], d);
            dist[b][a] = min(dist[b][a], d);
        }
    }

    // Floyd-Warshall
    // Prepare the path recovery
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (i == j || dist[i][j] == INF)
                parent[i][j] = -1;
            else
                parent[i][j] = i;

    // Compute shortest path
    for (int k = 0; k < N; ++k)
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j) {
                double newD = dist[i][k] + dist[k][j];
                if (newD < dist[i][j]) {
                    dist[i][j] = newD;
                    parent[i][j] = parent[k][j];
                }
            }

    // Debug
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j)
            if (dist[i][j] != -1)
                cerr << " " << parent[i][j];
            else
                cerr << " X";
        cerr << endl;
    }

    // Queries
    vector<int> result(MAXPLACES);
    int Q;
    cin >> Q;
    for (int q = 0; q < Q; ++q) {
        cin >> a >> b;
        cerr << a << " " << b << " dist: "
             << setprecision(8) << dist[a][b] << endl;

        // Recover path from a to b
        int i = 0;
        result[i++] = b;
        while ((b = parent[a][b]) != -1)
            result[i++] = b;
        for (int j = i-1; j >= 0; --j) {
            if (j < i-1)
                cout << " ";
            cout << result[j];
        }
        cout << endl;
    }
    return EXIT_SUCCESS;
}
