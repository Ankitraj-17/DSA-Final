#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cctype>
using namespace std;

// Custom utility helpers replacing <algorithm> functions
template <typename T>
T maxVal(T a, T b) { return (a > b) ? a : b; }

template <typename T>
void swapVal(T& a, T& b) {
    T temp = a;
    a = b;
    b = temp;
}

struct WasteBin {
    int id;
    string location;
    double wasteLevel; // 0 to 100
    string wasteType;  // Organic, General, Recyclable, Hazardous
    double capacity;   // Liters
};

double getPriorityScore(WasteBin b) { return b.wasteLevel * (b.capacity / 100.0); }
struct Vehicle { string plate, driver, type, status, zone; };
struct ServiceRequest { int id; string name, location, desc, urgency; };

int readChoice() {
    int val = 0;
    if (!(cin >> val)) {
        if (cin.eof()) {
            exit(0);
        }
        cin.clear();
    }
    cin.ignore(10000, '\n');
    return val;
}
void pressEnterToContinue() {
    cout << "\n  Press Enter to continue...";
    if (cin.eof()) exit(0);
    cin.get();
}

string pad(string s, size_t len, bool rightAlign = false) {
    if (s.length() >= len) return s.substr(0, len);
    return rightAlign ? string(len - s.length(), ' ') + s : s + string(len - s.length(), ' ');
}
void drawLine(const vector<int>& widths) {
    cout << "  +"; for (int w : widths) cout << string(w + 2, '-') << "+"; cout << "\n";
}
void printRow(const vector<string>& cols, const vector<int>& widths) {
    cout << "  |"; for (size_t i = 0; i < cols.size(); i++) cout << " " << pad(cols[i], widths[i]) << " |"; cout << "\n";
}

class Stack {
    struct Node {
        string data;
        Node* next;
    };
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
        Node* temp = new Node{val, topNode};
        topNode = temp;
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
        int i = 1; vector<int> w = {5, 55}; drawLine(w);
        printRow({"No.", "Action Description"}, w); drawLine(w);
        while (current) {
            printRow({to_string(i++), current->data}, w);
            current = current->next;
        }
        drawLine(w);
    }
};

class Queue {
    struct Node {
        ServiceRequest data;
        Node* next;
    };
    Node* frontNode = nullptr;
    Node* rearNode = nullptr;
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
        if (!rearNode) {
            frontNode = rearNode = temp;
        } else {
            rearNode->next = temp;
            rearNode = temp;
        }
        sz++;
    }
    ServiceRequest dequeue() {
        if (!frontNode) return {0, "", "", "", ""};
        Node* temp = frontNode;
        ServiceRequest req = temp->data;
        frontNode = frontNode->next;
        if (!frontNode) {
            rearNode = nullptr;
        }
        delete temp;
        sz--;
        return req;
    }
    bool isEmpty() { return frontNode == nullptr; }
    int size() { return sz; }
    void display() {
        if (!frontNode) { cout << "  (No pending service requests in queue)\n"; return; }
        Node* current = frontNode;
        vector<int> w = {10, 15, 20, 30, 8}; drawLine(w);
        printRow({"Request ID", "Citizen Name", "Location", "Description", "Urgency"}, w); drawLine(w);
        while (current) {
            ServiceRequest r = current->data;
            printRow({"#" + to_string(r.id), r.name, r.location, r.desc, r.urgency}, w);
            current = current->next;
        }
        drawLine(w);
    }
};

struct TreeNode { WasteBin bin; TreeNode *left, *right; int height; };
void clearTree(TreeNode* node) { if (node) { clearTree(node->left); clearTree(node->right); delete node; } }

