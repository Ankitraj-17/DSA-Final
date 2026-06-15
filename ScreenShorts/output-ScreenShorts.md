# Smart Waste Management System - Terminal Output "Screenshots"

This document provides captured terminal outputs simulating screenshots of the system in action.

## 1. Main Dashboard & System Status

```text
  +--------------------------------------------------------+
  |  Smart Waste Collection & Recycling Management System  |
  +--------------------------------------------------------+
  |  System Status                                         |
  |    Total Bins          : 11                            |
  |    Urgent Bins         : 4 need collection             |
  |    Pending Requests    : 5 citizen issues              |
  |    Registered Vehicles : 7                             |
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

## 2. Adding a New Waste Bin

```text
  +--------------------------------------------------+
  |               Waste Bin Management               |
  +--------------------------------------------------+
  |  [1] View All Bins                               |
  |  [2] Add New Bin                                 |
  |  [3] Search Bin by ID                            |
  |  [4] Search Bin by Location                      |
  |  [5] Remove Bin                                  |
  |  [0] Back                                        |
  +--------------------------------------------------+
  Enter choice: 2
  === Add New Bin ===
  Enter ID: 2
  Enter Location: 101
  Enter Fill Level (%): 2
  Enter Type (Organic/General/Recyclable/Hazardous): 6
  Enter Capacity (L): 3

  [SUCCESS] New Bin Added Successfully!
  === Waste Bin Details ===
  ID: 2
  Location: 101
  Fill Level: 2%
  Capacity: 3L
  Type: 6
  Current Waste: 0L
  Priority Status: Normal

  Press Enter to continue...
```

## 3. Fleet Management & Registered Vehicles

```text
  +--------------------------------------------------+
  |                Vehicle Management                |
  +--------------------------------------------------+
  |  [1] View Registered Vehicles                    |
  |  [2] Search Vehicle                              |
  |  [3] Add New Vehicle                             |
  |  [0] Back                                        |
  +--------------------------------------------------+
  Enter choice: 1
  === Registered Vehicles ===
  +----------------+--------------------+--------------------+--------------+----------+
  | Plate Number   | Driver Name        | Vehicle Type       | Status       | Zone     |
  +----------------+--------------------+--------------------+--------------+----------+
  | 111            | 2                  | 1                  | 2            | 1        |
  | GV-TRK-001     | Rajesh Kumar       | Compactor          | Available    | Central  |
  | GV-TRK-002     | Suresh Mehta       | Tipper             | On Route     | North    |
  | GV-TRK-003     | Amit Sharma        | Compactor          | Maintenance  | South    |
  | GV-RCY-001     | Priya Singh        | Recycling Van      | Available    | East     |
  | GV-RCY-002     | Deepak Verma       | Recycling Van      | On Route     | West     |
  | GV-TRK-009     | Karan Singh        | Compactor          | Available    | North    |
  +----------------+--------------------+--------------------+--------------+----------+

  Press Enter to continue...
```

## 4. Graph Navigation & Route Planning

```text
  +--------------------------------------------------+
  |                  Route Planning                  |
  +--------------------------------------------------+
  |  [1] Show City Map                               |
  |  [2] View Reachable Service Areas                |
  |  [3] Check City Network Coverage                 |
  |  [4] Find Shortest Collection Route              |
  |  [0] Back                                        |
  +--------------------------------------------------+
  Enter choice: 2
  Enter starting zone number (0-9): 1

  Reachable areas starting from Market District:
  [Market District][Park Avenue] -> [Recycling Center] -> [Central Depot] -> [Residential South] -> [University Area] -> [Main Street] -> [Commercial Hub] -> [Industrial Area] -> [Residential North]

  Press Enter to continue...
```

## 5. Environmental Progress Report

```text
  === Environmental Report ===
  +----------------------------------------+-----------------+
  | Metric                                 | Value           |
  +----------------------------------------+-----------------+
  | Total Bins Monitored                   | 11              |
  | Bins Needing Collection (>=80%)        | 4               |
  | Estimated Waste Volume                 | 3609 L          |
  | Recyclable Waste Share                 | 12.3%           |
  | Recycling Share Progress               | [=              |
  | Pending Citizen Issues                 | 5               |
  | Registered Vehicles                    | 7               |
  +----------------------------------------+-----------------+

  Press Enter to continue...
```
