
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

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
    int val;
    if (!(cin >> val)) cin.clear();
    cin.ignore(10000, '\n');
    return val;
}

void pressEnterToContinue() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}

void printBinHeader() {
    cout << "  ID    Location               Fill%  Capacity Type         Status\n";
    cout << "  ----------------------------------------------------------------\n";
}

void printBinRow(WasteBin b) {
    string status = (b.wasteLevel >= 80) ? "URGENT" : ((b.wasteLevel >= 50) ? "Moderate" : "Normal");
    string loc = b.location;
    if (loc.length() < 22) loc.append(22 - loc.length(), ' ');
    else loc = loc.substr(0, 22);

    string lvlStr = to_string((int)b.wasteLevel) + "%";
    if (lvlStr.length() < 5) lvlStr.insert(0, 5 - lvlStr.length(), ' ');

    string capStr = to_string((int)b.capacity) + "L";
    if (capStr.length() < 6) capStr.append(6 - capStr.length(), ' ');

    string type = b.wasteType;
    if (type.length() < 12) type.append(12 - type.length(), ' ');
    else type = type.substr(0, 12);

    cout << "  " << b.id << "   " << loc << " " << lvlStr << "  " << capStr << "   " << type << " " << status << "\n";
}

// 1. Stack for operational recovery (Linked List)
struct StackNode { string data; StackNode* next; };
class Stack {
public:
    StackNode* top; int count;
    Stack() { top = NULL; count = 0; }
    void push(string val) { top = new StackNode{val, top}; count++; }
    string pop() {
        if (!top) return "";
        StackNode* temp = top; string val = temp->data;
        top = top->next; delete temp; count--;
        return val;
    }
    bool isEmpty() { return top == NULL; }
    int size() { return count; }
    void display() {
        if (!top) { cout << "    (Log is empty)\n"; return; }
        StackNode* temp = top; int i = 1;
        while (temp) { cout << "    [" << i++ << "] " << temp->data << "\n"; temp = temp->next; }
    }
};

// 2. Queue for Service Requests (Linked List)
struct QueueNode { ServiceRequest data; QueueNode* next; };
class Queue {
public:
    QueueNode *frontPtr, *rearPtr; int count;
    Queue() { frontPtr = rearPtr = NULL; count = 0; }
    void enqueue(ServiceRequest req) {
        QueueNode* temp = new QueueNode{req, NULL};
        if (!rearPtr) frontPtr = rearPtr = temp;
        else { rearPtr->next = temp; rearPtr = temp; }
        count++;
    }
    ServiceRequest dequeue() {
        if (!frontPtr) return {0, "", "", "", ""};
        QueueNode* temp = frontPtr; ServiceRequest req = temp->data;
        frontPtr = frontPtr->next; if (!frontPtr) rearPtr = NULL;
        delete temp; count--;
        return req;
    }
    bool isEmpty() { return frontPtr == NULL; }
    int size() { return count; }
    void display() {
        if (!frontPtr) { cout << "    (No pending service requests in queue)\n"; return; }
        QueueNode* temp = frontPtr; int idx = 1;
        while (temp) {
            cout << "  ---------------------------------------------------\n"
                 << "  [" << idx++ << "] Request ID: #" << temp->data.id << " | Urgency: " << temp->data.urgency << "\n"
                 << "      Citizen  : " << temp->data.name << "\n"
                 << "      Location : " << temp->data.location << "\n"
                 << "      Details  : " << temp->data.desc << "\n";
            temp = temp->next;
        }
        cout << "  ---------------------------------------------------\n";
    }
};

// 3 & 4. BST and AVL Trees
struct TreeNode { WasteBin bin; TreeNode *left, *right; int height; };

class BST {
public:
    TreeNode* root; int count;
    BST() { root = NULL; count = 0; }
    TreeNode* insertHelper(TreeNode* node, WasteBin b) {
        if (!node) { count++; return new TreeNode{b, NULL, NULL, 1}; }
        if (b.id < node->bin.id) node->left = insertHelper(node->left, b);
        else if (b.id > node->bin.id) node->right = insertHelper(node->right, b);
        return node;
    }
    void insert(WasteBin b) { root = insertHelper(root, b); }
    TreeNode* searchHelper(TreeNode* node, int id) {
        if (!node || node->bin.id == id) return node;
        return (id < node->bin.id) ? searchHelper(node->left, id) : searchHelper(node->right, id);
    }
    WasteBin* search(int id) { TreeNode* t = searchHelper(root, id); return t ? &(t->bin) : NULL; }
    TreeNode* findMin(TreeNode* node) { while (node && node->left) node = node->left; return node; }
    TreeNode* deleteHelper(TreeNode* node, int id) {
        if (!node) return NULL;
        if (id < node->bin.id) node->left = deleteHelper(node->left, id);
        else if (id > node->bin.id) node->right = deleteHelper(node->right, id);
        else {
            if (!node->left || !node->right) {
                TreeNode* temp = node->left ? node->left : node->right;
                delete node; count--; return temp;
            }
            TreeNode* temp = findMin(node->right);
            node->bin = temp->bin; node->right = deleteHelper(node->right, temp->bin.id);
        }
        return node;
    }
    void remove(int id) { root = deleteHelper(root, id); }
    int getHeightHelper(TreeNode* node) {
        if (!node) return 0;
        int lh = getHeightHelper(node->left), rh = getHeightHelper(node->right);
        return 1 + (lh > rh ? lh : rh);
    }
    int getHeight() { return getHeightHelper(root); }
    void inorder(TreeNode* node) { if (node) { inorder(node->left); printBinRow(node->bin); inorder(node->right); } }
    void display() { if (root) { printBinHeader(); inorder(root); } else cout << "  (empty)\n"; }
    void preorder(TreeNode* node) { if (node) { cout << node->bin.id << " "; preorder(node->left); preorder(node->right); } }
    void printBSTStructure(TreeNode* node) {
        if (!node) return;
        cout << "    Bin " << node->bin.id;
        if (node->left) cout << " | Lower ID Link: " << node->left->bin.id;
        if (node->right) cout << " | Higher ID Link: " << node->right->bin.id;
        cout << "\n";
        printBSTStructure(node->left); printBSTStructure(node->right);
    }
};

