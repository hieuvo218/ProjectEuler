#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <unordered_map>
#include <stack>
#include <utility>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <algorithm>

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
    if (*(component + node) != -1) {
        return;
    }
    // assign node as belonging to the component whose root is root
    *(component + node) = root;
    for (int i = 0; i < readjacent_airports[node].size(); i++) {
        assign(readjacent_airports[node][i], root, readjacent_airports, component);
    }
}

struct information{
    int id;
    int index;
    int lowLink;
    int onStack;

    bool operator==(const information& other) const {
        return id == other.id;
    }
};

// Custom hash function
struct InformationHash {
    std::size_t operator()(const information* info) const {
        std::size_t h1 = std::hash<int>{}(info->id);
        return h1; // Combine the hashes
    }
};

int solve_with_tarjan(string start_airport, const vector<string> &airports, const vector<pair<string, string>> &routes) {
    int n = airports.size(); 
    unordered_map<string, information> mp;   
    unordered_map<information*, vector<information*>, InformationHash> adjacent_list(n);
    // Global variables
    int index = 0;
    stack<information> s;
    int component[n];
    fill_n(component, n, -1);

    // construct a map from name of airport to its id
    for (int i = 0; i < airports.size(); i++) {
        mp[airports[i]] = information{i, -1, 0, 0};
    }

    // Construct adjacent list in format of (airport1, [airport2, airport3, ...])
    for (int i = 0; i < routes.size(); i++) {
        adjacent_list[&mp[routes[i].first]].push_back(&mp[routes[i].second]);
    }

    // Use Tarjan's algorithm to find strongly connected component
    // Save the following information (index, lowlink, instack)

    function<void(information*)> strong_connect = [&](information *v) -> void {
        v->index = index;
        v->lowLink = index;
        v->onStack = true;
        index++;
        s.push(*v);

        // Consider successors of v
        for (information *w : adjacent_list[v]) {
            if (w->index == -1) {
                // Successor w has not been visited; recurse on it
                strong_connect(w);
                v->lowLink = min(w->lowLink, v->lowLink);
            }
            else if (w->onStack) 
                // Successor w is in stack S and hence in the current SCC
                // If w is not on stack, then (v, w) is an edge pointing to an SCC already
                // found and must be ignored
                // See below regarding the next line
                v->lowLink = min(w->index, v->lowLink);
        }

        // If v is a root node, pop the stack and generate an SCC
        if (v->lowLink == v->index) {
            while(s.top().id != v->id) {
                information w = s.top();
                s.pop();
                w.onStack = 0;
                w.lowLink = n;
                // Add node to SCC
                component[w.id] = v->id;
            }
            information w = s.top();
            s.pop();
            w.onStack = 0;
            w.lowLink = n;
            // Add node to SCC
            component[w.id] = v->id;
        }
    };

    for (int i = 0; i < n; i++)
        if (mp[airports[i]].index == -1)
            strong_connect(&mp[airports[i]]);
    
    // count the indegree of each component
    int indegree[airports.size()] = {0};
    for (int i = 0; i < airports.size(); i++) 
        for (auto adjacent_airport : adjacent_list[&mp[airports[i]]]) 
            if (component[i] != component[adjacent_airport->id]) 
                indegree[component[adjacent_airport->id]]++;

    // count the number of SCCs that has indegree = 0
    int res = 0;
    for (int i = 0; i < airports.size(); i++)
        if (component[i] == i && indegree[i] == 0 && component[mp[start_airport].id] != i)
            res++;
    return res;
}

int solve_with_kosaraju(string start_airport, const vector<string> &airports, const vector<pair<string, string>> &routes)
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
    int component[n];
    fill_n(component, n, -1);

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
    int res = solve_with_kosaraju(start_airport, airports, routes);
    cout << res << endl;

    return 0;
}