class BST {
public:
    TreeNode* root = nullptr; int count = 0;
    ~BST() { clearTree(root); }
    int getH(TreeNode* n) { return n ? 1 + maxVal(getH(n->left), getH(n->right)) : 0; }
    int getHeight() { return getH(root); }
    TreeNode* ins(TreeNode* n, WasteBin b) {
        if (!n) { count++; return new TreeNode{b, nullptr, nullptr, 1}; }
        if (b.id < n->bin.id) n->left = ins(n->left, b);
        else if (b.id > n->bin.id) n->right = ins(n->right, b);
        n->height = 1 + maxVal(getH(n->left), getH(n->right)); return n;
    }
    void insert(WasteBin b) { root = ins(root, b); }
    TreeNode* findMin(TreeNode* n) { while (n && n->left) n = n->left; return n; }
    TreeNode* del(TreeNode* n, int id) {
        if (!n) return nullptr;
        if (id < n->bin.id) n->left = del(n->left, id);
        else if (id > n->bin.id) n->right = del(n->right, id);
        else {
            if (!n->left || !n->right) { TreeNode* temp = n->left ? n->left : n->right; delete n; count--; return temp; }
            TreeNode* temp = findMin(n->right); n->bin = temp->bin; n->right = del(n->right, temp->bin.id);
        }
        if (n) n->height = 1 + maxVal(getH(n->left), getH(n->right)); return n;
    }
    void remove(int id) { root = del(root, id); }
    TreeNode* sh(TreeNode* n, int id) { return (!n || n->bin.id == id) ? n : (id < n->bin.id ? sh(n->left, id) : sh(n->right, id)); }
    WasteBin* search(int id) { TreeNode* t = sh(root, id); return t ? &(t->bin) : nullptr; }
    void inorder(TreeNode* n, vector<int>& w) {
        if (n) { inorder(n->left, w); string s = (n->bin.wasteLevel >= 80) ? "URGENT" : ((n->bin.wasteLevel >= 50) ? "Moderate" : "Normal");
            printRow({to_string(n->bin.id), n->bin.location, to_string((int)n->bin.wasteLevel) + "%", to_string((int)n->bin.capacity) + "L", n->bin.wasteType, s}, w); inorder(n->right, w); }
    }
    void display() { if (root) { vector<int> w = {6, 22, 6, 9, 11, 9}; drawLine(w); printRow({"ID", "Location", "Fill%", "Capacity", "Type", "Status"}, w); drawLine(w); inorder(root, w); drawLine(w); } else cout << "  (empty)\n"; }
    void collectPreorder(TreeNode* n, string& s) { if (n) { s += to_string(n->bin.id) + " "; collectPreorder(n->left, s); collectPreorder(n->right, s); } }
    void printTreeTable(TreeNode* n, vector<int>& w) {
        if (!n) return; string l = n->left ? "Bin " + to_string(n->left->bin.id) : "None", r = n->right ? "Bin " + to_string(n->right->bin.id) : "None";
        printRow({to_string(n->bin.id), l, r}, w); printTreeTable(n->left, w); printTreeTable(n->right, w);
    }
};

class AVL {
public:
    TreeNode* root = nullptr; int count = 0;
    ~AVL() { clearTree(root); }
    int getH(TreeNode* n) { return n ? n->height : 0; }
    int getHeight() { return getH(root); }
    int getBF(TreeNode* n) { return n ? getH(n->left) - getH(n->right) : 0; }
    void updH(TreeNode* n) { if (n) n->height = 1 + maxVal(getH(n->left), getH(n->right)); }
    TreeNode* rotR(TreeNode* y) { TreeNode* x = y->left, *T2 = x->right; x->right = y; y->left = T2; updH(y); updH(x); return x; }
    TreeNode* rotL(TreeNode* x) { TreeNode* y = x->right, *T2 = y->left; y->left = x; x->right = T2; updH(x); updH(y); return y; }
    TreeNode* bal(TreeNode* n) {
        updH(n); int bf = getBF(n);
        if (bf > 1) { if (getBF(n->left) < 0) n->left = rotL(n->left); return rotR(n); }
        if (bf < -1) { if (getBF(n->right) > 0) n->right = rotR(n->right); return rotL(n); }
        return n;
    }
    TreeNode* ins(TreeNode* n, WasteBin b) {
        if (!n) { count++; return new TreeNode{b, nullptr, nullptr, 1}; }
        if (b.id < n->bin.id) n->left = ins(n->left, b);
        else if (b.id > n->bin.id) n->right = ins(n->right, b);
        else return n;
        return bal(n);
    }
    void insert(WasteBin b) { root = ins(root, b); }
    TreeNode* findMin(TreeNode* n) { while (n && n->left) n = n->left; return n; }
    TreeNode* del(TreeNode* n, int id) {
        if (!n) return nullptr;
        if (id < n->bin.id) n->left = del(n->left, id);
        else if (id > n->bin.id) n->right = del(n->right, id);
        else {
            if (!n->left || !n->right) { TreeNode* temp = n->left ? n->left : n->right; delete n; count--; return temp; }
            TreeNode* temp = findMin(n->right); n->bin = temp->bin; n->right = del(n->right, temp->bin.id);
        }
        if (!n) return nullptr;
        return bal(n);
    }
    void remove(int id) { root = del(root, id); }
    TreeNode* sh(TreeNode* n, int id) { return (!n || n->bin.id == id) ? n : (id < n->bin.id ? sh(n->left, id) : sh(n->right, id)); }
    WasteBin* search(int id) { TreeNode* t = sh(root, id); return t ? &(t->bin) : nullptr; }
    void inorder(TreeNode* n, vector<int>& w) {
        if (n) { inorder(n->left, w); string s = (n->bin.wasteLevel >= 80) ? "URGENT" : ((n->bin.wasteLevel >= 50) ? "Moderate" : "Normal");
            printRow({to_string(n->bin.id), n->bin.location, to_string((int)n->bin.wasteLevel) + "%", to_string((int)n->bin.capacity) + "L", n->bin.wasteType, s}, w); inorder(n->right, w); }
    }
    void display() { if (root) { vector<int> w = {6, 22, 6, 9, 11, 9}; drawLine(w); printRow({"ID", "Location", "Fill%", "Capacity", "Type", "Status"}, w); drawLine(w); inorder(root, w); drawLine(w); } else cout << "  (empty)\n"; }
    void coll(TreeNode* n, WasteBin arr[], int& i) { if (n) { coll(n->left, arr, i); arr[i++] = n->bin; coll(n->right, arr, i); } }
    void getAll(WasteBin arr[], int& c) { c = 0; coll(root, arr, c); }
    void collectPreorder(TreeNode* n, string& s) { if (n) { s += to_string(n->bin.id) + " "; collectPreorder(n->left, s); collectPreorder(n->right, s); } }
    void printTreeTable(TreeNode* n, vector<int>& w) {
        if (!n) return; string l = n->left ? "Bin " + to_string(n->left->bin.id) : "None", r = n->right ? "Bin " + to_string(n->right->bin.id) : "None";
        printRow({to_string(n->bin.id), l, r}, w); printTreeTable(n->left, w); printTreeTable(n->right, w);
    }
};

