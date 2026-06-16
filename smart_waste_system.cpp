#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "sample_data.h"

double getPriorityScore(WasteBin b) { return b.wasteLevel * (b.capacity / 100.0); }

int readChoice() {
  int val = 0;
  if (!(cin >> val)) { if (cin.eof()) exit(0); cin.clear(); }
  cin.ignore(10000, '\n');
  return val;
}
void pressEnterToContinue() {
  cout << "\n  Press Enter to continue...";
  if (cin.eof()) exit(0);
  cin.get();
}

string pad(string s, size_t len, bool rightAlign = false) { if (s.length() >= len) return s.substr(0, len); return rightAlign ? string(len - s.length(), ' ') + s : s + string(len - s.length(), ' '); }
void drawLine(const vector<int>& widths) {
  cout << "  +";
  for (int w : widths) cout << string(w + 2, '-') << "+";
  cout << "\n";
}
void printRow(const vector<string>& cols, const vector<int>& widths) {
  cout << "  |";
  for (size_t i = 0; i < cols.size(); i++) cout << " " << pad(cols[i], widths[i]) << " |";
  cout << "\n";
}
void printTableHeader(const vector<string>& cols, const vector<int>& widths) {
  drawLine(widths);
  printRow(cols, widths);
  drawLine(widths);
}

string getPriorityStatus(double level) { return (level >= 80) ? "URGENT" : ((level >= 50) ? "Moderate" : "Normal"); }
string getActionRequired(double level) { return (level >= 80) ? "Collect Now" : ((level >= 50) ? "Schedule Soon" : "Routine Check"); }

// --- SECTION 1: CUSTOM DATA STRUCTURES (LINEAR) ---
// Stack: Used to maintain the Recovery Log (LIFO).
// Keeps track of the most recent actions so they can be reviewed or reversed.
class Stack {
  struct Node { string data; Node* next; };
  Node* topNode = nullptr;
  int sz = 0;

 public:
  ~Stack() {
    while (topNode) {
      Node* temp = topNode;
      topNode = topNode->next;
      delete temp;
    }
  }
  void push(string val) {
    topNode = new Node{val, topNode};
    sz++;
  }
  string pop() {
    if (!topNode) return "";
    Node* temp = topNode;
    string val = temp->data;
    topNode = topNode->next;
    delete temp;
    sz--;
    return val;
  }
  bool isEmpty() { return topNode == nullptr; }
  int size() { return sz; }
  void display() {
    if (!topNode) { cout << "  (Log is empty)\n"; return; }
    Node* current = topNode;
    int i = 1;
    vector<int> w = {5, 55};
    printTableHeader({"No.", "Action Description"}, w);
    while (current) {
      printRow({to_string(i++), current->data}, w);
      current = current->next;
    }
    drawLine(w);
  }
};

// Queue: Used for managing Citizen Service Requests (FIFO).
// Ensures that requests are processed in the order they were received.
class Queue {
  struct Node { ServiceRequest data; Node* next; };
  Node *frontNode = nullptr, *rearNode = nullptr;
  int sz = 0;

 public:
  ~Queue() {
    while (frontNode) {
      Node* temp = frontNode;
      frontNode = frontNode->next;
      delete temp;
    }
  }
  void enqueue(ServiceRequest req) {
    Node* temp = new Node{req, nullptr};
    if (!rearNode) frontNode = rearNode = temp;
    else { rearNode->next = temp; rearNode = temp; }
    sz++;
  }
  ServiceRequest dequeue() {
    if (!frontNode) return {0, "", "", "", ""};
    Node* temp = frontNode;
    ServiceRequest req = temp->data;
    frontNode = frontNode->next;
    if (!frontNode) rearNode = nullptr;
    delete temp;
    sz--;
    return req;
  }
  bool isEmpty() { return frontNode == nullptr; }
  int size() { return sz; }
  void display() {
    if (!frontNode) { cout << "  (No pending service requests in queue)\n"; return; }
    Node* current = frontNode;
    vector<int> w = {10, 15, 20, 30, 8};
    printTableHeader({"Request ID", "Citizen Name", "Location", "Description", "Urgency"}, w);
    while (current) {
      ServiceRequest r = current->data;
      printRow({"#" + to_string(r.id), r.name, r.location, r.desc, r.urgency}, w);
      current = current->next;
    }
    drawLine(w);
  }
};

// --- SECTION 2: TREES (HIERARCHICAL STORAGE FOR WASTE BINS) ---
// TreeNode: Shared node structure for both BST and AVL trees.
struct TreeNode {
  WasteBin bin;
  TreeNode *left, *right;
  int height;
};
void clearTree(TreeNode* node) {
  if (node) {
    clearTree(node->left);
    clearTree(node->right);
    delete node;
  }
}

