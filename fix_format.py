import re

def pad_table(lines):
    # Finds the maximum width for each column and pads the cells
    # This assumes standard markdown tables
    header = lines[0].strip().split('|')[1:-1]
    rows = [line.strip().split('|')[1:-1] for line in lines[2:]]
    
    col_widths = [len(c.strip()) for c in header]
    for row in rows:
        for i, c in enumerate(row):
            col_widths[i] = max(col_widths[i], len(c.strip()))
            
    def format_row(row_cells, is_separator=False):
        formatted = "|"
        for i, c in enumerate(row_cells):
            if is_separator:
                # keep alignment if there is :---
                if ':' in c:
                    formatted += f" {c.strip():-<{col_widths[i]}} |"
                else:
                    formatted += f" {'-' * col_widths[i]} |"
            else:
                formatted += f" {c.strip():<{col_widths[i]}} |"
        return formatted

    output = [format_row(header)]
    output.append(format_row(lines[1].strip().split('|')[1:-1], is_separator=True))
    for row in rows:
        output.append(format_row(row))
    return '\n'.join(output)

readme_flowchart = """```text
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
```"""

with open('/Users/ankitrajjha/DSA-FINAL/README.md', 'r') as f:
    readme = f.read()

# Replace flowchart
readme = re.sub(r'```text\n\s+\+-------------------------.*?```', readme_flowchart, readme, flags=re.DOTALL)

# Reformat tables
readme = readme.replace('$O(', 'O(').replace(')$', ')').replace('\\log', 'log') # Remove latex math

# Format tables (manual extraction and replacement for safety)
table1 = """| Feature | Data Structure / Algorithm | Purpose |
| -------------------- | --------------------------------- | ----------------------------- |
| Collection Scheduling | Sorting Algorithms (Merge/Quick Sort) | Prioritize waste collection schedules based on zone generation |
| Record Retrieval | Searching Algorithms (Binary Search) | Retrieve collection and recycling records efficiently |
| Recovery Procedures | Stack | Manage operational recovery procedures and undo changes |
| Service Requests | Queue | Process citizen and municipal service requests |
| Waste Database | BST & AVL Trees | Store and maintain structured waste management databases |
| Route Optimization | Graph (BFS, DFS, Dijkstra's) | Optimize collection routes for vehicles to reduce costs |
| Asset Identification | Hash Table | Enable rapid asset (bins, vehicles) identification |"""

table2 = """| Operation | Time Complexity | Space Complexity |
| ------------------------ | ---------------------- | -------------------------- |
| Sorting Schedules (Merge/Quick) | O(N log N) | O(N) or O(log N) |
| Searching Records (Binary Search)| O(log N) | O(1) |
| Stack Push/Pop | O(1) | O(A) |
| Queue Enqueue/Dequeue | O(1) | O(S) |
| AVL Tree Insert/Search | O(log D) | O(D) |
| Graph Route (Dijkstra) | O((V + E) log V) | O(V + E) |
| Hash Table Insert/Search | O(1) Avg | O(C) |"""

readme = readme.replace(table1, pad_table(table1.split('\n')))
readme = readme.replace(table2, pad_table(table2.split('\n')))

with open('/Users/ankitrajjha/DSA-FINAL/README.md', 'w') as f:
    f.write(readme)


with open('/Users/ankitrajjha/DSA-FINAL/report.MD', 'r') as f:
    report = f.read()

table3 = """| Feature | Data Structure / Algorithm | Justification |
| :--- | :--- | :--- |
| **Collection Scheduling** | **Sorting Algorithms (Merge/Quick)** | Orders the collection zones by waste generation level to prioritize high-need areas efficiently. |
| **Record Retrieval** | **Searching Algorithms (Binary Search)** | Allows quick, logarithmic-time lookup of historical recycling and collection records from sorted databases. |
| **Recovery Procedures** | **Stack** | Uses LIFO (Last-In-First-Out) ordering. The most recent operational change is at the top, perfectly mimicking the behavior of an 'Undo' button. |
| **Service Requests** | **Queue** | Enforces FIFO (First-In-First-Out) processing, guaranteeing that citizen or municipal requests are processed strictly in the order they were submitted. |
| **Asset Identification** | **Hash Table** | Provides near-instant ($O(1)$) lookup times to verify if a given physical asset (RFID or GPS ID) already exists or to find its status. |
| **Waste Database** | **BST / AVL Trees** | Automatically balances the collection database so that insertion, deletion, and searching remain fast and predictable at $O(\log N)$. |
| **Route Optimization** | **Graph & Dijkstra's Algorithm** | Models city intersections as vertices and roads as edges with distance weights. Dijkstra’s guarantees the shortest and most cost-effective path for garbage trucks. |"""

table4 = """| Component | Operation | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Sorting Algorithms** | Sort Schedules | $O(N \log N)$ | $O(N)$ or $O(\log N)$ |
| **Searching Algorithms**| Record Lookup | $O(\log N)$ | $O(1)$ |
| **Stack** | Push / Pop | $O(1)$ | $O(A)$ where $A$ is number of stored actions |
| **Queue** | Enqueue / Dequeue | $O(1)$ | $O(S)$ where $S$ is requests in queue |
| **Hash Table** | Insert / Lookup | $O(1)$ average, $O(C)$ worst | $O(C)$ where $C$ is unique assets |
| **BST/AVL Tree** | Insert / Search | $O(\log D)$ | $O(D)$ where $D$ is collection points |
| **Graph** | Add Edge | $O(1)$ average | $O(V + E)$ where $V$=intersections, $E$=roads |
| **Dijkstra / BFS** | Route finding | $O((V + E) \log V)$ | $O(V)$ for priority queue and visited sets |"""

report = report.replace(table3, pad_table(table3.split('\n')))
report = report.replace(table4, pad_table(table4.split('\n')))

with open('/Users/ankitrajjha/DSA-FINAL/report.MD', 'w') as f:
    f.write(report)