class VehicleHash {
public:
    struct Node {
        Vehicle data;
        Node* next;
    };
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
        Node* newNode = new Node{v, table[idx]};
        table[idx] = newNode;
        count++;
    }
    Vehicle* search(string plate) {
        int idx = getHashValue(plate);
        Node* current = table[idx];
        while (current) {
            if (current->data.plate == plate) return &(current->data);
            current = current->next;
        }
        return nullptr;
    }
    void displayAll() {
        vector<int> w = {14, 18, 18, 12, 8}; drawLine(w); printRow({"Plate Number", "Driver Name", "Vehicle Type", "Status", "Zone"}, w); drawLine(w);
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

struct Edge { int dest, weight; };
class CityGraph {
public:
    static const int maxVertices = 10; int V; string names[maxVertices]; vector<Edge> adjList[maxVertices];
    CityGraph(int num) { V = num; }
    void setName(int idx, string name) { names[idx] = name; }
    string getName(int idx) { return names[idx]; }
    void addEdge(int u, int v, int w) { adjList[u].push_back(Edge{v, w}); adjList[v].push_back(Edge{u, w}); }
    void showAdj() {
        vector<int> w = {20, 50}; drawLine(w); printRow({"Source Area", "Connected Areas (Distance)"}, w); drawLine(w);
        for (int i = 0; i < V; i++) {
            string conns = ""; for (size_t j = 0; j < adjList[i].size(); j++) conns += names[adjList[i][j].dest] + " (" + to_string(adjList[i][j].weight) + "km), ";
            if (conns.length() > 2) conns = conns.substr(0, conns.length() - 2);
            printRow({names[i], conns}, w);
        }
        drawLine(w);
    }
    void bfs(int start) {
        bool visited[maxVertices] = {false}; vector<int> qArr; int front = 0; visited[start] = true; qArr.push_back(start);
        cout << "\n  Reachable areas starting from " << names[start] << ":\n  ";
        while (front < (int)qArr.size()) {
            int current = qArr[front++]; cout << "[" << names[current] << "]"; if (front < (int)qArr.size()) cout << " -> ";
            for (size_t i = 0; i < adjList[current].size(); i++) {
                Edge edge = adjList[current][i];
                if (!visited[edge.dest]) { visited[edge.dest] = true; qArr.push_back(edge.dest); }
            }
        }
        cout << "\n";
    }
    void dfsHelper(int vertex, bool visited[], int& count, string& path) {
        visited[vertex] = true; count++; path += "[" + names[vertex] + "]";
        for (size_t i = 0; i < adjList[vertex].size(); i++) {
            Edge edge = adjList[vertex][i];
            if (!visited[edge.dest]) { path += " -> "; dfsHelper(edge.dest, visited, count, path); }
        }
    }
    void dfs(int start) {
        bool visited[maxVertices] = {false}; int count = 0; string path = ""; dfsHelper(start, visited, count, path);
        cout << "\n  Coverage path starting from " << names[start] << ":\n  " << path << "\n";
        if (count == V) cout << "\n  Status: All " << V << " areas are connected.\n";
        else cout << "\n  Status: Only " << count << "/" << V << " areas connected.\n";
    }
    void dijkstra(int src, int dst) {
        int dist[maxVertices], prev[maxVertices]; bool visited[maxVertices];
        for (int i = 0; i < V; i++) { dist[i] = 999999; prev[i] = -1; visited[i] = false; }
        dist[src] = 0;
        for (int count = 0; count < V - 1; count++) {
            int minDist = 999999, u = -1;
            for (int v = 0; v < V; v++) { if (!visited[v] && dist[v] < minDist) { minDist = dist[v]; u = v; } }
            if (u == -1) break; visited[u] = true;
            for (size_t i = 0; i < adjList[u].size(); i++) {
                int neighbor = adjList[u][i].dest, weight = adjList[u][i].weight;
                if (!visited[neighbor] && dist[u] + weight < dist[neighbor]) { dist[neighbor] = dist[u] + weight; prev[neighbor] = u; }
            }
        }
        if (dist[dst] == 999999) { cout << "\n  [ALERT] No path exists.\n"; return; }
        vector<int> path; int current = dst; while (current != -1) { path.push_back(current); current = prev[current]; }
        cout << "\n  Shortest Route from " << names[src] << " to " << names[dst] << ":\n  ";
        for (int i = (int)path.size() - 1; i >= 0; i--) {
            cout << "[" << names[path[i]] << "]";
            if (i > 0) cout << " --(" << (dist[path[i - 1]] - dist[path[i]]) << "km)--> ";
        }
        cout << "\n\n  Total Distance: " << dist[dst] << " km\n\n  Distances from " << names[src] << " to all areas:\n";
        vector<int> w = {22, 12}; drawLine(w); printRow({"Destination Area", "Distance"}, w); drawLine(w);
        for (int i = 0; i < V; i++) { string dStr = (dist[i] == 999999) ? "INF" : to_string(dist[i]) + " km"; printRow({names[i], dStr}, w); }
        drawLine(w);
    }
};

void heapify(WasteBin arr[], int n, int i) {
    int smallest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && getPriorityScore(arr[l]) < getPriorityScore(arr[smallest]))
        smallest = l;

    if (r < n && getPriorityScore(arr[r]) < getPriorityScore(arr[smallest]))
        smallest = r;

    if (smallest != i) {
        swapVal(arr[i], arr[smallest]);
        heapify(arr, n, smallest);
    }
}