void treeInorder(TreeNode* n, vector<int>& w) {
  if (n) {
    treeInorder(n->left, w);
    string s = getPriorityStatus(n->bin.wasteLevel);
    printRow({to_string(n->bin.id), n->bin.location, to_string((int)n->bin.wasteLevel) + "%", to_string((int)n->bin.capacity) + "L",
              n->bin.wasteType, s},
             w);
    treeInorder(n->right, w);
  }
}
void treeCollectPreorder(TreeNode* n, string& s) {
  if (n) {
    s += to_string(n->bin.id) + " ";
    treeCollectPreorder(n->left, s);
    treeCollectPreorder(n->right, s);
  }
}
void treePrintTable(TreeNode* n, vector<int>& w) {
  if (!n) return;
  string l = n->left ? "Bin " + to_string(n->left->bin.id) : "None";
  string r = n->right ? "Bin " + to_string(n->right->bin.id) : "None";
  printRow({to_string(n->bin.id), l, r}, w);
  treePrintTable(n->left, w);
  treePrintTable(n->right, w);
}

TreeNode* findMinNode(TreeNode* n) { while (n && n->left) n = n->left; return n; }
TreeNode* searchNode(TreeNode* n, int id) { return (!n || n->bin.id == id) ? n : (id < n->bin.id ? searchNode(n->left, id) : searchNode(n->right, id)); }
WasteBin* searchBin(TreeNode* root, int id) { TreeNode* t = searchNode(root, id); return t ? &(t->bin) : nullptr; }
void displayTree(TreeNode* root) {
  if (!root) { cout << "  (empty)\n"; return; }
  vector<int> w = {6, 22, 6, 9, 11, 9};
  printTableHeader({"ID", "Location", "Fill%", "Capacity", "Type", "Status"}, w);
  treeInorder(root, w);
  drawLine(w);
}

// Binary Search Tree (BST): Used as backup storage for Waste Bins.
// Provides basic O(log N) average time complexity for insertions and deletions.
class BST {
 public:
  TreeNode* root = nullptr;
  int count = 0;
  ~BST() { clearTree(root); }
  int getH(TreeNode* n) { return n ? 1 + max(getH(n->left), getH(n->right)) : 0; }
  int getHeight() { return getH(root); }
  TreeNode* ins(TreeNode* n, WasteBin b) {
    if (!n) {
      count++;
      return new TreeNode{b, nullptr, nullptr, 1};
    }
    if (b.id < n->bin.id) n->left = ins(n->left, b);
    else if (b.id > n->bin.id) n->right = ins(n->right, b);
    n->height = 1 + max(getH(n->left), getH(n->right));
    return n;
  }
  void insert(WasteBin b) { root = ins(root, b); }
  TreeNode* del(TreeNode* n, int id) {
    if (!n) return nullptr;
    if (id < n->bin.id) n->left = del(n->left, id);
    else if (id > n->bin.id) n->right = del(n->right, id);
    else {
      if (!n->left || !n->right) {
        TreeNode* temp = n->left ? n->left : n->right;
        delete n;
        count--;
        return temp;
      }
      TreeNode* temp = findMinNode(n->right);
      n->bin = temp->bin;
      n->right = del(n->right, temp->bin.id);
    }
    if (n) n->height = 1 + max(getH(n->left), getH(n->right));
    return n;
  }
  void remove(int id) { root = del(root, id); }
};

// AVL Tree: Used as primary storage for Waste Bins.
// Self-balancing binary search tree ensuring strictly O(log N) complexity.
class AVL {
 public:
  TreeNode* root = nullptr;
  int count = 0;
  ~AVL() { clearTree(root); }
  int getH(TreeNode* n) { return n ? n->height : 0; }
  int getHeight() { return getH(root); }
  int getBF(TreeNode* n) { return n ? getH(n->left) - getH(n->right) : 0; }
  void updH(TreeNode* n) { if (n) n->height = 1 + max(getH(n->left), getH(n->right)); }
  TreeNode* rotR(TreeNode* y) {
    TreeNode *x = y->left, *T2 = x->right;
    x->right = y;
    y->left = T2;
    updH(y);
    updH(x);
    return x;
  }
  TreeNode* rotL(TreeNode* x) {
    TreeNode *y = x->right, *T2 = y->left;
    y->left = x;
    x->right = T2;
    updH(x);
    updH(y);
    return y;
  }
  TreeNode* bal(TreeNode* n) {
    updH(n);
    int bf = getBF(n);
    if (bf > 1) { if (getBF(n->left) < 0) n->left = rotL(n->left); return rotR(n); }
    if (bf < -1) { if (getBF(n->right) > 0) n->right = rotR(n->right); return rotL(n); }
    return n;
  }
  TreeNode* ins(TreeNode* n, WasteBin b) {
    if (!n) {
      count++;
      return new TreeNode{b, nullptr, nullptr, 1};
    }
    if (b.id < n->bin.id) n->left = ins(n->left, b);
    else if (b.id > n->bin.id) n->right = ins(n->right, b);
    else return n;
    return bal(n);
  }
  void insert(WasteBin b) { root = ins(root, b); }
  TreeNode* del(TreeNode* n, int id) {
    if (!n) return nullptr;
    if (id < n->bin.id) n->left = del(n->left, id);
    else if (id > n->bin.id) n->right = del(n->right, id);
    else {
      if (!n->left || !n->right) {
        TreeNode* temp = n->left ? n->left : n->right;
        delete n;
        count--;
        return temp;
      }
      TreeNode* temp = findMinNode(n->right);
      n->bin = temp->bin;
      n->right = del(n->right, temp->bin.id);
    }
    if (!n) return nullptr;
    return bal(n);
  }
  void remove(int id) { root = del(root, id); }
  void coll(TreeNode* n, vector<WasteBin>& v) {
    if (n) {
      coll(n->left, v);
      v.push_back(n->bin);
      coll(n->right, v);
    }
  }
  vector<WasteBin> getAll() {
    vector<WasteBin> v;
    coll(root, v);
    return v;
  }
};

