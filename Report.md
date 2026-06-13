# Smart Waste Collection & Recycling Management System - Final Report

## 1. Project Explanation

The **Smart Waste Collection & Recycling Management System** is a robust simulation of a backend platform designed to manage large-scale urban waste and recycling operations. The system tackles common logistical problems such as rapid asset lookup, fair request processing, operational rollback, collection zone prioritization, and optimizing vehicle routing. By modeling these features through highly specific Data Structures and Algorithms (DSA), the project demonstrates how foundational computer science concepts scale up to solve complex environmental and logistical problems.

The entire project has been synthesized into a monolithic, Object-Oriented `main.cpp` file for easy compilation, featuring an interactive menu system and 20 sample collection assets pre-loaded for immediate testing.

---

## 2. Feature to DSA Mapping

| Feature                   | Data Structure / Algorithm               | Justification                                                                                                                                                       |
| :------------------------ | :--------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| **Collection Scheduling** | **Sorting Algorithms (Merge/Quick)**     | Orders the collection zones by waste generation level to prioritize high-need areas efficiently.                                                                    |
| **Record Retrieval**      | **Searching Algorithms (Binary Search)** | Allows quick, logarithmic-time lookup of historical recycling and collection records from sorted databases.                                                         |
| **Recovery Procedures**   | **Stack**                                | Uses LIFO (Last-In-First-Out) ordering. The most recent operational change is at the top, perfectly mimicking the behavior of an 'Undo' button.                     |
| **Service Requests**      | **Queue**                                | Enforces FIFO (First-In-First-Out) processing, guaranteeing that citizen or municipal requests are processed strictly in the order they were submitted.             |
| **Asset Identification**  | **Hash Table**                           | Provides near-instant ($O(1)$) lookup times to verify if a given physical asset (RFID or GPS ID) already exists or to find its status.                              |
| **Waste Database**        | **BST / AVL Trees**                      | Automatically balances the collection database so that insertion, deletion, and searching remain fast and predictable at $O(\log N)$.                               |
| **Route Optimization**    | **Graph & Dijkstra's Algorithm**         | Models city intersections as vertices and roads as edges with distance weights. Dijkstra’s guarantees the shortest and most cost-effective path for garbage trucks. |

---

## 3. Time and Space Complexity Table

| Component                | Operation         | Time Complexity              | Space Complexity                              |
| :----------------------- | :---------------- | :--------------------------- | :-------------------------------------------- |
| **Sorting Algorithms**   | Sort Schedules    | $O(N \log N)$                | $O(N)$ or $O(\log N)$                         |
| **Searching Algorithms** | Record Lookup     | $O(\log N)$                  | $O(1)$                                        |
| **Stack**                | Push / Pop        | $O(1)$                       | $O(A)$ where $A$ is number of stored actions  |
| **Queue**                | Enqueue / Dequeue | $O(1)$                       | $O(S)$ where $S$ is requests in queue         |
| **Hash Table**           | Insert / Lookup   | $O(1)$ average, $O(C)$ worst | $O(C)$ where $C$ is unique assets             |
| **BST/AVL Tree**         | Insert / Search   | $O(\log D)$                  | $O(D)$ where $D$ is collection points         |
| **Graph**                | Add Edge          | $O(1)$ average               | $O(V + E)$ where $V$=intersections, $E$=roads |
| **Dijkstra / BFS**       | Route finding     | $O((V + E) \log V)$          | $O(V)$ for priority queue and visited sets    |

---

---

## 4. Step-by-Step Explanation of Program Flow

1. **Initialization:**
   * The program starts in `main()`, which instantiates the `SmartWasteManagement` facade.
   * `populateSampleData()` is called. 20 collection points, vehicles, and roads are loaded into the data structures to seed the system.
2. **Registration (Hashing & AVL):**
   * New assets (bins/trucks) are processed via `registerAsset()`. The system hashes the ID and checks the Hash Table. Unique assets enter the AVL Tree for persistent, balanced database storage.
3. **Service Requests (Queueing):**
   * When special or overflow requests arrive, they enter the Request Queue sequentially.
   * `processRequests()` is invoked to dequeue them one by one for the fleet to handle, preventing starvation of older requests.
4. **Routing & Prioritization (Graph & Sorting):**
   * To build the daily schedule, zones are sorted by waste generation levels using Sorting Algorithms.
   * When dispatching a vehicle to the highest priority zone, Dijkstra's algorithm traverses the City Map Graph to output the exact shortest driving route from the depot to the target.
5. **Interactive Menu:**
   * A `do-while` loop starts, displaying the operational options menu.
   * Based on the user's input (`switch` statement), the system routes the request to the specific subsystem (e.g., optimize route calls Dijkstra, retrieve record calls Binary Search).
   * Operational actions (like reassignment) are pushed to the Stack. If the operator hits Undo, the last action is popped and reversed.
6. **Termination:**
   * The system runs until the user selects Exit. The loop breaks, destructors automatically clean up memory (e.g., traversing and deleting AVL tree nodes), and the program terminates safely.
