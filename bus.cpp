#include <iostream>
#include <unordered_map>
#include <vector>
#include <climits>
using namespace std;
class BusSystem {
public:
    // Vertex represents a bus stop
    struct Vertex {
        unordered_map<string, int> nbrs; // bus stops connected to this stop
    };

    // Methods for the BusSystem class
    BusSystem();
    int numVertices();
    bool containsVertex(const string& stopName);
    void addVertex(const string& stopName);
    void removeVertex(const string& stopName);
    int numEdges();
    bool containsEdge(const string& stopName1, const string& stopName2);
    void addEdge(const string& stopName1, const string& stopName2, int distance);
    void removeEdge(const string& stopName1, const string& stopName2);
    void displayMap();
    void displayStops();

    // Path finding methods
    bool hasPath(const string& stopName1, const string& stopName2);
    int dijkstra(const string& src, const string& des);

    // User interaction methods
    void printCodeList();
    void menu();

private:
    unordered_map<string, Vertex> stops;

    // Helper function for Dijkstra's algorithm
    string getMinDistanceStop(const unordered_map<string, int>& distance, const unordered_map<string, bool>& processed);
};

// Constructor
BusSystem::BusSystem() {}

// Function to get the number of bus stops
int BusSystem::numVertices() {
    return stops.size();
}

// Function to check if a bus stop exists
bool BusSystem::containsVertex(const string& stopName) {
    return stops.count(stopName) > 0;
}

// Function to add a bus stop
void BusSystem::addVertex(const string& stopName) {
    Vertex vtx;
    stops[stopName] = vtx;
}

// Function to remove a bus stop
void BusSystem::removeVertex(const string& stopName) {
    stops.erase(stopName);

    // Remove edges connected to this stop
    for (auto& entry : stops) {
        entry.second.nbrs.erase(stopName);
    }
}

// Function to get the number of edges (connections between stops)
int BusSystem::numEdges() {
    int count = 0;
    for (const auto& entry : stops) {
        count += entry.second.nbrs.size();
    }
    return count / 2; // Each edge is counted twice (undirected graph)
}

// Function to check if an edge exists between two bus stops
bool BusSystem::containsEdge(const string& stopName1, const string& stopName2) {
    // auto it = stops.find(stopName1);
    unordered_map<string, Vertex>::iterator it = stops.find(stopName1);
    if (it != stops.end()) {
        return it->second.nbrs.find(stopName2) != it->second.nbrs.end();
    }
    return false;
}

// Function to add an edge between two bus stops
void BusSystem::addEdge(const string& stopName1, const string& stopName2, int distance) {
    stops[stopName1].nbrs[stopName2] = distance;
    stops[stopName2].nbrs[stopName1] = distance;
}

// Function to remove an edge between two bus stops
void BusSystem::removeEdge(const string& stopName1, const string& stopName2) {
    stops[stopName1].nbrs.erase(stopName2);
    stops[stopName2].nbrs.erase(stopName1);
}

// Function to display the bus stop map
void BusSystem::displayMap() {
    cout << "\t Bus Stop Map \n";
    cout << "\t -------------- \n";
    for (const auto& entry : stops) {
        string stopName = entry.first;
        cout << stopName << " => \n";
        // const auto& nbrs = entry.second.nbrs;
        const unordered_map<string, int>& nbrs = entry.second.nbrs;
        for (const auto& nbr : nbrs) {
            cout << "\t" << nbr.first << "\t" << nbr.second << " KM\n";
        }
    }
    cout << "\t -------------- \n";
}

// Function to display the list of bus stops
void BusSystem::displayStops() {
    cout << "\n ************************************************************ \n";
    cout << "\n List of Bus Stops: \n";
    int i = 1;
    for (const auto& entry : stops) {
        cout << i << ". " << entry.first << "\n";
        i++;
    }
    cout << "\n ************************************************************ \n";
}

// Function to check if a path exists between two bus stops
bool BusSystem::hasPath(const string& stopName1, const string& stopName2) {
    unordered_map<string, bool> processed;
    return dijkstra(stopName1, stopName2) != numeric_limits<int>::max();
}

// Function to perform Dijkstra's algorithm to find the minimum distance between two bus stops
int BusSystem::dijkstra(const string& src, const string& des) {
    unordered_map<string, int> distance;
    unordered_map<string, bool> processed;

    for (const auto& entry : stops) {
        distance[entry.first] = numeric_limits<int>::max();
    }

    distance[src] = 0;

    for (int i = 0; i < numVertices() - 1; ++i) {
        string currentStop = getMinDistanceStop(distance, processed);
        processed[currentStop] = true;

        for (const auto& nbr : stops[currentStop].nbrs) {
            if (!processed[nbr.first] && distance[currentStop] != numeric_limits<int>::max() &&
                distance[currentStop] + nbr.second < distance[nbr.first]) {
                distance[nbr.first] = distance[currentStop] + nbr.second;
            }
        }
    }

    return distance[des];
}
// int BusSystem::dijkstra(const std::string& src, const std::string& des, std::unordered_map<std::string, bool>& processed) {
//     std::unordered_map<std::string, int> distance;