// --- SECTION 3: HASHING (VEHICLE MANAGEMENT) ---
// Hash Table with Chaining: Used for O(1) average time lookups of Vehicles.
class VehicleHash {
 public:
  struct Node { Vehicle data; Node* next; };
  static const int size = 53;
  Node* table[size];
  int count;

  VehicleHash() {
    count = 0;
    for (int i = 0; i < size; i++) table[i] = nullptr;
  }
  ~VehicleHash() {
    for (int i = 0; i < size; i++) {
      Node* current = table[i];
      while (current) {
        Node* temp = current;
        current = current->next;
        delete temp;
      }
    }
  }
  int getHashValue(string key) {
    unsigned long h = 0;
    for (size_t i = 0; i < key.length(); i++) h = (h * 31 + key[i]) % size;
    return (int)h;
  }
  void insert(Vehicle v) {
    int idx = getHashValue(v.plate);
    table[idx] = new Node{v, table[idx]};
    count++;
  }
  Vehicle* search(string plate) {
    int idx = getHashValue(plate);
    Node* current = table[idx];
    while (current) { if (current->data.plate == plate) return &(current->data); current = current->next; }
    return nullptr;
  }
  void displayAll() {
    vector<int> w = {14, 18, 18, 12, 8};
    printTableHeader({"Plate Number", "Driver Name", "Vehicle Type", "Status", "Zone"}, w);
    for (int i = 0; i < size; i++) {
      Node* current = table[i];
      while (current) {
        Vehicle v = current->data;
        printRow({v.plate, v.driver, v.type, v.status, v.zone}, w);
        current = current->next;
      }
    }
    drawLine(w);
  }
};