void heapSort(WasteBin arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (int i = n - 1; i > 0; i--) {
        swapVal(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

int partition(WasteBin arr[], int low, int high) {
    double pivot = getPriorityScore(arr[high]);
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (getPriorityScore(arr[j]) > pivot) {
            i++;
            swapVal(arr[i], arr[j]);
        }
    }
    swapVal(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(WasteBin arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int binarySearch(WasteBin arr[], int n, int targetId) {
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid].id == targetId) return mid;
        if (arr[mid].id < targetId) low = mid + 1; else high = mid - 1;
    }
    return -1;
}

string toLowerCase(string str) {
    for (size_t i = 0; i < str.length(); i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
    return str;
}
int linearSearch(WasteBin arr[], int n, string locKeyword) {
    string target = toLowerCase(locKeyword);
    for (int i = 0; i < n; i++) { if (toLowerCase(arr[i].location).find(target) != string::npos) return i; }
    return -1;
}

AVL avl_tree; BST bst_tree; Queue request_queue; Stack action_stack; VehicleHash vehicle_hash; CityGraph* city_graph = nullptr; int nextRequestId = 6;

void saveBins() {
    ofstream out("bins.txt"); if (!out) return;
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size); out << size << "\n";
    for (int i = 0; i < size; i++) out << tempArr[i].id << "\n" << tempArr[i].location << "\n" << tempArr[i].wasteLevel << "\n" << tempArr[i].wasteType << "\n" << tempArr[i].capacity << "\n";
    out.close();
}
void loadBins() {
    ifstream in("bins.txt");
    if (!in) {
        WasteBin bins[10] = {
            {101, "Main Street", 92.0, "Organic", 500}, {102, "Park Avenue", 45.0, "Recyclable", 300},
            {103, "Industrial Area", 88.5, "General", 1000}, {104, "Residential North", 30.0, "Organic", 400},
            {105, "Residential South", 76.0, "Recyclable", 400}, {106, "Market District", 95.0, "General", 600},
            {107, "Commercial Hub", 55.0, "Hazardous", 750}, {108, "University Area", 40.0, "Recyclable", 350},
            {109, "Town Square", 83.0, "Organic", 450}, {110, "Riverside Zone", 62.0, "General", 550}
        };
        for (int i = 0; i < 10; i++) { avl_tree.insert(bins[i]); bst_tree.insert(bins[i]); }
        saveBins(); return;
    }
    int size; if (!(in >> size)) return;
    for (int i = 0; i < size; i++) {
        WasteBin b; in >> b.id; in.ignore(); getline(in, b.location); in >> b.wasteLevel; in.ignore(); getline(in, b.wasteType); in >> b.capacity;
        avl_tree.insert(b); bst_tree.insert(b);
    }
    in.close();
}
void saveVehicles() {
    ofstream out("vehicles.txt"); if (!out) return; out << vehicle_hash.count << "\n";
    for (int i = 0; i < VehicleHash::size; i++) {
        VehicleHash::Node* current = vehicle_hash.table[i];
        while (current) {
            Vehicle v = current->data;
            out << v.plate << "\n" << v.driver << "\n" << v.type << "\n" << v.status << "\n" << v.zone << "\n";
            current = current->next;
        }
    }
    out.close();
}
void loadVehicles() {
    ifstream in("vehicles.txt");
    if (!in) {
        vehicle_hash.insert({"GV-TRK-001", "Rajesh Kumar", "Compactor", "Available", "Central"}); vehicle_hash.insert({"GV-TRK-002", "Suresh Mehta", "Tipper", "On Route", "North"});
        vehicle_hash.insert({"GV-TRK-003", "Amit Sharma", "Compactor", "Maintenance", "South"}); vehicle_hash.insert({"GV-RCY-001", "Priya Singh", "Recycling Van", "Available", "East"});
        vehicle_hash.insert({"GV-RCY-002", "Deepak Verma", "Recycling Van", "On Route", "West"});
        saveVehicles(); return;
    }
    int count; if (!(in >> count)) return; in.ignore();
    for (int i = 0; i < count; i++) {
        Vehicle v; getline(in, v.plate); getline(in, v.driver); getline(in, v.type); getline(in, v.status); getline(in, v.zone); vehicle_hash.insert(v);
    }
    in.close();
}
void saveRequests() {
    ofstream out("requests.txt"); if (!out) return; int count = request_queue.size(); out << count << "\n";
    for (int i = 0; i < count; i++) {
        ServiceRequest r = request_queue.dequeue(); out << r.id << "\n" << r.name << "\n" << r.location << "\n" << r.desc << "\n" << r.urgency << "\n";
        request_queue.enqueue(r);
    }
    out.close();
}
void loadRequests() {
    ifstream in("requests.txt");
    if (!in) {
        request_queue.enqueue({1, "Suresh Mehta", "Main Street", "Overflowing bin near bus stop", "High"}); request_queue.enqueue({2, "Anita Desai", "Park Avenue", "Missed collection Thursday", "Medium"});
        request_queue.enqueue({3, "Vikram Joshi", "Industrial Area", "Hazardous waste leakage", "High"}); request_queue.enqueue({4, "Meera Patel", "Residential North", "Request new recycling bin", "Low"});
        request_queue.enqueue({5, "Karan Gupta", "Market District", "Foul odour from bins", "Medium"});
        saveRequests(); return;
    }
    int count; if (!(in >> count)) return; int maxId = 5;
    for (int i = 0; i < count; i++) {
        ServiceRequest r; in >> r.id; in.ignore(); getline(in, r.name); getline(in, r.location); getline(in, r.desc); getline(in, r.urgency);
        request_queue.enqueue(r); if (r.id > maxId) maxId = r.id;
    }
    nextRequestId = maxId + 1; in.close();
}
void loadSampleData() {
    loadBins(); loadVehicles(); loadRequests();
    city_graph = new CityGraph(10);
    city_graph->setName(0, "Central Depot"); city_graph->setName(1, "Main Street"); city_graph->setName(2, "Park Avenue"); city_graph->setName(3, "Industrial Area"); city_graph->setName(4, "Residential North");
    city_graph->setName(5, "Residential South"); city_graph->setName(6, "Market District"); city_graph->setName(7, "Commercial Hub"); city_graph->setName(8, "University Area"); city_graph->setName(9, "Recycling Center");
    city_graph->addEdge(0, 1, 2); city_graph->addEdge(0, 2, 3); city_graph->addEdge(1, 3, 4); city_graph->addEdge(1, 4, 2); city_graph->addEdge(2, 5, 3); city_graph->addEdge(2, 6, 5);
    city_graph->addEdge(3, 7, 1); city_graph->addEdge(4, 7, 3); city_graph->addEdge(5, 8, 2); city_graph->addEdge(6, 9, 4); city_graph->addEdge(7, 8, 6); city_graph->addEdge(8, 9, 3);
    action_stack.push("System initialized");
}

void printBinCard(WasteBin* ptr) {
    if (!ptr) return;
    cout << "  === Waste Bin Details ===\n"
         << "  ID: " << ptr->id << "\n"
         << "  Location: " << ptr->location << "\n"
         << "  Fill Level: " << (int)ptr->wasteLevel << "%\n"
         << "  Capacity: " << (int)ptr->capacity << "L\n"
         << "  Type: " << ptr->wasteType << "\n"
         << "  Current Waste: " << (int)(ptr->wasteLevel * ptr->capacity / 100.0) << "L\n"
         << "  Priority Status: " << ((ptr->wasteLevel >= 80) ? "URGENT" : ((ptr->wasteLevel >= 50) ? "Moderate" : "Normal")) << "\n";
}

void showBinDatabase() {
    cout << "\n  === Waste Bin Records ===\n";
    avl_tree.display();
    cout << "  Total Bins: " << avl_tree.count << " | Primary Storage Depth: " << avl_tree.getHeight() << "\n";
    action_stack.push("Viewed waste bin records"); pressEnterToContinue();
}

void manageBins() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |               Waste Bin Management               |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] View All Bins                               |\n"
         << "  |  [2] Add New Bin                                 |\n"
         << "  |  [3] Search Bin by ID                            |\n"
         << "  |  [4] Search Bin by Location                      |\n"
         << "  |  [5] Remove Bin                                  |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) { showBinDatabase(); return; }
    else if (choice == 2) {
        int id; string loc, type; double lvl, cap;
        cout << "\n  === Add New Bin ===\n";
        cout << "  Enter ID: "; cin >> id; cin.ignore(); cout << "  Enter Location: "; getline(cin, loc);
        cout << "  Enter Fill Level (%): "; cin >> lvl; cout << "  Enter Type (Organic/General/Recyclable/Hazardous): "; cin >> type;
        cout << "  Enter Capacity (L): "; cin >> cap; cin.ignore();
        WasteBin temp = {id, loc, lvl, type, cap}; avl_tree.insert(temp); bst_tree.insert(temp); saveBins();
        cout << "\n  [SUCCESS] New Bin Added Successfully!\n";
        printBinCard(&temp); action_stack.push("Inserted Bin " + to_string(id));
    } else if (choice == 3) {
        int id; cout << "\n  Enter Bin ID to search: "; cin >> id; cin.ignore();
        WasteBin* ptr = avl_tree.search(id);
        if (ptr) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(ptr); }
        else cout << "\n  [ALERT] Bin ID " << id << " was not found.\n";
        action_stack.push("Searched Bin " + to_string(id));
    } else if (choice == 4) {
        string keyword; cout << "\n  Enter location keyword: "; getline(cin, keyword);
        WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
        int idx = linearSearch(tempArr, size, keyword);
        if (idx != -1) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(&tempArr[idx]); }
        else cout << "\n  [ALERT] Location containing '" << keyword << "' not found.\n";
        action_stack.push("Searched for location: " + keyword);
    } else if (choice == 5) {
        int id; cout << "\n  Enter Bin ID to delete: "; cin >> id; cin.ignore();
        if (avl_tree.search(id)) {
            avl_tree.remove(id); bst_tree.remove(id); saveBins();
            cout << "\n  [SUCCESS] Bin " << id << " removed successfully.\n";
            action_stack.push("Deleted Bin " + to_string(id));
        } else cout << "\n  [ALERT] Bin ID " << id << " was not found.\n";
    }
    if (choice != 0) pressEnterToContinue();
}