class AVL {
public:
    TreeNode* root; int count;
    AVL() { root = NULL; count = 0; }
    int getHeight(TreeNode* n) { return n ? n->height : 0; }
    int getHeight() { return getHeight(root); }
    int getBalanceFactor(TreeNode* n) { return n ? getHeight(n->left) - getHeight(n->right) : 0; }
    void updateHeight(TreeNode* n) {
        if (n) {
            int lh = getHeight(n->left), rh = getHeight(n->right);
            n->height = 1 + (lh > rh ? lh : rh);
        }
    }
    TreeNode* rotateRight(TreeNode* y) {
        TreeNode* x = y->left; TreeNode* T2 = x->right;
        x->right = y; y->left = T2; updateHeight(y); updateHeight(x);
        return x;
    }
    TreeNode* rotateLeft(TreeNode* x) {
        TreeNode* y = x->right; TreeNode* T2 = y->left;
        y->left = x; x->right = T2; updateHeight(x); updateHeight(y);
        return y;
    }
    TreeNode* balance(TreeNode* n) {
        updateHeight(n); int bf = getBalanceFactor(n);
        if (bf > 1) {
            if (getBalanceFactor(n->left) >= 0) return rotateRight(n);
            n->left = rotateLeft(n->left); return rotateRight(n);
        }
        if (bf < -1) {
            if (getBalanceFactor(n->right) <= 0) return rotateLeft(n);
            n->right = rotateRight(n->right); return rotateLeft(n);
        }
        return n;
    }
    TreeNode* insertHelper(TreeNode* node, WasteBin b) {
        if (!node) { count++; return new TreeNode{b, NULL, NULL, 1}; }
        if (b.id < node->bin.id) node->left = insertHelper(node->left, b);
        else if (b.id > node->bin.id) node->right = insertHelper(node->right, b);
        else return node;
        return balance(node);
    }
    void insert(WasteBin b) { root = insertHelper(root, b); }
    TreeNode* findMin(TreeNode* node) { while (node && node->left) node = node->left; return node; }
    TreeNode* deleteHelper(TreeNode* node, int id) {
        if (!node) return NULL;
        if (id < node->bin.id) node->left = deleteHelper(node->left, id);
        else if (id > node->bin.id) node->right = deleteHelper(node->right, id);
        else {
            if (!node->left || !node->right) {
                TreeNode* temp = node->left ? node->left : node->right;
                delete node; count--; return temp;
            }
            TreeNode* temp = findMin(node->right);
            node->bin = temp->bin; node->right = deleteHelper(node->right, temp->bin.id);
        }
        if (!node) return NULL;
        return balance(node);
    }
    void remove(int id) { root = deleteHelper(root, id); }
    TreeNode* searchHelper(TreeNode* node, int id) {
        if (!node || node->bin.id == id) return node;
        return (id < node->bin.id) ? searchHelper(node->left, id) : searchHelper(node->right, id);
    }
    WasteBin* search(int id) { TreeNode* t = searchHelper(root, id); return t ? &(t->bin) : NULL; }
    void inorder(TreeNode* node) { if (node) { inorder(node->left); printBinRow(node->bin); inorder(node->right); } }
    void display() { if (root) { printBinHeader(); inorder(root); } else cout << "  (empty)\n"; }
    void collectHelper(TreeNode* node, WasteBin arr[], int& i) {
        if (!node) return;
        collectHelper(node->left, arr, i); arr[i++] = node->bin; collectHelper(node->right, arr, i);
    }
    void getAll(WasteBin arr[], int& c) { c = 0; collectHelper(root, arr, c); }
    void preorder(TreeNode* node) { if (node) { cout << node->bin.id << " "; preorder(node->left); preorder(node->right); } }
    void printAVLStructure(TreeNode* node) {
        if (!node) return;
        cout << "    Bin " << node->bin.id << " (Storage Level: " << node->height << ")";
        if (node->left) cout << " | Lower ID Link: " << node->left->bin.id;
        if (node->right) cout << " | Higher ID Link: " << node->right->bin.id;
        cout << "\n";
        printAVLStructure(node->left); printAVLStructure(node->right);
    }
};