// --- SECTION 4: GRAPHS (CITY ROUTE OPTIMIZATION) ---
// Graph using Adjacency List: Represents city areas and roads.
// Supports BFS for reachability, DFS for coverage, and Dijkstra for shortest paths.
struct Edge { int dest, weight; };
class CityGraph {
 public:
  static const int maxVertices = 10;
  int V;
  string names[maxVertices];
  vector<Edge> adjList[maxVertices];
  CityGraph(int num) { V = num; }
  void setName(int idx, string name) { names[idx] = name; }
  string getName(int idx) { return names[idx]; }
  void addEdge(int u, int v, int w) {
    adjList[u].push_back(Edge{v, w});
    adjList[v].push_back(Edge{u, w});
  }
  void showAdj() {
    vector<int> w = {20, 50};
    printTableHeader({"Source Area", "Connected Areas (Distance)"}, w);
    for (int i = 0; i < V; i++) {
      string conns = "";
      for (size_t j = 0; j < adjList[i].size(); j++) { conns += names[adjList[i][j].dest] + " (" + to_string(adjList[i][j].weight) + "km), "; }
      if (conns.length() > 2) conns = conns.substr(0, conns.length() - 2);
      printRow({names[i], conns}, w);
    }
    drawLine(w);
  }
  void bfs(int start) {
    bool visited[maxVertices] = {false};
    vector<int> qArr;
    int front = 0;
    visited[start] = true;
    qArr.push_back(start);
    cout << "\n  Reachable areas starting from " << names[start] << ":\n  ";
    while (front < (int)qArr.size()) {
      int current = qArr[front++];
      cout << "[" << names[current] << "]";
      if (front < (int)qArr.size()) cout << " -> ";
      for (size_t i = 0; i < adjList[current].size(); i++) {
        Edge edge = adjList[current][i];
        if (!visited[edge.dest]) { visited[edge.dest] = true; qArr.push_back(edge.dest); }
      }
    }
    cout << "\n";
  }
  void dfsHelper(int vertex, bool visited[], int& count, string& path) {
    visited[vertex] = true;
    count++;
    path += "[" + names[vertex] + "]";
    for (size_t i = 0; i < adjList[vertex].size(); i++) {
      Edge edge = adjList[vertex][i];
      if (!visited[edge.dest]) { path += " -> "; dfsHelper(edge.dest, visited, count, path); }
    }
  }
  void dfs(int start) {
    bool visited[maxVertices] = {false};
    int count = 0;
    string path = "";
    dfsHelper(start, visited, count, path);
    cout << "\n  Coverage path starting from " << names[start] << ":\n  " << path << "\n";
    if (count == V) cout << "\n  Status: All " << V << " areas are connected.\n";
    else cout << "\n  Status: Only " << count << "/" << V << " areas connected.\n";
  }
  void dijkstra(int src, int dst) {
    int dist[maxVertices], prev[maxVertices];
    bool visited[maxVertices];
    for (int i = 0; i < V; i++) {
      dist[i] = 999999;
      prev[i] = -1;
      visited[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < V - 1; count++) {
      int minDist = 999999, u = -1;
      for (int v = 0; v < V; v++) {
        if (!visited[v] && dist[v] < minDist) { minDist = dist[v]; u = v; }
      }
      if (u == -1) break;
      visited[u] = true;
      for (size_t i = 0; i < adjList[u].size(); i++) {
        int neighbor = adjList[u][i].dest, weight = adjList[u][i].weight;
        if (!visited[neighbor] && dist[u] + weight < dist[neighbor]) { dist[neighbor] = dist[u] + weight; prev[neighbor] = u; }
      }
    }
    if (dist[dst] == 999999) { cout << "\n  [ALERT] No path exists.\n"; return; }
    vector<int> path;
    int current = dst;
    while (current != -1) { path.push_back(current); current = prev[current]; }
    cout << "\n  Shortest Route from " << names[src] << " to " << names[dst] << ":\n  ";
    for (int i = (int)path.size() - 1; i >= 0; i--) { cout << "[" << names[path[i]] << "]"; if (i > 0) cout << " --(" << (dist[path[i - 1]] - dist[path[i]]) << "km)--> "; }
    cout << "\n\n  Total Distance: " << dist[dst] << " km\n\n  Distances from " << names[src] << " to all areas:\n";
    vector<int> w = {22, 12};
    printTableHeader({"Destination Area", "Distance"}, w);
    for (int i = 0; i < V; i++) {
      string dStr = (dist[i] == 999999) ? "INF" : to_string(dist[i]) + " km";
      printRow({names[i], dStr}, w);
    }
    drawLine(w);
  }
};

// --- SECTION 5: SORTING ALGORITHMS (COLLECTION SCHEDULING) ---
// Quick Sort: Main sorting algorithm to prioritize bins by waste level (O(N log N)).
int partition(WasteBin arr[], int low, int high) {
  double pivot = getPriorityScore(arr[high]);
  int i = (low - 1);
  for (int j = low; j <= high - 1; j++) { if (getPriorityScore(arr[j]) > pivot) swap(arr[++i], arr[j]); }
  swap(arr[i + 1], arr[high]);
  return (i + 1);
}

void quickSort(WasteBin arr[], int low, int high) {
  if (low < high) {
    int pi = partition(arr, low, high);
    quickSort(arr, low, pi - 1);
    quickSort(arr, pi + 1, high);
  }
}

// --- SECTION 6: SEARCHING ALGORITHMS ---
// Binary Search: O(log N) search on a sorted array of bins by ID.
int binarySearch(WasteBin arr[], int n, int targetId) {
  int low = 0, high = n - 1;
  while (low <= high) {
    int mid = low + (high - low) / 2;
    if (arr[mid].id == targetId) return mid;
    if (arr[mid].id < targetId) low = mid + 1;
    else high = mid - 1;
  }
  return -1;
}

string toLowerCase(string str) {
  for (size_t i = 0; i < str.length(); i++) { str[i] = tolower((unsigned char)str[i]); }
  return str;
}
// Linear Search: O(N) search through bins to match location keywords (case-insensitive).
int linearSearch(WasteBin arr[], int n, string locKeyword) {
  string target = toLowerCase(locKeyword);
  for (int i = 0; i < n; i++) { if (toLowerCase(arr[i].location).find(target) != string::npos) return i; }
  return -1;
}

// --- SECTION 7: GLOBAL DATA INSTANCES & INITIALIZATION ---
// Global instances of core data structures driving the system.
AVL avl_tree;
BST bst_tree;
Queue request_queue;
Stack action_stack;
VehicleHash vehicle_hash;
CityGraph* city_graph = nullptr;
int nextRequestId = 6;

vector<WasteBin> getAllBins() { return avl_tree.getAll(); }

void loadBins() {
  for (int i = 0; i < 10; i++) { avl_tree.insert(sample_bins[i]); bst_tree.insert(sample_bins[i]); }
}
void loadVehicles() {
  for (int i = 0; i < 5; i++) { vehicle_hash.insert(sample_vehicles[i]); }
}
void loadRequests() {
  int maxId = 0;
  for (int i = 0; i < 5; i++) { request_queue.enqueue(sample_requests[i]); if (sample_requests[i].id > maxId) maxId = sample_requests[i].id; }
  nextRequestId = maxId + 1;
}
void loadSampleData() {
  loadBins();
  loadVehicles();
  loadRequests();

  city_graph = new CityGraph(10);
  for (int i = 0; i < 10; i++) { city_graph->setName(i, sample_city_names[i]); }
  for (int i = 0; i < 12; i++) { city_graph->addEdge(sample_edges[i].u, sample_edges[i].v, sample_edges[i].w); }

  action_stack.push("System initialized");
}

void printBinCard(WasteBin* ptr) {
  if (!ptr) return;
  cout << " === Waste Bin Details ===\n ID: " << ptr->id << "\n Location: " << ptr->location << "\n Fill Level: " << (int)ptr->wasteLevel << "%\n Capacity: " << (int)ptr->capacity << "L\n Type: " << ptr->wasteType << "\n Current Waste: " << (int)(ptr->wasteLevel * ptr->capacity / 100.0) << "L\n Priority Status: " << getPriorityStatus(ptr->wasteLevel) << "\n";
}

void showBinDatabase() {
  cout << "\n  === Waste Bin Records ===\n";
  displayTree(avl_tree.root);
  cout << "  Total Bins: " << avl_tree.count << " | Primary Storage Depth: " << avl_tree.getHeight() << "\n";
  action_stack.push("Viewed waste bin records");
  pressEnterToContinue();
}

// --- SECTION 8: MENU & BUSINESS LOGIC IMPLEMENTATION ---
// Manages adding, searching, and removing bins via AVL/BST structures.
void manageBins() {
  cout << "\n  +--------------------------------------------------+\n  |               Waste Bin Management               |\n  +--------------------------------------------------+\n  |  [1] View All Bins                               |\n  |  [2] Add New Bin                                 |\n  |  [3] Search Bin by ID                            |\n  |  [4] Search Bin by Location                      |\n  |  [5] Remove Bin                                  |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) { showBinDatabase(); return; } else if (choice == 2) {
    int id;
    string loc, type;
    double lvl, cap;
    cout << "\n  === Add New Bin ===\n";
    cout << "  Enter ID: ";
    cin >> id;
    cin.ignore();
    cout << "  Enter Location: ";
    getline(cin, loc);
    cout << "  Enter Fill Level (%): ";
    cin >> lvl;
    cout << "  Enter Type (Organic/General/Recyclable/Hazardous): ";
    cin >> type;
    cout << "  Enter Capacity (L): ";
    cin >> cap;
    cin.ignore();
    WasteBin temp = {id, loc, lvl, type, cap};
    avl_tree.insert(temp);
    bst_tree.insert(temp);
    cout << "\n  [SUCCESS] New Bin Added Successfully!\n";
    printBinCard(&temp);
    action_stack.push("Inserted Bin " + to_string(id));
  } else if (choice == 3) {
    int id;
    cout << "\n  Enter Bin ID to search: ";
    cin >> id;
    cin.ignore();
    vector<WasteBin> tempArr = getAllBins();
    int idx = binarySearch(tempArr.data(), tempArr.size(), id);
    if (idx != -1) { cout << "\n  [RESULT] Bin Found using Binary Search on sorted records:\n"; printBinCard(&tempArr[idx]); } else cout << "\n  [ALERT] Bin ID " << id << " was not found.\n";
    action_stack.push("Binary searched Bin " + to_string(id));
  } else if (choice == 4) {
    string keyword;
    cout << "\n  Enter location keyword: ";
    getline(cin, keyword);
    vector<WasteBin> tempArr = getAllBins();
    int idx = linearSearch(tempArr.data(), tempArr.size(), keyword);
    if (idx != -1) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(&tempArr[idx]); } else cout << "\n  [ALERT] Location containing '" << keyword << "' not found.\n";
    action_stack.push("Searched for location: " + keyword);
  } else if (choice == 5) {
    int id;
    cout << "\n  Enter Bin ID to delete: ";
    cin >> id;
    cin.ignore();
    if (searchBin(avl_tree.root, id)) {
      avl_tree.remove(id);
      bst_tree.remove(id);
      cout << "\n  [SUCCESS] Bin " << id << " removed successfully.\n";
      action_stack.push("Deleted Bin " + to_string(id));
    } else cout << "\n  [ALERT] Bin ID " << id << " was not found.\n";
  }
  if (choice != 0) pressEnterToContinue();
}