//     for (const auto& entry : stops) {
//         distance[entry.first] = std::numeric_limits<int>::max();
//     }

//     distance[src] = 0;

//     for (int i = 0; i < numVertices() - 1; ++i) {
//         std::string currentStop = getMinDistanceStop(distance, processed);
//         processed[currentStop] = true;

//         for (const auto& nbr : stops[currentStop].nbrs) {
//             if (!processed[nbr.first] && distance[currentStop] != std::numeric_limits<int>::max() &&
//                 distance[currentStop] + nbr.second < distance[nbr.first]) {
//                 distance[nbr.first] = distance[currentStop] + nbr.second;
//             }
//         }
//     }

//     return distance[des];
// }

// Helper function for Dijkstra's algorithm to get the bus stop with the minimum distance
string BusSystem::getMinDistanceStop(const unordered_map<string, int>& distance,const unordered_map<string, bool>& processed) {
    int minDistance = numeric_limits<int>::max();
    string minStop;

    for (const auto& entry : stops) {
        string stopName = entry.first;
        if (!processed.at(stopName) && distance.at(stopName) <= minDistance) {
            minDistance = distance.at(stopName);
            minStop = stopName;
        }
    }

    return minStop;
}

// Function to display the list of available options for user interaction
void BusSystem::printCodeList() {
    cout << "\n ********************** Bus System App ************************* \n";
    cout << "\n List of Available Options: \n";
    cout << " 1. Display Bus Stops\n";
    cout << " 2. Display Bus Stop Map\n";
    cout << " 3. Add Bus Stop\n";
    cout << " 4. Remove Bus Stop\n";
    cout << " 5. Add Bus Route\n";
    cout << " 6. Remove Bus Route\n";
    cout << " 7. Check if Path Exists between Bus Stops\n";
    cout << " 8. Find Minimum Distance between Bus Stops\n";
    cout << " 9. Exit\n";
    cout << "\n **************************************************************** \n";
}

// Function to handle user interaction with the bus system
void BusSystem::menu() {
    int choice;
    string stopName1, stopName2;
    int distance;
    int minDistance;

    do {
        printCodeList();
        cout << " Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayStops();
                break;

            case 2:
                displayMap();
                break;

            case 3:
                cout << " Enter the name of the new Bus Stop: ";
                cin >> stopName1;
                addVertex(stopName1);
                cout << " Bus Stop '" << stopName1 << "' added successfully.\n";
                break;

            case 4:
                cout << " Enter the name of the Bus Stop to remove: ";
                cin >> stopName1;
                if (containsVertex(stopName1)) {
                    removeVertex(stopName1);
                    cout << " Bus Stop '" << stopName1 << "' removed successfully.\n";
                } else {
                    cout << " Bus Stop '" << stopName1 << "' not found.\n";
                }
                break;

            case 5:
                cout << " Enter the names of the Bus Stops to connect: ";
                cin >> stopName1 >> stopName2;
                cout << " Enter the distance between " << stopName1 << " and " << stopName2 << " (in KM): ";
                cin >> distance;
                if (containsVertex(stopName1) && containsVertex(stopName2)) {
                    addEdge(stopName1, stopName2, distance);
                    cout << " Bus Route added successfully.\n";
                }
                else {
                    cout << " One or both Bus Stops not found.\n";
                }
                break;

            case 6:
                cout << " Enter the names of the Bus Stops to disconnect: ";
                cin >> stopName1 >> stopName2;
                if (containsEdge(stopName1, stopName2)) {
                    removeEdge(stopName1, stopName2);
                    cout << " Bus Route removed successfully.\n";
                } else {
                    cout << " Bus Route not found.\n";
                }
                break;

            case 7:
                cout << " Enter the names of the Bus Stops to check for a path: ";
                cin >> stopName1 >> stopName2;
                if (hasPath(stopName1, stopName2)) {
                    cout << " Path exists between " << stopName1 << " and " << stopName2 << ".\n";
                } else {
                    cout << " No path found between " << stopName1 << " and " << stopName2 << ".\n";
                }
                break;

            case 8:
                cout << " Enter the names of the Bus Stops to find the minimum distance: ";
                cin >> stopName1 >> stopName2;
                minDistance = dijkstra(stopName1, stopName2);
                if (minDistance != numeric_limits<int>::max()) {
                    cout << " Minimum distance between " << stopName1 << " and " << stopName2 << " is " << minDistance << " KM.\n";
                } else {
                    cout << " No path found between " << stopName1 << " and " << stopName2 << ".\n";
                }
                break;

            case 9:
                cout << " Exiting Bus System App. Thank you!\n";
                break;

            default:
                cout << " Invalid choice. Please try again.\n";
                break;
        }

    } while (choice != 10);
}

int main() {
    BusSystem busSystem;
    busSystem.menu();
    return 0;
}