void collectionSchedule() {
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
    if (size == 0) { cout << "  No bins available.\n"; pressEnterToContinue(); return; }
    heapSort(tempArr, size);
    cout << "\n  === Collection Priority List ===\n";
    vector<int> w = {10, 8, 22, 6, 17}; drawLine(w);
    printRow({"Priority", "Bin ID", "Location", "Fill", "Action Required"}, w); drawLine(w);
    for (int i = 0; i < size; i++) {
        string action = (tempArr[i].wasteLevel >= 80) ? "Collect Now" : ((tempArr[i].wasteLevel >= 50) ? "Schedule Soon" : "Routine Check");
        printRow({to_string(i + 1), to_string(tempArr[i].id), tempArr[i].location, to_string((int)tempArr[i].wasteLevel) + "%", action}, w);
    }
    drawLine(w);
    action_stack.push("Viewed collection priority list"); pressEnterToContinue();
}

void serviceRequests() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |                 Service Requests                 |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] View Pending Requests                       |\n"
         << "  |  [2] Submit New Request                          |\n"
         << "  |  [3] Process Next Request                        |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  === Pending Service Requests ===\n";
        request_queue.display();
    } else if (choice == 2) {
        string name, location, desc, urgency;
        cout << "\n  === Submit Service Request ===\n";
        cout << "  Enter Citizen Name: "; getline(cin, name); cout << "  Enter Location: "; getline(cin, location);
        cout << "  Enter Description: "; getline(cin, desc); cout << "  Enter Urgency (Low/Medium/High): "; getline(cin, urgency);
        request_queue.enqueue({nextRequestId++, name, location, desc, urgency}); saveRequests();
        cout << "\n  [SUCCESS] Request Submitted Successfully!\n";
        action_stack.push("Submitted service request");
    } else if (choice == 3) {
        if (request_queue.isEmpty()) cout << "  No pending requests.\n";
        else {
            ServiceRequest r = request_queue.dequeue(); saveRequests();
            cout << "\n  === Processing Request ===\n"
                 << "  Request ID: #" << r.id << "\n"
                 << "  Citizen: " << r.name << "\n"
                 << "  Location: " << r.location << "\n"
                 << "  Details: " << r.desc << "\n"
                 << "  Urgency: " << r.urgency << "\n"
                 << "  Status: Completed.\n";
            action_stack.push("Processed request #" + to_string(r.id));
        }
    }
    if (choice != 0) pressEnterToContinue();
}