// Generates a prioritized collection list using Quick Sort.
void collectionSchedule() {
  vector<WasteBin> tempArr = getAllBins();
  if (tempArr.empty()) {
    cout << "  No bins available.\n";
    pressEnterToContinue();
    return;
  }
  quickSort(tempArr.data(), 0, tempArr.size() - 1);

  cout << "\n  === Collection Priority List ===\n";
  cout << "  Sorting Engine: Quick Sort\n";
  vector<int> w = {10, 8, 22, 6, 17};
  printTableHeader({"Priority", "Bin ID", "Location", "Fill", "Action Required"}, w);
  for (size_t i = 0; i < tempArr.size(); i++) {
    string action = getActionRequired(tempArr[i].wasteLevel);
    printRow({to_string(i + 1), to_string(tempArr[i].id), tempArr[i].location, to_string((int)tempArr[i].wasteLevel) + "%", action}, w);
  }
  drawLine(w);
  action_stack.push("Viewed collection priority list");
  pressEnterToContinue();
}

// Handles citizen requests via Queue (Enqueue/Dequeue).
void serviceRequests() {
  cout << "\n  +--------------------------------------------------+\n  |                 Service Requests                 |\n  +--------------------------------------------------+\n  |  [1] View Pending Requests                       |\n  |  [2] Submit New Request                          |\n  |  [3] Process Next Request                        |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) { cout << "\n  === Pending Service Requests ===\n"; request_queue.display(); } else if (choice == 2) {
    string name, location, desc, urgency;
    cout << "\n  === Submit Service Request ===\n";
    cout << "  Enter Citizen Name: ";
    getline(cin, name);
    cout << "  Enter Location: ";
    getline(cin, location);
    cout << "  Enter Description: ";
    getline(cin, desc);
    cout << "  Enter Urgency (Low/Medium/High): ";
    getline(cin, urgency);
    request_queue.enqueue({nextRequestId++, name, location, desc, urgency});
    cout << "\n  [SUCCESS] Request Submitted Successfully!\n";
    action_stack.push("Submitted service request");
  } else if (choice == 3) {
    if (request_queue.isEmpty()) cout << "  No pending requests.\n";
    else {
      ServiceRequest r = request_queue.dequeue();
      cout << "\n === Processing Request ===\n Request ID: #" << r.id << "\n Citizen: " << r.name << "\n Location: " << r.location << "\n Details: " << r.desc << "\n Urgency: " << r.urgency << "\n Status: Completed.\n";
      action_stack.push("Processed request #" + to_string(r.id));
    }
  }
  if (choice != 0) pressEnterToContinue();
}