// 5. Hash Table for Vehicles (Separate Chaining)
struct HashNode { Vehicle data; HashNode* next; };
class VehicleHash {
public:
    static const int size = 53; HashNode* table[size]; int count;
    VehicleHash() { count = 0; for (int i = 0; i < size; i++) table[i] = NULL; }
    int getHashValue(string key) {
        unsigned long h = 0;
        for (int i = 0; i < key.length(); i++) h = (h * 31 + key[i]) % size;
        return (int)h;
    }
    void insert(Vehicle v) { int index = getHashValue(v.plate); table[index] = new HashNode{v, table[index]}; count++; }
    Vehicle* search(string plate) {
        HashNode* current = table[getHashValue(plate)];
        while (current) { if (current->data.plate == plate) return &(current->data); current = current->next; }
        return NULL;
    }
    void displayAll() {
        cout << "  Plate         Driver             Type               Status         Zone\n";
        cout << "  ----------------------------------------------------------------------------\n";
        for (int i = 0; i < size; i++) {
            HashNode* current = table[i];
            while (current) {
                string pl = current->data.plate; if (pl.length() < 13) pl.append(13 - pl.length(), ' ');
                string dr = current->data.driver; if (dr.length() < 18) dr.append(18 - dr.length(), ' ');
                string ty = current->data.type; if (ty.length() < 18) ty.append(18 - ty.length(), ' ');
                string st = current->data.status; if (st.length() < 14) st.append(14 - st.length(), ' ');
                cout << "  " << pl << " " << dr << " " << ty << " " << st << " " << current->data.zone << "\n";
                current = current->next;
            }
        }
    }
};

// 6. Graph for City Networks
struct Edge { int dest, weight; Edge* next; };
class CityGraph {
public:
    static const int maxVertices = 10; int V; string names[maxVertices]; Edge* adjList[maxVertices];
    CityGraph(int num) { V = num; for (int i = 0; i < maxVertices; i++) adjList[i] = NULL; }
    void setName(int idx, string name) { names[idx] = name; }
    string getName(int idx) { return names[idx]; }
    void addEdge(int u, int v, int w) {
        adjList[u] = new Edge{v, w, adjList[u]}; adjList[v] = new Edge{u, w, adjList[v]};
    }
    void showAdj() {
        for (int i = 0; i < V; i++) {
            cout << "  [" << i << "] " << names[i] << " -> ";
            Edge* temp = adjList[i];
            while (temp) { cout << names[temp->dest] << "(" << temp->weight << "km) "; temp = temp->next; }
            cout << "\n";
        }
    }
    void bfs(int start) {
        bool visited[maxVertices] = {false}; int queueArr[maxVertices], front = 0, rear = 0;
        visited[start] = true; queueArr[rear++] = start;
        cout << "  Reachable Service Areas:\n    ";
        while (front < rear) {
            int current = queueArr[front++]; cout << "[" << names[current] << "]";
            if (front < rear) cout << " -> ";
            Edge* temp = adjList[current];
            while (temp) {
                if (!visited[temp->dest]) { visited[temp->dest] = true; queueArr[rear++] = temp->dest; }
                temp = temp->next;
            }
        }
        cout << "\n";
    }
    void dfsHelper(int vertex, bool visited[], int& visitedCount) {
        visited[vertex] = true; visitedCount++; cout << "[" << names[vertex] << "]";
        Edge* temp = adjList[vertex];
        while (temp) {
            if (!visited[temp->dest]) { cout << " -> "; dfsHelper(temp->dest, visited, visitedCount); }
            temp = temp->next;
        }
    }
    void dfs(int start) {
        bool visited[maxVertices] = {false}; int visitedCount = 0;
        cout << "  City Coverage Path:\n    "; dfsHelper(start, visited, visitedCount); cout << "\n\n";
        if (visitedCount == V) cout << "  Status: All " << V << " areas are connected.\n";
        else cout << "  Status: Only " << visitedCount << "/" << V << " areas are connected (Grid is disconnected).\n";
    }
    void dijkstra(int src, int dst) {
        int dist[maxVertices], prev[maxVertices]; bool visited[maxVertices] = {false};
        for (int i = 0; i < V; i++) { dist[i] = 999999; prev[i] = -1; }
        dist[src] = 0;
        for (int i = 0; i < V - 1; i++) {
            int minVal = 999999, u = -1;
            for (int v = 0; v < V; v++) { if (!visited[v] && dist[v] < minVal) { minVal = dist[v]; u = v; } }
            if (u == -1) break;
            visited[u] = true;
            Edge* temp = adjList[u];
            while (temp) {
                if (!visited[temp->dest] && dist[u] + temp->weight < dist[temp->dest]) {
                    dist[temp->dest] = dist[u] + temp->weight; prev[temp->dest] = u;
                }
                temp = temp->next;
            }
        }
        if (dist[dst] == 999999) { cout << "  No path exists.\n"; return; }
        int path[maxVertices], len = 0, current = dst;
        while (current != -1) { path[len++] = current; current = prev[current]; }
        cout << "  Route Plan:\n    ";
        for (int i = len - 1; i >= 0; i--) {
            cout << "[" << names[path[i]] << "]";
            if (i > 0) cout << " ===(" << (dist[path[i - 1]] - dist[path[i]]) << "km)===> ";
        }
        cout << "\n\n  Total Distance: " << dist[dst] << " km\n\n  Distances from [" << names[src] << "]:\n";
        cout << "  ---------------------------------------------------\n";
        for (int i = 0; i < V; i++) {
            string dStr = (dist[i] == 999999) ? "INF" : to_string(dist[i]) + " km";
            string spaceName = names[i]; while (spaceName.length() < 22) spaceName += " ";
            cout << "    " << spaceName << ": " << dStr << "\n";
        }
        cout << "  ---------------------------------------------------\n";
    }
};

