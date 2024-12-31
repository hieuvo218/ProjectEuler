#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <stack>
#include <utility>

using namespace std;

// visit function to visit each node in the graph by using dfs traversal and push each node to stack
// according to its finish time
void visit(int node, int visited[], const vector<vector<int>> &adjacent_airports, stack<int> &s) {
    if (visited[node] == 1) {
        return;
    }
    visited[node] = 1;
    for (int i = 0; i < adjacent_airports[node].size(); i++) 
        visit(adjacent_airports[node][i], visited, adjacent_airports, s);
    s.push(node);
}

// assign function to assign each airport to a component by using dfs traversal, if a node has not 
// been assigned assign it to the component whose root is the node
void assign(int node, int root, const vector<vector<int>> &readjacent_airports, int *component){
    // if node is already assigned to a component
    if (component[node] != 0) {
        return;
    }
    // assign node as belonging to the component whose root is root
    component[node] = root;
    for (int i = 0; i < readjacent_airports[node].size(); i++) {
        assign(readjacent_airports[node][i], root, readjacent_airports, component);
    }
}

int solve(string start_airport, const vector<string> &airports, const vector<pair<string, string>> &routes)
{
    int n = airports.size();    
    unordered_map<string, int> mp;
    vector<vector<int>> adjacent_airports(n);
    vector<vector<int>> readjacent_airports(n);

    // construct a map from name of airport to its id
    for (int i = 0; i < airports.size(); i++) {
        mp[airports[i]] = i;
    }

    // Construct adjacent list
    for (int i = 0; i < routes.size(); i++) {
        adjacent_airports[mp[routes[i].first]].push_back(mp[routes[i].second]);
        readjacent_airports[mp[routes[i].second]].push_back(mp[routes[i].first]);
    }

    // Use Kosaraju's algorithm to find strongly connected component
    stack<int> s;

    // forward dfs
    int visited[airports.size()] = {0};
    for (int i = 0; i < airports.size(); i++) {
        visit(i, visited, adjacent_airports, s);
    }

    // backward dfs
    int component[airports.size()] = {0};
    while(!s.empty()) {
        int node = s.top();
        s.pop();
        assign(node, node, readjacent_airports, component);
    }
    
    // count the number of components
    int count = 0;
    for (int i = 0; i < airports.size(); i++) {
        if (component[i] == 0) {
            count++;
        }
    }
    
    // count the indegree of each component
    int indegree[airports.size()] = {0};
    for (int i = 0; i < airports.size(); i++) 
        for (int j = 0; j < adjacent_airports[i].size(); j++) 
            if (component[i] != component[adjacent_airports[i][j]]) 
                indegree[component[adjacent_airports[i][j]]]++;

    // count the number of SCCs that has indegree = 0
    int res = 0;
    for (int i = 0; i < airports.size(); i++)
        if (component[i] == i && indegree[i] == 0 && component[mp[start_airport]] != i)
            res++;
    return res;
}

int main() {
    string start_airport = "LGA";
    vector<string> airports = {"BGI", "CDG", "DEL", "DOH", "DSM", "EWR", "EYW", "HND", "ICN",
                                "JFK", "LGA", "LHR", "ORD", "SAN", "SFO", "SIN", "TLV", "BUD"};
    vector<pair<string, string>> routes = {{"DSM", "ORD"},
                                     {"ORD", "BGI"},
                                     {"BGI", "LGA"},
                                     {"SIN", "CDG"},
                                     {"CDG", "SIN"},
                                     {"CDG", "BUD"},
                                     {"DEL", "DOH"},
                                     {"DEL", "CDG"},
                                     {"TLV", "DEL"},
                                     {"EWR", "HND"},
                                     {"HND", "ICN"},
                                     {"HND", "JFK"},
                                     {"ICN", "JFK"},
                                     {"JFK", "LGA"},
                                     {"EYW", "LHR"},
                                     {"LHR", "SFO"},
                                     {"SFO", "SAN"},
                                     {"SFO", "DSM"},
                                     {"SAN", "EYW"}};
    int res = solve(start_airport, airports, routes);
    cout << res << endl;

    return 0;
}