void operationalRecovery() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |                   Recovery Log                   |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] View Log History                            |\n"
         << "  |  [2] Clear Last Log Entry                        |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  === Recovery Log History ===\n";
        action_stack.display();
    } else if (choice == 2) {
        if (action_stack.isEmpty()) cout << "  No past actions to recover.\n";
        else {
            string lastAction = action_stack.pop();
            cout << "\n  [SUCCESS] Cleared Entry: " << lastAction << "\n";
        }
    }
    if (choice != 0) pressEnterToContinue();
}

void vehicleRegistry() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |                Vehicle Management                |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] View Registered Vehicles                    |\n"
         << "  |  [2] Search Vehicle                              |\n"
         << "  |  [3] Add New Vehicle                             |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  === Registered Vehicles ===\n";
        vehicle_hash.displayAll();
    } else if (choice == 2) {
        string p; cout << "  Enter Plate Number: "; cin >> p; cin.ignore();
        Vehicle* v = vehicle_hash.search(p);
        if (v) {
            cout << "\n  === Vehicle Details ===\n"
                 << "  Plate Number: " << v->plate << "\n"
                 << "  Driver: " << v->driver << "\n"
                 << "  Type: " << v->type << "\n"
                 << "  Status: " << v->status << "\n"
                 << "  Zone: " << v->zone << "\n";
        } else cout << "\n  [ALERT] Vehicle " << p << " not found.\n";
        action_stack.push("Searched for vehicle plate: " + p);
    } else if (choice == 3) {
        string pl, dr, ty, st, zn;
        cout << "\n  === Register Vehicle ===\n";
        cout << "  Enter Plate: "; getline(cin, pl); cout << "  Enter Driver: "; getline(cin, dr);
        cout << "  Enter Type: "; getline(cin, ty); cout << "  Enter Status: "; getline(cin, st);
        cout << "  Enter Zone: "; getline(cin, zn);
        vehicle_hash.insert({pl, dr, ty, st, zn}); saveVehicles();
        cout << "\n  [SUCCESS] Vehicle Registered Successfully.\n";
        action_stack.push("Registered vehicle " + pl);
    }
    if (choice != 0) pressEnterToContinue();
}

