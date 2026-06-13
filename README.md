# Smart Waste Collection & Recycling Management System

## 2.1 Project Title

**Smart Waste Collection & Recycling Management System Using Data Structures and Algorithms in C++**

---

## 2.2 Problem Statement

Municipal corporations and environmental services face significant challenges in managing urban waste collection, optimizing recycling operations, and monitoring collection vehicles effectively. Inefficient routing leads to higher fuel consumption, service requests pile up without proper prioritization, and tracking specific recycling assets becomes tedious, overall negatively impacting environmental sustainability.

The objective of this project is to design and implement a **Smart Waste Collection & Recycling Management** core platform. This system leverages fundamental Data Structures and Algorithms (DSA) to efficiently manage waste collection, optimize recycling operations, monitor collection vehicles, and improve environmental sustainability.

The system supports scheduling, service request processing, operational recovery, optimal route finding, and fast asset identification to build a comprehensive waste management dashboard.

---

## 2.3 Objectives

The primary objectives of this project are:

* Prioritize high-waste generation zones using optimal **Custom Heap Sort** and **Quick Sort** Algorithms for collection schedules.
* Retrieve collection and recycling records rapidly using robust **Binary Search** and **Linear Search**.
* Manage operational recovery log procedures efficiently using a **Custom Linked-List Stack** mechanism (Undo actions).
* Process incoming citizen service requests in a fair, sequential order using a **Custom Linked-List Queue**.
* Store and maintain waste management databases dynamically using **BST & AVL Trees** for balanced performance.
* Optimize vehicle utilization and reduce collection costs by finding the shortest and most efficient collection routes using **BFS, DFS, and Dijkstra's Algorithm** on road networks.
* Enable rapid asset identification and tracking using a **Custom Hash Table with Chaining Collision Resolution**.
* Improve recycling efficiency and enhance overall environmental sustainability through data-driven structures.
* Demonstrate the practical applications of multiple DSA concepts in a real-world municipal environment.

---

## 2.4 System Overview / Architecture

### System Workflow

1. Citizens and authorities log service requests into the system.
2. Service requests are processed in a FIFO manner via a **Custom Queue**.
3. New waste bins and recycling assets are registered and rapidly identified using a **Custom Hash Table**.
4. Waste management databases (bins, zones, vehicle statuses) are stored and balanced dynamically using **AVL & BST Trees**.
5. Collection schedules are dynamically prioritized using **Heap Sort** (sorting by priorities).
6. Vehicle routes to collection points are optimized using **Dijkstra's Algorithm / BFS / DFS** on the city's road network graph.
7. Any accidental changes or required rollbacks in operations are managed by a **Custom Stack** for recovery log tracking.
8. Historical collection and recycling records are quickly retrieved via **Binary Search** by ID or **Linear Search** by Location.

### Architecture Diagram

```text
                +----------------+
                | Service Request|
                +--------+-------+
                         |
                         v
               +------------------+
               | Asset Check      |
               | (Hash Table)     |
               +--------+---------+
                         |
                         v
               +------------------+
               | Request Queue    |
               | (Custom Queue)   |
               +--------+---------+
                         |
           +--------------+--------------+
           |              |              |
           v              v              v
 
      +---------+    +---------+    +---------+
      | AVL Tree|    | Sorting |    | Graph   |
      | Database|    | Priorit.|    | Routing |
      +---------+    +---------+    +---------+
 
                          |
                          v
 
                +------------------+
                | Recovery Stack   |
                | (Custom Stack)   |
                +------------------+
```
*(Note: A Stack is maintained alongside for operational recovery / Undo actions)*

---

## 2.5 Data Structures and Algorithms Used

