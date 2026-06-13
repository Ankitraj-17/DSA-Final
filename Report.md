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

## 4. 20 Viva Questions with Answers

**Q1: What is the primary advantage of using a Hash Table for Asset Identification over an Array?**
A: An array takes $O(N)$ time to search for a specific asset ID. A Hash Table computes an index directly from the ID and accomplishes this lookup in $O(1)$ average time.

**Q2: Why use an AVL Tree instead of a standard Binary Search Tree (BST) for the Waste Database?**
A: A standard BST can become skewed (like a linked list) if data is inserted in a sorted order, degrading search time to $O(N)$. An AVL tree automatically balances itself to strictly maintain $O(\log N)$ operations.

**Q3: How do Sorting Algorithms maintain the collection schedule priority?**
A: Algorithms like Quick Sort or Merge Sort reorder the array of zones based on their specific waste volume score, allowing the fleet to always address the highest volume zones efficiently in $O(N \log N)$ time.

**Q4: Can we detect duplicate service requests using an Array instead of a Hash Table?**
A: Yes, but it's inefficient. Searching an unsorted array takes $O(N)$ time. A Hash Table accomplishes existence checks in $O(1)$ average time.

**Q5: Why is an Adjacency List better than an Adjacency Matrix for the City Map Graph?**
A: Road networks form a "sparse" graph (most intersections only connect to 3 or 4 roads). A matrix requires $O(V^2)$ space, while an adjacency list requires $O(V + E)$ space, saving massive amounts of memory.

**Q6: Explain how Dijkstra's Algorithm finds the optimal collection route.**
A: Dijkstra's explores paths starting from the depot using a priority queue to always extend the shortest known path. It mathematically guarantees finding the absolute minimum distance to the target neighborhood.

**Q7: Why use Breadth-First Search (BFS) in the Graph?**
A: BFS explores all neighbors at the current depth before moving deeper. If edge weights are uniform (e.g. block-by-block routing), BFS guarantees finding the route with the fewest turns or intersections.

**Q8: Explain the logic behind your Queue for Service Requests.**
A: It enforces FIFO (First-In-First-Out) behavior. When citizens log overflow bins, requests must be processed in the exact order they arrive to maintain fairness and sequential consistency.

**Q9: What happens if two assets have the same Hash ID?**
A: This is a collision. Our Hash Table handles it (via chaining or probing), but ideally, asset IDs are unique. If the system detects it via an `unordered_set` or map, it halts the registration and flags it as a duplicate in $O(1)$ time.

**Q10: What does the Recovery Procedure (Stack) achieve?**
A: It implements an "Undo" feature. By pushing actions (like vehicle reassignment) onto a stack, the last action pushed is the first one popped, which perfectly mirrors operator undo expectations.

**Q11: Why is Binary Search chosen for Record Retrieval?**
A: Historical records are kept chronologically sorted. Binary Search divides the search space in half each step, finding records in $O(\log N)$ rather than checking every record in $O(N)$ linear time.

**Q12: In Graph traversal, what is the role of the 'visited' set?**
A: To prevent infinite loops. City streets often have cycles (e.g. roundabouts or city blocks). Marking an intersection as 'visited' ensures BFS or Dijkstra's doesn't process the same intersection endlessly.

**Q13: How does the system handle an empty Stack during an Undo request?**
A: The system checks if the Stack is empty (`stack.empty()`) in $O(1)$ time before attempting to pop. If empty, it alerts the user that there are no actions to undo, preventing a segmentation fault.

**Q14: Are classes inside main.cpp good practice for production?**
A: No. In production, classes should be separated into `.h` headers and `.cpp` implementation files for modularity, reusability, and faster compilation. However, for this specific assignment requirement, they were consolidated.

**Q15: How does std::priority_queue know to act as a Min Heap for Dijkstra's?**
A: We provide a custom comparator struct (e.g., `std::greater<>`) that overloads the `operator()`. By returning `a > b`, we instruct the queue to bubble the smallest distances to the top.

**Q16: What is the time complexity of building the City Map Graph?**
A: Adding a street/edge to the `unordered_map`-based Adjacency List takes average $O(1)$ time. Thus, loading $E$ roads takes $O(E)$ time.

**Q17: Why do we use Merge Sort or Quick Sort instead of Bubble Sort for scheduling?**
A: Bubble Sort takes $O(N^2)$ time, which is vastly too slow for evaluating thousands of city zones. Quick Sort or Merge Sort handle large datasets efficiently in $O(N \log N)$ time.

**Q18: What is the space complexity of the StackManager?**
A: $O(A \times S)$ where $A$ is the number of stored string actions, and $S$ is the average length of the action string.

**Q19: How are the 20 sample collection points processed on startup?**
A: They are hardcoded in a `populateSampleData()` function, pushed through the registration method, sorted into the Database (AVL Tree), and mapped to intersections in the City Map Graph.

**Q20: Can you change the sorting criteria for zones?**
A: Yes. If we wanted to prioritize by "longest time since last pickup" instead of "highest waste volume", we simply update the comparator logic passed to our sorting algorithm.

---

## 5. Step-by-Step Explanation of Program Flow

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