// 7. Sorting: Heap Sort and Quick Sort
void heapify(WasteBin arr[], int n, int i) {
    int largest = i, left = 2 * i + 1, right = 2 * i + 2;
    if (left < n && getPriorityScore(arr[left]) > getPriorityScore(arr[largest])) largest = left;
    if (right < n && getPriorityScore(arr[right]) > getPriorityScore(arr[largest])) largest = right;
    if (largest != i) { swap(arr[i], arr[largest]); heapify(arr, n, largest); }
}
void heapSort(WasteBin arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) { swap(arr[0], arr[i]); heapify(arr, i, 0); }
    for (int i = 0; i < n / 2; i++) swap(arr[i], arr[n - 1 - i]);
}
int partition(WasteBin arr[], int low, int high) {
    double pivot = getPriorityScore(arr[high]); int i = low - 1;
    for (int j = low; j < high; j++) { if (getPriorityScore(arr[j]) >= pivot) swap(arr[++i], arr[j]); }
    swap(arr[i + 1], arr[high]); return i + 1;
}
void quickSort(WasteBin arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high); quickSort(arr, low, pi - 1); quickSort(arr, pi + 1, high);
    }
}

// 8. Searching: Binary Search and Linear Search
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
    string result = "";
    for (int i = 0; i < str.length(); i++) {
        char ch = str[i]; result += (ch >= 'A' && ch <= 'Z') ? (ch + 32) : ch;
    }
    return result;
}
int linearSearch(WasteBin arr[], int n, string locKeyword) {
    string target = toLowerCase(locKeyword);
    for (int i = 0; i < n; i++) {
        if (toLowerCase(arr[i].location).find(target) != string::npos) return i;
    }
    return -1;
}

// Global variables
AVL avl_tree; BST bst_tree; Queue request_queue; Stack action_stack; VehicleHash vehicle_hash;
CityGraph* city_graph = NULL; int nextRequestId = 6;

void saveBins() {
    ofstream out("bins.txt");
    if (!out) return;
    WasteBin tempArr[100]; int size = 0;
    avl_tree.getAll(tempArr, size);
    out << size << "\n";
    for (int i = 0; i < size; i++) {
        out << tempArr[i].id << "\n";
        out << tempArr[i].location << "\n";
        out << tempArr[i].wasteLevel << "\n";
        out << tempArr[i].wasteType << "\n";
        out << tempArr[i].capacity << "\n";
    }
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
        saveBins();
        return;
    }
    int size;
    if (!(in >> size)) return;
    for (int i = 0; i < size; i++) {
        WasteBin b;
        in >> b.id;
        in.ignore();
        getline(in, b.location);
        in >> b.wasteLevel;
        in.ignore();
        getline(in, b.wasteType);
        in >> b.capacity;
        avl_tree.insert(b);
        bst_tree.insert(b);
    }
    in.close();
}

void saveVehicles() {
    ofstream out("vehicles.txt");
    if (!out) return;
    out << vehicle_hash.count << "\n";
    for (int i = 0; i < VehicleHash::size; i++) {
        HashNode* current = vehicle_hash.table[i];
        while (current) {
            out << current->data.plate << "\n";
            out << current->data.driver << "\n";
            out << current->data.type << "\n";
            out << current->data.status << "\n";
            out << current->data.zone << "\n";
            current = current->next;
        }
    }
    out.close();
}

void loadVehicles() {
    ifstream in("vehicles.txt");
    if (!in) {
        vehicle_hash.insert({"GV-TRK-001", "Rajesh Kumar", "Compactor", "Available", "Central"});
        vehicle_hash.insert({"GV-TRK-002", "Suresh Mehta", "Tipper", "On Route", "North"});
        vehicle_hash.insert({"GV-TRK-003", "Amit Sharma", "Compactor", "Maintenance", "South"});
        vehicle_hash.insert({"GV-RCY-001", "Priya Singh", "Recycling Van", "Available", "East"});
        vehicle_hash.insert({"GV-RCY-002", "Deepak Verma", "Recycling Van", "On Route", "West"});
        saveVehicles();
        return;
    }
    int count;
    if (!(in >> count)) return;
    in.ignore();
    for (int i = 0; i < count; i++) {
        Vehicle v;
        getline(in, v.plate);
        getline(in, v.driver);
        getline(in, v.type);
        getline(in, v.status);
        getline(in, v.zone);
        vehicle_hash.insert(v);
    }
    in.close();
}

void saveRequests() {
    ofstream out("requests.txt");
    if (!out) return;
    int count = request_queue.size();
    out << count << "\n";
    for (int i = 0; i < count; i++) {
        ServiceRequest r = request_queue.dequeue();
        out << r.id << "\n";
        out << r.name << "\n";
        out << r.location << "\n";
        out << r.desc << "\n";
        out << r.urgency << "\n";
        request_queue.enqueue(r);
    }
    out.close();
}