// Recovers and reviews past actions via Stack (Push/Pop).
void operationalRecovery() {
  cout << "\n  +--------------------------------------------------+\n  |                   Recovery Log                   |\n  +--------------------------------------------------+\n  |  [1] View Log History                            |\n  |  [2] Clear Last Log Entry                        |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) { cout << "\n  === Recovery Log History ===\n"; action_stack.display(); } else if (choice == 2) {
    if (action_stack.isEmpty()) cout << "  No past actions to recover.\n";
    else { string lastAction = action_stack.pop(); cout << "\n  [SUCCESS] Cleared Entry: " << lastAction << "\n"; }
  }
  if (choice != 0) pressEnterToContinue();
}

// Registers and searches vehicles using Hash Map (Chaining).
void vehicleRegistry() {
  cout << "\n  +--------------------------------------------------+\n  |                Vehicle Management                |\n  +--------------------------------------------------+\n  |  [1] View Registered Vehicles                    |\n  |  [2] Search Vehicle                              |\n  |  [3] Add New Vehicle                             |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) { cout << "\n  === Registered Vehicles ===\n"; vehicle_hash.displayAll(); } else if (choice == 2) {
    string p;
    cout << "  Enter Plate Number: ";
    cin >> p;
    cin.ignore();
    Vehicle* v = vehicle_hash.search(p);
    if (v) {
      cout << "\n === Vehicle Details ===\n Plate Number: " << v->plate << "\n Driver: " << v->driver << "\n Type: " << v->type << "\n Status: " << v->status << "\n Zone: " << v->zone << "\n";
    } else cout << "\n  [ALERT] Vehicle " << p << " not found.\n";
    action_stack.push("Searched for vehicle plate: " + p);
  } else if (choice == 3) {
    string pl, dr, ty, st, zn;
    cout << "\n  === Register Vehicle ===\n";
    cout << "  Enter Plate: ";
    getline(cin, pl);
    cout << "  Enter Driver: ";
    getline(cin, dr);
    cout << "  Enter Type: ";
    getline(cin, ty);
    cout << "  Enter Status: ";
    getline(cin, st);
    cout << "  Enter Zone: ";
    getline(cin, zn);
    vehicle_hash.insert({pl, dr, ty, st, zn});
    cout << "\n  [SUCCESS] Vehicle Registered Successfully.\n";
    action_stack.push("Registered vehicle " + pl);
  }
  if (choice != 0) pressEnterToContinue();
}

