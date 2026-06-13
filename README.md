# DSA-FINAL-PROJECT

# Smart Waste Collection & Recycling Management System

## 2.1 Project Title

**Smart Waste Collection & Recycling Management System Using Data Structures and Algorithms**

---

## 2.2 Problem Statement

Municipal corporations and environmental services face significant challenges in managing urban waste collection, optimizing recycling operations, and monitoring collection vehicles effectively. Inefficient routing leads to higher fuel consumption, service requests pile up without proper prioritization, and tracking specific recycling assets becomes tedious, overall negatively impacting environmental sustainability.

The objective of this project is to design and implement a **Smart Waste Collection & Recycling Management** core platform. This system leverages fundamental Data Structures and Algorithms (DSA) to efficiently manage waste collection, optimize recycling operations, monitor collection vehicles, and improve environmental sustainability.

The system supports scheduling, service request processing, operational recovery, optimal route finding, and fast asset identification to build a comprehensive waste management dashboard.

---

## 2.3 Objectives

The primary objectives of this project are:

* Prioritize high-waste generation zones using optimal Sorting Algorithms for collection schedules.
* Retrieve collection and recycling records rapidly using robust Searching Algorithms.
* Manage operational recovery procedures efficiently using a Stack mechanism.
* Process incoming service requests in a fair, sequential order using a Queue.
* Store and maintain waste management databases dynamically using BST & AVL Trees for balanced performance.
* Optimize vehicle utilization and reduce collection costs by finding the shortest and most efficient collection routes using BFS, DFS, and Dijkstra's Algorithm.
* Enable rapid asset identification and tracking using Hashing techniques.
* Improve recycling efficiency and enhance overall environmental sustainability through data-driven structures.
* Demonstrate the practical applications of multiple DSA concepts in a real-world municipal environment.

---

## 2.4 System Overview / Architecture

### System Workflow

1. Citizens and authorities log service requests into the system.
2. Service requests are processed in a FIFO manner via a Queue.
3. New waste bins and recycling assets are registered and rapidly identified using a Hash Table.
4. Waste management databases (bins, zones, vehicle statuses) are stored and maintained using AVL Trees.
5. Collection schedules are dynamically prioritized using Sorting Algorithms.
6. Vehicle routes to collection points are optimized using Dijkstra's Algorithm / BFS / DFS on the city's road network graph.
7. Any accidental deletions or required rollbacks in operations are managed by a Stack for recovery.
8. Historical collection and recycling records are quickly retrieved via Searching Algorithms.
9. All data feeds into a Waste Management Dashboard displaying schedules, statistics, tracking, and reports.

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
               | (Queue)          |
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
               | (Stack)          |
               +------------------+