void loadRequests() {
    ifstream in("requests.txt");
    if (!in) {
        request_queue.enqueue({1, "Suresh Mehta", "Main Street", "Overflowing bin near bus stop", "High"});
        request_queue.enqueue({2, "Anita Desai", "Park Avenue", "Missed collection Thursday", "Medium"});
        request_queue.enqueue({3, "Vikram Joshi", "Industrial Area", "Hazardous waste leakage", "High"});
        request_queue.enqueue({4, "Meera Patel", "Residential North", "Request new recycling bin", "Low"});
        request_queue.enqueue({5, "Karan Gupta", "Market District", "Foul odour from bins", "Medium"});
        saveRequests();
        return;
    }
    int count;
    if (!(in >> count)) return;
    int maxId = 5;
    for (int i = 0; i < count; i++) {
        ServiceRequest r;
        in >> r.id;
        in.ignore();
        getline(in, r.name);
        getline(in, r.location);
        getline(in, r.desc);
        getline(in, r.urgency);
        request_queue.enqueue(r);
        if (r.id > maxId) maxId = r.id;
    }
    nextRequestId = maxId + 1;
    in.close();
}

void loadSampleData() {
    loadBins();
    loadVehicles();
    loadRequests();
    city_graph = new CityGraph(10);
    city_graph->setName(0, "Central Depot"); city_graph->setName(1, "Main Street");
    city_graph->setName(2, "Park Avenue"); city_graph->setName(3, "Industrial Area");
    city_graph->setName(4, "Residential North"); city_graph->setName(5, "Residential South");
    city_graph->setName(6, "Market District"); city_graph->setName(7, "Commercial Hub");
    city_graph->setName(8, "University Area"); city_graph->setName(9, "Recycling Center");
    city_graph->addEdge(0, 1, 2); city_graph->addEdge(0, 2, 3); city_graph->addEdge(1, 3, 4);
    city_graph->addEdge(1, 4, 2); city_graph->addEdge(2, 5, 3); city_graph->addEdge(2, 6, 5);
    city_graph->addEdge(3, 7, 1); city_graph->addEdge(4, 7, 3); city_graph->addEdge(5, 8, 2);
    city_graph->addEdge(6, 9, 4); city_graph->addEdge(7, 8, 6); city_graph->addEdge(8, 9, 3);
    action_stack.push("System initialized");
}

void printBinCard(WasteBin* ptr) {
    if (!ptr) return;
    string pStr = (ptr->wasteLevel >= 80) ? "High Alert" : ((ptr->wasteLevel >= 50) ? "Medium Alert" : "Regular");
    double currentVolume = ptr->wasteLevel * ptr->capacity / 100.0;
    cout << "  ---------------------------------------------------\n"
         << "   Waste Bin Details\n"
         << "  ---------------------------------------------------\n"
         << "    Bin ID         : " << ptr->id << "\n"
         << "    Location       : " << ptr->location << "\n"
         << "    Fill Level     : " << ptr->wasteLevel << "%\n"
         << "    Capacity       : " << ptr->capacity << " L\n"
         << "    Waste Type     : " << ptr->wasteType << "\n"
         << "    Current Waste  : " << currentVolume << " L\n"
         << "    Priority       : " << pStr << "\n"
         << "  ---------------------------------------------------\n";
}

void showBinDatabase() {
    cout << "\n--------------------------------------------------\n"
         << "## Waste Bin Records\n"
         << "--------------------------------------------------\n";
    avl_tree.display();
    cout << "\n  Summary:\n"
         << "    Total Bins           : " << avl_tree.count << "\n"
         << "    Storage Depth        : " << avl_tree.getHeight() << "\n";
    action_stack.push("Viewed waste bin records"); pressEnterToContinue();
}

void manageBins() {
    cout << "\n--------------------------------------------------\n"
         << "## Waste Bin Management\n"
         << "--------------------------------------------------\n"
         << "  [1] View All Bins\n  [2] Add New Bin\n  [3] Search Bin by ID\n  [4] Search Bin by Location\n  [5] Remove Bin\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        showBinDatabase();
        return;
    } else if (choice == 2) {
        int id; string loc, type; double lvl, cap;
        cout << "  Enter ID: "; cin >> id; cin.ignore();
        cout << "  Enter Location: "; getline(cin, loc);
        cout << "  Enter Fill Level (%): "; cin >> lvl;
        cout << "  Enter Type (Organic/General/Recyclable/Hazardous): "; cin >> type;
        cout << "  Enter Capacity (L): "; cin >> cap; cin.ignore();
        WasteBin temp = {id, loc, lvl, type, cap};
        avl_tree.insert(temp); bst_tree.insert(temp);
        saveBins();
        cout << "\n  ---------------------------------------------------\n"
             << "  [SUCCESS] New Bin Added Successfully!\n";
        printBinCard(&temp);
        action_stack.push("Inserted Bin " + to_string(id));
    } else if (choice == 3) {
        int id; cout << "  Enter Bin ID to search: "; cin >> id; cin.ignore();
        WasteBin* ptr = avl_tree.search(id);
        if (ptr) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(ptr); }
        else cout << "\n  [ALERT] Bin ID: " << id << " not found.\n";
        action_stack.push("Searched Bin " + to_string(id));
    } else if (choice == 4) {
        string keyword; cout << "  Enter location keyword: "; getline(cin, keyword);
        WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
        int idx = linearSearch(tempArr, size, keyword);
        if (idx != -1) { cout << "\n  [RESULT] Matching Bin Found:\n"; printBinCard(&tempArr[idx]); }
        else cout << "\n  [ALERT] No bin found for location keyword '" << keyword << "'.\n";
        action_stack.push("Searched for location: " + keyword);
    } else if (choice == 5) {
        int id; cout << "  Enter Bin ID to delete: "; cin >> id; cin.ignore();
        if (avl_tree.search(id)) {
            avl_tree.remove(id); bst_tree.remove(id);
            saveBins();
            cout << "\n  ---------------------------------------------------\n"
                 << "  [SUCCESS] Bin " << id << " removed successfully.\n"
                 << "  ---------------------------------------------------\n";
            action_stack.push("Deleted Bin " + to_string(id));
        } else cout << "\n  [ALERT] Bin ID: " << id << " not found.\n";
    }
    if (choice != 0) pressEnterToContinue();
}