// Computes reachability (BFS), coverage (DFS), and shortest path (Dijkstra).
void routeOptimizer() {
  cout << "\n  +--------------------------------------------------+\n  |                  Route Planning                  |\n  +--------------------------------------------------+\n  |  [1] Show City Map                               |\n  |  [2] View Reachable Service Areas                |\n  |  [3] Check City Network Coverage                 |\n  |  [4] Find Shortest Collection Route              |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) { cout << "\n  === City Connections Map ===\n"; city_graph->showAdj(); } else if (choice == 2) {
    int s;
    cout << "  Enter starting zone number (0-9): ";
    cin >> s;
    cin.ignore();
    if (s >= 0 && s < 10) { city_graph->bfs(s); action_stack.push("Checked connections from " + city_graph->getName(s)); } else cout << "  Invalid.\n";
  } else if (choice == 3) {
    int s;
    cout << "  Enter starting zone number (0-9): ";
    cin >> s;
    cin.ignore();
    if (s >= 0 && s < 10) { city_graph->dfs(s); action_stack.push("Checked network connectivity from " + city_graph->getName(s)); } else cout << "  Invalid.\n";
  } else if (choice == 4) {
    for (int i = 0; i < 10; i++) cout << "    [" << i << "] " << city_graph->getName(i) << "\n";
    int src, dst;
    cout << "  Enter Source area number: ";
    cin >> src;
    cout << "  Enter Destination area number: ";
    cin >> dst;
    cin.ignore();
    if (src >= 0 && src < 10 && dst >= 0 && dst < 10) { city_graph->dijkstra(src, dst); action_stack.push("Planned route from " + city_graph->getName(src) + " to " + city_graph->getName(dst)); } else cout << "  Invalid area numbers.\n";
  }
  if (choice != 0) pressEnterToContinue();
}

// Generates metrics and statistics across the whole system.
void environmentalReport() {
  vector<WasteBin> tempArr = getAllBins();
  int urgentCount = 0;
  double totalVolume = 0, recyclableVolume = 0;
  for (size_t i = 0; i < tempArr.size(); i++) {
    double volume = tempArr[i].wasteLevel * tempArr[i].capacity / 100.0;
    totalVolume += volume;
    if (tempArr[i].wasteLevel >= 80) urgentCount++;
    if (tempArr[i].wasteType == "Recyclable") recyclableVolume += volume;
  }
  double recRate = totalVolume > 0 ? (recyclableVolume / totalVolume) * 100.0 : 0.0;
  int rInt = (int)recRate, rDec = (int)((recRate - rInt) * 10), filled = (int)(recRate / 6.66);
  string bar = string(filled, '=') + string(15 - filled, ' ');

  cout << "\n  === Environmental Report ===\n";
  vector<int> w = {38, 15};
  printTableHeader({"Metric", "Value"}, w);
  printRow({"Total Bins Monitored", to_string(tempArr.size())}, w);
  printRow({"Bins Needing Collection (>=80%)", to_string(urgentCount)}, w);
  printRow({"Estimated Waste Volume", to_string((int)totalVolume) + " L"}, w);
  printRow({"Recyclable Waste Share", to_string(rInt) + "." + to_string(rDec) + "%"}, w);
  printRow({"Recycling Share Progress", "[" + bar + "]"}, w);
  printRow({"Pending Citizen Issues", to_string(request_queue.size())}, w);
  printRow({"Registered Vehicles", to_string(vehicle_hash.count)}, w);
  drawLine(w);
  action_stack.push("Viewed Environmental Report");
  pressEnterToContinue();
}

void complexityAnalysis() {
  cout << "\n  === Technical Analysis Complexity ===\n";
  vector<int> w = {26, 18, 12};
  printTableHeader({"Feature Area", "Time Complexity", "Extra Space"}, w);
  printRow({"Bin add/search/remove", "O(log N)", "O(1)"}, w);
  printRow({"Vehicle plate lookup", "O(1) average", "O(1)"}, w);
  printRow({"Recovery log operation", "O(1)", "O(1)"}, w);
  printRow({"Service request handling", "O(1)", "O(1)"}, w);
  printRow({"Quick sort scheduling", "O(N log N) avg", "O(log N)"}, w);
  printRow({"Binary search by ID", "O(log N)", "O(1)"}, w);
  printRow({"Search by location", "O(N)", "O(1)"}, w);
  printRow({"Area coverage check", "O(V + E)", "O(V)"}, w);
  printRow({"Shortest route planning", "O(V^2 + E)", "O(V)"}, w);
  drawLine(w);
  cout << "  Scale Info: N = bins count, V = zones (10), E = connections (12)\n";
  action_stack.push("Viewed technical analysis summary");
  pressEnterToContinue();
}