void routeOptimizer() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |                  Route Planning                  |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] Show City Map                               |\n"
         << "  |  [2] View Reachable Service Areas                |\n"
         << "  |  [3] Check City Network Coverage                 |\n"
         << "  |  [4] Find Shortest Collection Route              |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  === City Connections Map ===\n";
        city_graph->showAdj();
    } else if (choice == 2) {
        int s; cout << "  Enter starting zone number (0-9): "; cin >> s; cin.ignore();
        if (s >= 0 && s < 10) {
            city_graph->bfs(s);
            action_stack.push("Checked connections from " + city_graph->getName(s));
        } else cout << "  Invalid.\n";
    } else if (choice == 3) {
        int s; cout << "  Enter starting zone number (0-9): "; cin >> s; cin.ignore();
        if (s >= 0 && s < 10) {
            city_graph->dfs(s);
            action_stack.push("Checked network connectivity from " + city_graph->getName(s));
        } else cout << "  Invalid.\n";
    } else if (choice == 4) {
        for (int i = 0; i < 10; i++) cout << "    [" << i << "] " << city_graph->getName(i) << "\n";
        int src, dst; cout << "  Enter Source area number: "; cin >> src; cout << "  Enter Destination area number: "; cin >> dst; cin.ignore();
        if (src >= 0 && src < 10 && dst >= 0 && dst < 10) {
            city_graph->dijkstra(src, dst);
            action_stack.push("Planned route from " + city_graph->getName(src) + " to " + city_graph->getName(dst));
        } else cout << "  Invalid area numbers.\n";
    }
    if (choice != 0) pressEnterToContinue();
}

void environmentalReport() {
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
    int urgentCount = 0; double totalVolume = 0, recyclableVolume = 0;
    for (int i = 0; i < size; i++) {
        double volume = tempArr[i].wasteLevel * tempArr[i].capacity / 100.0;
        totalVolume += volume; if (tempArr[i].wasteLevel >= 80) urgentCount++;
        if (tempArr[i].wasteType == "Recyclable") recyclableVolume += volume;
    }
    double recRate = totalVolume > 0 ? (recyclableVolume / totalVolume) * 100.0 : 0.0;
    int rInt = (int)recRate, rDec = (int)((recRate - rInt) * 10), filled = (int)(recRate / 6.66);
    string bar = string(filled, '=') + string(15 - filled, ' ');

    cout << "\n  === Environmental Report ===\n";
    vector<int> w = {38, 15}; drawLine(w); printRow({"Metric", "Value"}, w); drawLine(w);
    printRow({"Total Bins Monitored", to_string(size)}, w);
    printRow({"Bins Needing Collection (>=80%)", to_string(urgentCount)}, w);
    printRow({"Estimated Waste Volume", to_string((int)totalVolume) + " L"}, w);
    printRow({"Recyclable Waste Share", to_string(rInt) + "." + to_string(rDec) + "%"}, w);
    printRow({"Recycling Share Progress", "[" + bar + "]"}, w);
    printRow({"Pending Citizen Issues", to_string(request_queue.size())}, w);
    printRow({"Registered Vehicles", to_string(vehicle_hash.count)}, w); drawLine(w);
    action_stack.push("Viewed Environmental Report"); pressEnterToContinue();
}

void complexityAnalysis() {
    cout << "\n  === System Performance Complexity ===\n";
    vector<int> w = {26, 18, 12}; drawLine(w); printRow({"Feature Area", "Time Complexity", "Extra Space"}, w); drawLine(w);
    printRow({"Bin add/search/remove", "O(log N)", "O(1)"}, w);
    printRow({"Vehicle plate lookup", "O(1) average", "O(1)"}, w);
    printRow({"Recovery log operation", "O(1)", "O(1)"}, w);
    printRow({"Service request handling", "O(1)", "O(1)"}, w);
    printRow({"Collection priority list", "O(N log N)", "O(1)"}, w);
    printRow({"Search by bin ID", "O(log N)", "O(1)"}, w);
    printRow({"Search by location", "O(N)", "O(1)"}, w);
    printRow({"Area coverage check", "O(V + E)", "O(V)"}, w);
    printRow({"Shortest route planning", "O(V + E log V)", "O(V)"}, w); drawLine(w);
    cout << "  Scale Info: N = bins count, V = zones (10), E = connections (12)\n";
    action_stack.push("Viewed system performance summary"); pressEnterToContinue();
}