void collectionSchedule() {
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
    if (size == 0) {
        cout << "\n--------------------------------------------------\n"
             << "## Collection Schedule\n"
             << "--------------------------------------------------\n";
        cout << "  No bins available.\n"; pressEnterToContinue(); return;
    }
    heapSort(tempArr, size);

    cout << "\n--------------------------------------------------\n"
         << "## Collection Priority List\n"
         << "--------------------------------------------------\n"
         << "  Priority  Bin ID  Location               Fill   Action Required\n"
         << "  ---------------------------------------------------------------\n";
    for (int i = 0; i < size; i++) {
        string action = (tempArr[i].wasteLevel >= 80) ? "Collect Now" : ((tempArr[i].wasteLevel >= 50) ? "Schedule Soon" : "Routine Check");
        string loc = tempArr[i].location;
        if (loc.length() < 22) loc.append(22 - loc.length(), ' '); else loc = loc.substr(0, 22);
        string lvlStr = to_string((int)tempArr[i].wasteLevel) + "%";
        if (lvlStr.length() < 5) lvlStr.insert(0, 5 - lvlStr.length(), ' ');
        cout << "  " << i + 1 << "         " << tempArr[i].id << "     " << loc << " " << lvlStr << "  " << action << "\n";
    }
    cout << "  ---------------------------------------------------------------\n";
    action_stack.push("Viewed collection priority list"); pressEnterToContinue();
}

void serviceRequests() {
    cout << "\n--------------------------------------------------\n"
         << "## Service Requests\n"
         << "--------------------------------------------------\n"
         << "  [1] View Pending Requests\n  [2] Submit New Request\n  [3] Process Next Request\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  --- Pending Service Requests ---\n";
        request_queue.display();
    } else if (choice == 2) {
        string name, location, desc, urgency;
        cout << "  Enter Citizen Name: "; getline(cin, name);
        cout << "  Enter Location: "; getline(cin, location);
        cout << "  Enter Description: "; getline(cin, desc);
        cout << "  Enter Urgency (Low/Medium/High): "; getline(cin, urgency);
        request_queue.enqueue({nextRequestId++, name, location, desc, urgency});
        saveRequests();
        cout << "\n  ---------------------------------------------------\n"
             << "  [SUCCESS] Request Submitted Successfully!\n"
             << "  ---------------------------------------------------\n";
        action_stack.push("Submitted service request");
    } else if (choice == 3) {
        if (request_queue.isEmpty()) cout << "  No pending requests.\n";
        else {
            ServiceRequest r = request_queue.dequeue();
            saveRequests();
            cout << "\n  ---------------------------------------------------\n"
                 << "  [PROCESSING] Processing Next Request:\n"
                 << "  ---------------------------------------------------\n"
                 << "    Request ID   : #" << r.id << "\n"
                 << "    Citizen      : " << r.name << "\n"
                 << "    Location     : " << r.location << "\n"
                 << "    Details      : " << r.desc << "\n"
                 << "    Urgency      : " << r.urgency << "\n"
                 << "  ---------------------------------------------------\n"
                 << "  Status: Completed. Pending requests remaining: " << request_queue.size() << "\n";
            action_stack.push("Processed request #" + to_string(r.id));
        }
    }
    if (choice != 0) pressEnterToContinue();
}

void operationalRecovery() {
    cout << "\n--------------------------------------------------\n"
         << "## Recovery Log\n"
         << "--------------------------------------------------\n"
         << "  [1] View Log History\n  [2] Clear Last Log Entry\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  --- Recovery Log ---\n";
        action_stack.display();
    } else if (choice == 2) {
        if (action_stack.isEmpty()) cout << "  No past actions to recover.\n";
        else {
            string lastAction = action_stack.pop();
            cout << "\n  ---------------------------------------------------\n"
                 << "  [SUCCESS] Last log entry cleared:\n"
                 << "    Cleared Entry: " << lastAction << "\n"
                 << "  ---------------------------------------------------\n";
        }
    }
    if (choice != 0) pressEnterToContinue();
}

void vehicleRegistry() {
    cout << "\n--------------------------------------------------\n"
         << "## Vehicle Management\n"
         << "--------------------------------------------------\n"
         << "  [1] View Registered Vehicles\n  [2] Search Vehicle\n  [3] Add New Vehicle\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) vehicle_hash.displayAll();
    else if (choice == 2) {
        string p; cout << "  Enter Plate Number: "; cin >> p; cin.ignore();
        Vehicle* v = vehicle_hash.search(p);
        if (v) {
            cout << "\n  ---------------------------------------------------\n"
                 << "   Vehicle Details\n"
                 << "  ---------------------------------------------------\n"
                 << "    Plate Number   : " << v->plate << "\n"
                 << "    Driver         : " << v->driver << "\n"
                 << "    Type           : " << v->type << "\n"
                 << "    Status         : " << v->status << "\n"
                 << "    Zone           : " << v->zone << "\n"
                 << "  ---------------------------------------------------\n";
        } else cout << "\n  [ALERT] Vehicle " << p << " not found.\n";
        action_stack.push("Searched for vehicle plate: " + p);
    } else if (choice == 3) {
        string pl, dr, ty, st, zn;
        cout << "  Enter Plate: "; getline(cin, pl); cout << "  Enter Driver: "; getline(cin, dr);
        cout << "  Enter Type: "; getline(cin, ty); cout << "  Enter Status: "; getline(cin, st);
        cout << "  Enter Zone: "; getline(cin, zn);
        vehicle_hash.insert({pl, dr, ty, st, zn});
        saveVehicles();
        cout << "\n  ---------------------------------------------------\n"
             << "  [SUCCESS] Vehicle Registered Successfully.\n"
             << "  ---------------------------------------------------\n";
        action_stack.push("Registered vehicle " + pl);
    }
    if (choice != 0) pressEnterToContinue();
}