// In-depth technical breakdown showing the structures used and their complexity.
void technicalAnalysis() {
  cout << "\n  +--------------------------------------------------+\n  |                Technical Analysis                |\n  +--------------------------------------------------+\n  |  [1] Primary Storage Status                      |\n  |  [2] Backup Storage Status                       |\n  |  [3] Storage Comparison                          |\n  |  [4] Feature Performance Summary                 |\n  |  [0] Back                                        |\n  +--------------------------------------------------+\n  Enter choice: ";
  int choice = readChoice();
  if (choice == 1) {
    cout << "\n  === Primary Storage Status (AVL Tree) ===\n";
    displayTree(avl_tree.root);
    string pStr = "";
    treeCollectPreorder(avl_tree.root, pStr);
    cout << "\n  Preorder Traversal: " << pStr << "\n\n  === AVL Node Connections Table ===\n";
    vector<int> w = {10, 15, 15};
    printTableHeader({"Node ID", "Left Child", "Right Child"}, w);
    treePrintTable(avl_tree.root, w);
    drawLine(w);
  } else if (choice == 2) {
    cout << "\n  === Backup Storage Status (BST Tree) ===\n";
    displayTree(bst_tree.root);
    string pStr = "";
    treeCollectPreorder(bst_tree.root, pStr);
    cout << "\n  Preorder Traversal: " << pStr << "\n\n  === BST Node Connections Table ===\n";
    vector<int> w = {10, 15, 15};
    printTableHeader({"Node ID", "Left Child", "Right Child"}, w);
    treePrintTable(bst_tree.root, w);
    drawLine(w);
  } else if (choice == 3) {
    cout << "\n  === Storage Comparison ===\n";
    vector<int> w = {25, 15};
    printTableHeader({"Storage Structure", "Tree Height"}, w);
    printRow({"AVL Tree (Primary)", to_string(avl_tree.getHeight())}, w);
    printRow({"BST Tree (Backup)", to_string(bst_tree.getHeight())}, w);
    drawLine(w);
  } else if (choice == 4) { complexityAnalysis(); return; }
  if (choice != 0) pressEnterToContinue();
}

// --- SECTION 9: MAIN APPLICATION ENTRY POINT ---
// Drives the application event loop and main menu UI.
int main() {
  loadSampleData();
  while (true) {
    vector<WasteBin> tempArr = getAllBins();
    int urgentCount = 0;
    for (size_t i = 0; i < tempArr.size(); i++) { if (tempArr[i].wasteLevel >= 80) urgentCount++; }
    string recommendedAction;
    if (urgentCount > 0) recommendedAction = "Review Collection Planning";
    else if (request_queue.size() > 0) recommendedAction = "Process Citizen Requests";
    else recommendedAction = "System operating normally";

    cout << "\n  +--------------------------------------------------------+\n  |  Smart Waste Collection & Recycling Management System  |\n  +--------------------------------------------------------+\n  |  System Status                                         |\n  |    Total Bins          : " << pad(to_string(tempArr.size()), 29) << " |\n  |    Urgent Bins         : " << pad(to_string(urgentCount) + " need collection", 29) << " |\n  |    Pending Requests    : " << pad(to_string(request_queue.size()) + " citizen issues", 29) << " |\n  |    Registered Vehicles : " << pad(to_string(vehicle_hash.count), 29) << " |\n  |    Recommended Action  : " << pad(recommendedAction, 29) << " |\n  +--------------------------------------------------------+\n  |  [1] Waste Bin Management                              |\n  |  [2] Collection Planning                               |\n  |  [3] Citizen Service Requests                          |\n  |  [4] Vehicle Management                                |\n  |  [5] Route Planning                                    |\n  |  [6] Environmental Report                              |\n  |  [7] Technical Analysis                                |\n  |  [8] Recovery Log                                      |\n  |  [0] Exit                                              |\n  +--------------------------------------------------------+\n  Enter Choice: ";
    int choice = readChoice();
    if (choice == 1) manageBins();
    else if (choice == 2) collectionSchedule();
    else if (choice == 3) serviceRequests();
    else if (choice == 4) vehicleRegistry();
    else if (choice == 5) routeOptimizer();
    else if (choice == 6) environmentalReport();
    else if (choice == 7) technicalAnalysis();
    else if (choice == 8) operationalRecovery();
    else if (choice == 0) {
      cout << "\n  Thank you for using the Smart Waste Management System!\n";
      delete city_graph;
      break;
    } else cout << "  Invalid choice. Try again.\n";
  }
  return 0;
}