```
*(Note: A Stack is maintained alongside for operational recovery / Undo actions)*

---

## 2.5 Data Structures and Algorithms Used

| Feature               | Data Structure / Algorithm            | Purpose                                                        |
| --------------------- | ------------------------------------- | -------------------------------------------------------------- |
| Collection Scheduling | Sorting Algorithms (Merge/Quick Sort) | Prioritize waste collection schedules based on zone generation |
| Record Retrieval      | Searching Algorithms (Binary Search)  | Retrieve collection and recycling records efficiently          |
| Recovery Procedures   | Stack                                 | Manage operational recovery procedures and undo changes        |
| Service Requests      | Queue                                 | Process citizen and municipal service requests                 |
| Waste Database        | BST & AVL Trees                       | Store and maintain structured waste management databases       |
| Route Optimization    | Graph (BFS, DFS, Dijkstra's)          | Optimize collection routes for vehicles to reduce costs        |
| Asset Identification  | Hash Table                            | Enable rapid asset (bins, vehicles) identification             |

---

## 2.6 Implementation Approach

### 1. Collection Scheduling (Sorting Algorithms)
* Areas are assigned waste generation scores.
* Sorting algorithms (like Quick Sort or Merge Sort) order the zones to prioritize high-waste generation zones for immediate dispatch.

### 2. Record Retrieval (Searching Algorithms)
* Historical logs of recycled materials and collection times are kept sorted.
* Binary Search is utilized to fetch specific records based on date or zone ID in logarithmic time.

### 3. Recovery Procedures (Stack)
* Critical operational changes (e.g., reassigning a vehicle, marking a zone clear) are pushed onto a Stack.
* An 'Undo' operation simply pops the last action, restoring the previous state seamlessly.

### 4. Service Requests (Queue)
* Incoming requests for special pickups or overflowing bins enter a Queue.
* The fleet processes these requests in a strict First-In-First-Out (FIFO) sequence to ensure fairness.

### 5. Waste Database (BST & AVL Trees)
* The core database of registered collection points is stored in an AVL tree.
* This ensures that insertions, deletions, and lookups remain strictly balanced, providing predictable performance as the city grows.

### 6. Route Optimization (Graph Traversals & Dijkstra's)
* The city map is modeled as a Graph where intersections are vertices and roads are edges with weights representing distance or traffic.
* Dijkstra's algorithm finds the absolute shortest path for the collection trucks.
* BFS/DFS are used to ensure all nodes in a specific neighborhood subclass are visited during a standard sweep.

### 7. Asset Identification (Hashing)
* RFID tags on bins and GPS IDs on vehicles are mapped using Hash Tables.
* This provides O(1) average time complexity to locate an asset's current status and details.

---

## 2.7 Time and Space Complexity Analysis

| Operation                         | Time Complexity  | Space Complexity |
| --------------------------------- | ---------------- | ---------------- |
| Sorting Schedules (Merge/Quick)   | O(N log N)       | O(N) or O(log N) |
| Searching Records (Binary Search) | O(log N)         | O(1)             |
| Stack Push/Pop                    | O(1)             | O(A)             |
| Queue Enqueue/Dequeue             | O(1)             | O(S)             |
| AVL Tree Insert/Search            | O(log D)         | O(D)             |
| Graph Route (Dijkstra)            | O((V + E) log V) | O(V + E)         |
| Hash Table Insert/Search          | O(1) Avg         | O(C)             |

*Where:*
* **$N$** = Number of schedules/zones
* **$A$** = Number of actions recorded for recovery
* **$S$** = Number of service requests
* **$D$** = Number of database entries (collection points)
* **$V$** = Number of intersections (vertices)
* **$E$** = Number of roads (edges)
* **$C$** = Number of physical assets (bins/trucks)

---

## 2.8 Execution Steps

### Prerequisites
* C++ Compiler (GCC/G++)
* C++11 or above standard

### Compile
```bash
g++ main.cpp -o SmartWasteManagement
```

### Run
```bash
./SmartWasteManagement
```

### Menu Operations
1. Register New Asset (Bin/Vehicle)
2. Log Service Request
3. Process Next Service Request
4. Prioritize Collection Schedules
5. Add Waste Collection Point to Database
6. Optimize Vehicle Route (Find Shortest Path)
7. Search Historical Records
8. Undo Last Operational Change
9. View Waste Management Dashboard
10. Exit

---

## 2.9 Sample Inputs and Outputs

### Route Optimization (Dijkstra's)

**Input:**
```text
Start Node : Depot (0)
Target Node : Sector 45 (7)
```

**Output:**
```text
Optimal Route: Depot -> Sector 12 -> Sector 30 -> Sector 45
Total Distance/Cost: 14.5 km
```

### Schedule Prioritization

**Output:**
```text
Rank 1 | Waste Level: 95% | Zone: Commercial Hub
Rank 2 | Waste Level: 82% | Zone: Residential Block A
Rank 3 | Waste Level: 60% | Zone: Industrial Park
```

### Service Request Processing

**Input:**
```text
Add Request: Overflowing bin at Main Street
```

**Output:**
```text
Request Added to Queue.
Processing Request: Overflowing bin at Main Street... Resolved.
```

---

## 2.10 Screenshots

*Note: Replace with actual screenshots of your CLI/Dashboard implementation.*

* **Main Dashboard / Menu**
  *[Screenshot 2026-06-12 at 8 34 13 PM]*
* **Asset Hashing Results**
  *[Screenshot 2026-06-12 at 8 35 20 PM]*
* **City Graph Routing**
  *[Screenshot 2026-06-12 at 8 36 15 PM]*
* **Prioritized Schedules**
  *[Screenshot 2026-06-12 at 8 36 39 PM]*

---

## 2.11 Results and Observations

* Successfully implemented a comprehensive suite of DSA concepts tailored for municipal waste management.
* **Queues** effectively managed the backlog of citizen service requests without starvation.
* **Hash Tables** proved essential for the instantaneous lookup of thousands of distributed assets.
* **AVL Trees** maintained a robust, balanced database of collection points, ensuring fast queries regardless of data scale.
* **Dijkstra’s Algorithm** drastically optimized the simulated routes, showcasing tangible reductions in travel distance and logical fuel costs.
* **Sorting Algorithms** ensured that critical, high-waste zones were always serviced first, maximizing environmental hygiene.
* **Stacks** provided a foolproof mechanism for operators to undo accidental route reassignments or database entries.

---

## 2.12 Conclusion

The Smart Waste Collection & Recycling Management System clearly demonstrates how foundational Data Structures and Algorithms can be orchestrated to solve complex logistical and environmental challenges. By integrating Queues, Stacks, Hash Tables, Trees, and Graph algorithms, the project transforms inefficient municipal routines into a highly optimized, prioritized, and trackable workflow.

This project not only satisfies the requirement of combining multiple algorithms into a single platform but also provides a strong conceptual framework for real-world integration with IoT bin sensors, GPS truck tracking, and live traffic data for future scaling.

---
**Author**
*ITM Skills University - B.Tech CSE 2025-29 (Semester II)*