void systemPerformance() {
    cout << "\n  +--------------------------------------------------+\n"
         << "  |                System Performance                |\n"
         << "  +--------------------------------------------------+\n"
         << "  |  [1] Primary Storage Status                      |\n"
         << "  |  [2] Backup Storage Status                       |\n"
         << "  |  [3] Storage Comparison                          |\n"
         << "  |  [4] Feature Performance Summary                 |\n"
         << "  |  [0] Back                                        |\n"
         << "  +--------------------------------------------------+\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  === Primary Storage Status (AVL Tree) ===\n"; avl_tree.display();
        string pStr = ""; avl_tree.collectPreorder(avl_tree.root, pStr); cout << "\n  Preorder Traversal: " << pStr << "\n\n  === AVL Node Connections Table ===\n";
        vector<int> w = {10, 15, 15}; drawLine(w); printRow({"Node ID", "Left Child", "Right Child"}, w); drawLine(w); avl_tree.printTreeTable(avl_tree.root, w); drawLine(w);
    } else if (choice == 2) {
        cout << "\n  === Backup Storage Status (BST Tree) ===\n"; bst_tree.display();
        string pStr = ""; bst_tree.collectPreorder(bst_tree.root, pStr); cout << "\n  Preorder Traversal: " << pStr << "\n\n  === BST Node Connections Table ===\n";
        vector<int> w = {10, 15, 15}; drawLine(w); printRow({"Node ID", "Left Child", "Right Child"}, w); drawLine(w); bst_tree.printTreeTable(bst_tree.root, w); drawLine(w);
    } else if (choice == 3) {
        cout << "\n  === Storage Comparison ===\n";
        vector<int> w = {25, 15}; drawLine(w); printRow({"Storage Structure", "Tree Height"}, w); drawLine(w);
        printRow({"AVL Tree (Primary)", to_string(avl_tree.getHeight())}, w); printRow({"BST Tree (Backup)", to_string(bst_tree.getHeight())}, w); drawLine(w);
    } else if (choice == 4) { complexityAnalysis(); return; }
    if (choice != 0) pressEnterToContinue();
}

int main() {
    loadSampleData();
    while (true) {
        WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
        int urgentCount = 0; for (int i = 0; i < size; i++) { if (tempArr[i].wasteLevel >= 80) urgentCount++; }
        string recommendedAction;
        if (urgentCount > 0) recommendedAction = "Review Collection Planning";
        else if (request_queue.size() > 0) recommendedAction = "Process Citizen Requests";
        else recommendedAction = "System operating normally";

        cout << "\n  +--------------------------------------------------------+\n"
             << "  |  Smart Waste Collection & Recycling Management System  |\n"
             << "  +--------------------------------------------------------+\n"
             << "  |  System Status                                         |\n"
             << "  |    Total Bins          : " << pad(to_string(size), 29) << " |\n"
             << "  |    Urgent Bins         : " << pad(to_string(urgentCount) + " need collection", 29) << " |\n"
             << "  |    Pending Requests    : " << pad(to_string(request_queue.size()) + " citizen issues", 29) << " |\n"
             << "  |    Registered Vehicles : " << pad(to_string(vehicle_hash.count), 29) << " |\n"
             << "  |    Recommended Action  : " << pad(recommendedAction, 29) << " |\n"
             << "  +--------------------------------------------------------+\n"
             << "  |  [1] Waste Bin Management                              |\n"
             << "  |  [2] Collection Planning                               |\n"
             << "  |  [3] Citizen Service Requests                          |\n"
             << "  |  [4] Vehicle Management                                |\n"
             << "  |  [5] Route Planning                                    |\n"
             << "  |  [6] Environmental Report                              |\n"
             << "  |  [7] System Performance                                |\n"
             << "  |  [8] Recovery Log                                      |\n"
             << "  |  [0] Exit                                              |\n"
             << "  +--------------------------------------------------------+\n"
             << "  Enter Choice: ";
        int choice = readChoice();
        if (choice == 1) manageBins();
        else if (choice == 2) collectionSchedule();
        else if (choice == 3) serviceRequests();
        else if (choice == 4) vehicleRegistry();
        else if (choice == 5) routeOptimizer();
        else if (choice == 6) environmentalReport();
        else if (choice == 7) systemPerformance();
        else if (choice == 8) operationalRecovery();
        else if (choice == 0) { cout << "\n  Thank you for using the Smart Waste Management System!\n"; delete city_graph; break; }
        else cout << "  Invalid choice. Try again.\n";
    }
    return 0;
}