void routeOptimizer() {
    cout << "\n--------------------------------------------------\n"
         << "## Route Planning\n"
         << "--------------------------------------------------\n"
         << "  [1] Show City Map\n  [2] View Reachable Service Areas\n  [3] Check City Network Coverage\n  [4] Find Shortest Collection Route\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        cout << "\n  --- City Map ---\n"
             << "       [4] Res. North --(3km)-- [7] Comm. Hub --(6km)-- [8] Univ. Area\n"
             << "        |                        |                       |\n"
             << "      (2km)                    (1km)                   (2km)\n"
             << "        |                        |                       |\n"
             << "       [1] Main Street -------- [3] Ind. Area           [5] Res. South\n"
             << "        |                                                |\n"
             << "      (2km)                                            (3km)\n"
             << "        |                                                |\n"
             << "       [0] Central Depot ---------(3km)----------------- [2] Park Avenue\n"
             << "                                                         |\n"
             << "                                                       (5km)\n"
             << "                                                         |\n"
             << "       [9] Recyc. Center ---------(4km)----------------- [6] Market Dist.\n\n"
             << "  All Connections:\n";
        city_graph->showAdj();
    } else if (choice == 2) {
        int s; cout << "  Enter starting zone number (0-9): "; cin >> s; cin.ignore();
        if (s >= 0 && s < 10) {
            cout << "\n  -------------------------------------------------------------\n";
            city_graph->bfs(s);
            cout << "  -------------------------------------------------------------\n";
            action_stack.push("Checked connections from " + city_graph->getName(s));
        } else cout << "  Invalid.\n";
    } else if (choice == 3) {
        int s; cout << "  Enter starting zone number (0-9): "; cin >> s; cin.ignore();
        if (s >= 0 && s < 10) {
            cout << "\n  -------------------------------------------------------------\n";
            city_graph->dfs(s);
            cout << "  -------------------------------------------------------------\n";
            action_stack.push("Checked network connectivity from " + city_graph->getName(s));
        } else cout << "  Invalid.\n";
    } else if (choice == 4) {
        for (int i = 0; i < 10; i++) cout << "    [" << i << "] " << city_graph->getName(i) << "\n";
        int src, dst; cout << "  Enter Source area number: "; cin >> src; cout << "  Enter Destination area number: "; cin >> dst; cin.ignore();
        if (src >= 0 && src < 10 && dst >= 0 && dst < 10) {
            cout << "\n  -------------------------------------------------------------\n";
            city_graph->dijkstra(src, dst);
            cout << "  -------------------------------------------------------------\n";
            action_stack.push("Planned route from " + city_graph->getName(src) + " to " + city_graph->getName(dst));
        } else cout << "  Invalid area numbers.\n";
    }
    if (choice != 0) pressEnterToContinue();
}

void environmentalReport() {
    cout << "\n--------------------------------------------------\n"
         << "## Environmental Report\n"
         << "--------------------------------------------------\n";
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
    int urgentCount = 0; double totalVolume = 0, recyclableVolume = 0;
    for (int i = 0; i < size; i++) {
        double volume = tempArr[i].wasteLevel * tempArr[i].capacity / 100.0;
        totalVolume += volume; if (tempArr[i].wasteLevel >= 80) urgentCount++;
        if (tempArr[i].wasteType == "Recyclable") { recyclableVolume += volume; }
    }
    double recRate = totalVolume > 0 ? (recyclableVolume / totalVolume) * 100.0 : 0.0;
    int rInt = (int)recRate; int rDec = (int)((recRate - rInt) * 10);

    cout << "  Total Bins Monitored    : " << size << "\n"
         << "  Bins Needing Collection : " << urgentCount << "\n"
         << "  Estimated Waste Volume  : " << totalVolume << " L\n"
         << "  Recyclable Waste Share  : " << rInt << "." << rDec << "%\n"
         << "  Pending Citizen Issues  : " << request_queue.size() << "\n"
         << "  Registered Vehicles     : " << vehicle_hash.count << "\n"
         << "  --------------------------------------------------\n";
    action_stack.push("Viewed Environmental Report"); pressEnterToContinue();
}