| Feature | Data Structure / Algorithm | Purpose |
| :--- | :--- | :--- |
| **Collection Scheduling** | Custom Heap Sort & Quick Sort | Prioritize waste collection schedules based on fill levels and capacity. |
| **Record Retrieval** | Binary Search & Linear Search | Retrieve collection and recycling records efficiently. |
| **Recovery Procedures** | Custom Singly Linked-List Stack | Manage operational recovery procedures and undo changes. |
| **Service Requests** | Custom Singly Linked-List Queue | Process citizen and municipal service requests in FIFO order. |
| **Waste Database** | BST & AVL Trees | Store and maintain balanced waste management databases. |
| **Route Optimization** | Graph (BFS, DFS, Dijkstra's) | Optimize collection routes for vehicles on the city map network. |
| **Asset Identification** | Custom Hash Table (Collision Chaining) | Enable rapid GPS/RFID asset (bins, vehicles) identification. |

---

## 2.6 Implementation Approach

### 1. Collection Scheduling (Heap Sort & Quick Sort)
* **Custom Heap Sort**: Implemented a min-heap structure sorting algorithm to arrange bins in descending order of priority scores ($score = level \times \frac{capacity}{100}$). 
* **Custom Quick Sort**: Implemented descending partition-based Quick Sort to sort database subsets. No `std::sort` or library shortcuts are used.

### 2. Record Retrieval (Searching Algorithms)
* **Binary Search**: Written standard loop-based Binary Search to query collection points by their unique IDs in $O(\log N)$ logarithmic time.
* **Linear Search**: Written sequential string matching search using `toLowerCase` char loop helper for finding bins by location keyword in $O(N)$ time.

### 3. Recovery Procedures (Custom Stack)
* Critical operational modifications (e.g. adding new bins, routing vehicles) log descriptive strings onto a custom Stack.
* The Stack is built using dynamic singly linked-list nodes, ensuring $O(1)$ push/pop operations.

### 4. Service Requests (Custom Queue)
* Incoming service issues (e.g. overflowing bins) are enqueued in a custom FIFO Queue.
* The Queue maintains `frontNode` and `rearNode` pointers to achieve $O(1)$ enqueue and dequeue times.

### 5. Waste Database (BST & AVL Trees)
* Bins are stored in an **AVL Tree** (primary storage) and **BST** (backup storage).
* Height balancing rotates AVL Tree nodes ($LL$, $RR$, $LR$, $RL$ rotations) dynamically to guarantee $O(\log N)$ lookup performance.

### 6. Route Optimization (Graph Traversals & Dijkstra's)
* City zones are modeled as a Graph of vertices and edges with weights (km distances).
* **BFS** sweeps reachable nodes from a depot. **DFS** checks overall graph connectivity.
* **Dijkstra's Algorithm** uses a classic linear distance scan array (no complex library priority queues) to find the shortest collection route.

### 7. Asset Identification (Custom Hash Table)
* Vehicle records are indexed inside a Hash Table of size 53 using polynomial rolling hashing ($h = \sum key[i] \times 31^i \pmod{53}$).
* Collision resolution is handled via a custom singly linked-list chaining configuration (`Node* table[53]`).

---

## 2.7 Time and Space Complexity Analysis

| Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- |
| **Heap Sort Scheduling** | $O(N \log N)$ | $O(1)$ auxiliary |
| **Quick Sort Sorting** | $O(N \log N)$ average | $O(\log N)$ stack recursion |
| **Binary Search lookup** | $O(\log N)$ | $O(1)$ |
| **Stack push / pop** | $O(1)$ | $O(A)$ total actions |
| **Queue enqueue / dequeue** | $O(1)$ | $O(S)$ total requests |
| **AVL Tree insertion / lookup** | $O(\log D)$ | $O(D)$ total nodes |
| **Graph Route (Dijkstra)** | $O(V^2 + E)$ | $O(V)$ auxiliary arrays |
| **Hash Table insert / lookup** | $O(1)$ average | $O(C)$ total vehicles |

*Where:*
* **$N$** = Number of bins sorted
* **$A$** = Number of logged recovery actions
* **$S$** = Number of pending requests
* **$D$** = Total registered bins in database
* **$V$** = Number of zones/intersections (10)
* **$E$** = Number of connections/roads (12)
* **$C$** = Number of registered vehicles

---

## 2.8 Execution Steps

### Prerequisites
* C++ Compiler (GCC/G++ or Clang) supporting C++11 or higher.

### Compile
Compile the source code using the following terminal command:
```bash
g++ -std=c++17 smart_waste_system.cpp -o smart_waste_system
```

### Run
Launch the executable:
```bash
./smart_waste_system
```

---

## 2.9 Sample Inputs and Outputs

### 1. Main System Dashboard (Menu Layout)
When launched, the program displays a clean status summary dashboard and a grid selection menu:
```text
  +--------------------------------------------------------+
  |  Smart Waste Collection & Recycling Management System  |
  +--------------------------------------------------------+
  |  System Status                                         |
  |    Total Bins          : 11                            |
  |    Urgent Bins         : 4 need collection             |
  |    Pending Requests    : 5 citizen issues              |
  |    Registered Vehicles : 6                             |
  |    Recommended Action  : Review Collection Planning    |
  +--------------------------------------------------------+
  |  [1] Waste Bin Management                              |
  |  [2] Collection Planning                               |
  |  [3] Citizen Service Requests                          |
  |  [4] Vehicle Management                                |
  |  [5] Route Planning                                    |
  |  [6] Environmental Report                              |
  |  [7] Technical Analysis                                |
  |  [8] Recovery Log                                      |
  |  [0] Exit                                              |
  +--------------------------------------------------------+
  Enter Choice: 
```

### 2. Collection Scheduling (Heap Sorted Priority List)
Selecting option `[2]` runs the custom Heap Sort algorithm and outputs the bins sorted by priority:
```text
  === Collection Priority List ===
  +------------+----------+------------------------+--------+-------------------+
  | Priority   | Bin ID   | Location               | Fill   | Action Required   |
  +------------+----------+------------------------+--------+-------------------+
  | 1          | 103      | Industrial Area        | 88%    | Collect Now       |
  | 2          | 106      | Market District        | 95%    | Collect Now       |
  | 3          | 101      | Main Street            | 92%    | Collect Now       |
  | 4          | 107      | Commercial Hub         | 55%    | Schedule Soon     |
  | 5          | 109      | Town Square            | 83%    | Collect Now       |
  ...
```

### 3. Route Optimization (Dijkstra Shortest Path)
Selecting option `[5]` -> option `[4]` computes the shortest routing paths across the road graph network:
```text
  Shortest Route from Central Depot to Recycling Center:
  [Central Depot] --(3km)--> [Park Avenue] --(5km)--> [Market District] --(4km)--> [Recycling Center]

  Total Distance: 12 km
```

---

## 2.10 Results and Observations

* **Modular Structuring**: Designing custom linked-list stacks and queues verified memory consumption bounds and isolated logic cleanly from standard STL boilerplate.
* **Hash Collision Resolution**: Singly linked chaining demonstrated true polynomial hashing indexing and collision recovery, ensuring consistent $O(1)$ asset lookups.
* **Balancing Trees**: AVL Tree rotations maintained logarithmic height bounds, preventing skewed trees even when loading sequential bin IDs (unlike basic BSTs).
* **Dijkstra Utility**: The optimized pathfinder achieved significant reductions in simulated collection trip distances, proving graph theory's applicability to municipal fuel management.
* **Redirection Safety**: The execution logic terminates cleanly upon reaching standard input EOF, avoiding terminal spinning or thread lockups when scripting.

---

## 2.11 Conclusion

The Smart Waste Collection & Recycling Management System clearly demonstrates how fundamental Data Structures and Algorithms can be orchestrated to solve complex logistical and environmental challenges. By integrating custom Queues, Stacks, Hash Tables, Tree Nodes, and Graph algorithms, the project transforms inefficient municipal routines into a highly optimized, prioritized, and trackable workflow.

This project not only satisfies the requirement of combining multiple algorithms into a single platform but also provides a strong conceptual framework for real-world integration with IoT bin sensors, GPS truck tracking, and live traffic data for future scaling.

---
**Author**
*Ankitraj jha*