void searchingAlgorithms() {
    cout << "\n--------------------------------------------------\n"
         << "## Search Records\n"
         << "--------------------------------------------------\n"
         << "  [1] Search by Bin ID\n  [2] Search by Location\n  [0] Back\n  Enter choice: ";
    int choice = readChoice();
    WasteBin tempArr[100]; int size = 0; avl_tree.getAll(tempArr, size);
    if (choice == 1) {
        int id; cout << "  Enter Bin ID to search: "; cin >> id; cin.ignore();
        int idx = binarySearch(tempArr, size, id);
        if (idx != -1) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(&tempArr[idx]); }
        else cout << "\n  [ALERT] Bin ID: " << id << " was not found.\n";
        action_stack.push("Searched for Bin ID " + to_string(id));
    } else if (choice == 2) {
        string keyword; cout << "  Enter location keyword: "; getline(cin, keyword);
        int idx = linearSearch(tempArr, size, keyword);
        if (idx != -1) { cout << "\n  [RESULT] Bin Found:\n"; printBinCard(&tempArr[idx]); }
        else cout << "\n  [ALERT] Location containing '" << keyword << "' not found.\n";
        action_stack.push("Searched for location: " + keyword);
    }
    if (choice != 0) pressEnterToContinue();
}

void complexityAnalysis() {
    cout << "\n--------------------------------------------------\n"
         << "## System Performance Summary\n"
         << "--------------------------------------------------\n"
         << "  Feature Performance:\n\n"
         << "    Feature Area              Time Requirement        Extra Space\n"
         << "    --------------------------------------------------------------\n"
         << "    Bin add/search/remove     O(log N)                O(1)\n"
         << "    Vehicle plate lookup      O(1) average            O(1)\n"
         << "    Recovery log operation    O(1)                    O(1)\n"
         << "    Service request handling  O(1)                    O(1)\n"
         << "    Collection priority list  O(N log N)              O(1)\n"
         << "    Search by bin ID          O(log N)                O(1)\n"
         << "    Search by location        O(N)                    O(1)\n"
         << "    Area coverage check       O(V + E)                O(V)\n"
         << "    Shortest route planning   O(V^2)                  O(V)\n\n"
         << "  System Scale Details: N = bins count, V = zones (10), E = connections (12)\n";
    action_stack.push("Viewed system performance summary"); pressEnterToContinue();
}

void systemPerformance() {
    cout << "\n--------------------------------------------------\n"
         << "## System Performance\n"
         << "--------------------------------------------------\n"
         << "  [1] Primary Storage Status\n"
         << "  [2] Backup Storage Status\n"
         << "  [3] Storage Comparison\n"
         << "  [4] Feature Performance Summary\n"
         << "  [0] Back\n"
         << "  Enter choice: ";
    int choice = readChoice();
    if (choice == 1) {
        avl_tree.display();
        cout << "\n--------------------------------------------------\n"
             << "## Primary Storage Status\n"
             << "--------------------------------------------------\n"
             << "  Storage Order: "; avl_tree.preorder(avl_tree.root); cout << "\n\n";
        avl_tree.printAVLStructure(avl_tree.root);
        cout << "  --------------------------------------------------\n";
    } else if (choice == 2) {
        bst_tree.display();
        cout << "\n--------------------------------------------------\n"
             << "## Backup Storage Status\n"
             << "--------------------------------------------------\n"
             << "  Storage Order: "; bst_tree.preorder(bst_tree.root); cout << "\n\n";
        bst_tree.printBSTStructure(bst_tree.root);
        cout << "\n  (Note: Backup storage may become uneven with ordered entries.)\n"
             << "  --------------------------------------------------\n";
    } else if (choice == 3) {
        cout << "\n--------------------------------------------------\n"
             << "## Storage Comparison\n"
             << "--------------------------------------------------\n"
             << "    Primary Storage Depth: " << avl_tree.getHeight() << " levels\n"
             << "    Backup Storage Depth : " << bst_tree.getHeight() << " levels\n"
             << "  ---------------------------------------------------\n";
    } else if (choice == 4) {
        complexityAnalysis();
        return;
    }
    if (choice != 0) pressEnterToContinue();
}

int main() {
    loadSampleData();
    while (true) {
        // Count urgent bins
        WasteBin tempArr[100]; int size = 0;
        avl_tree.getAll(tempArr, size);
        int urgentCount = 0;
        for (int i = 0; i < size; i++) {
            if (tempArr[i].wasteLevel >= 80) urgentCount++;
        }
        string recommendedAction;
        if (urgentCount > 0) recommendedAction = "Review Collection Planning";
        else if (request_queue.size() > 0) recommendedAction = "Process Citizen Requests";
        else recommendedAction = "System operating normally";

        cout << "\n--------------------------------------------------\n"
             << "## Smart Waste Collection & Recycling Management System\n"
             << "--------------------------------------------------\n"
             << "  System Dashboard\n\n"
             << "  Total Bins          : " << size << "\n"
             << "  Urgent Bins         : " << urgentCount << " need collection\n"
             << "  Pending Requests    : " << request_queue.size() << " citizen issues\n"
             << "  Registered Vehicles : " << vehicle_hash.count << "\n"
             << "  Recommended Action  : " << recommendedAction << "\n"
             << "--------------------------------------------------\n\n"
             << "  [1] Waste Bin Management\n"
             << "  [2] Collection Planning\n"
             << "  [3] Citizen Service Requests\n"
             << "  [4] Vehicle Management\n"
             << "  [5] Route Planning\n"
             << "  [6] Environmental Report\n"
             << "  [7] System Performance\n"
             << "  [8] Recovery Log\n"
             << "  [0] Exit\n"
             << "--------------------------------------------------\n"
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
        else if (choice == 0) { cout << "\n  Thank you for using the Smart Waste Management System!\n"; break; }
        else cout << "  Invalid choice. Try again.\n";
    }
    return 0;
}
