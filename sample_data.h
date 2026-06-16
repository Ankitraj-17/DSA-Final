#ifndef SAMPLE_DATA_H
#define SAMPLE_DATA_H

#include <string>
using namespace std;

struct WasteBin {
  int id;
  string location;
  double wasteLevel; // 0 to 100
  string wasteType;  // Organic, General, Recyclable, Hazardous
  double capacity;   // Liters
};

struct Vehicle {
  string plate, driver, type, status, zone;
};
struct ServiceRequest {
  int id;
  string name, location, desc, urgency;
};

static WasteBin sample_bins[10] = {
    {101, "Main Street", 92.0, "Organic", 500},
    {102, "Park Avenue", 45.0, "Recyclable", 300},
    {103, "Industrial Area", 88.5, "General", 1000},
    {104, "Residential North", 30.0, "Organic", 400},
    {105, "Residential South", 76.0, "Recyclable", 400},
    {106, "Market District", 95.0, "General", 600},
    {107, "Commercial Hub", 55.0, "Hazardous", 750},
    {108, "University Area", 40.0, "Recyclable", 350},
    {109, "Town Square", 83.0, "Organic", 450},
    {110, "Riverside Zone", 62.0, "General", 550}};

static Vehicle sample_vehicles[5] = {
    {"GV-TRK-001", "Rajesh Kumar", "Compactor", "Available", "Central"},
    {"GV-TRK-002", "Suresh Mehta", "Tipper", "On Route", "North"},
    {"GV-TRK-003", "Amit Sharma", "Compactor", "Maintenance", "South"},
    {"GV-RCY-001", "Priya Singh", "Recycling Van", "Available", "East"},
    {"GV-RCY-002", "Deepak Verma", "Recycling Van", "On Route", "West"}};

static ServiceRequest sample_requests[5] = {
    {1, "Suresh Mehta", "Main Street", "Overflowing bin near bus stop", "High"},
    {2, "Anita Desai", "Park Avenue", "Missed collection Thursday", "Medium"},
    {3, "Vikram Joshi", "Industrial Area", "Hazardous waste leakage", "High"},
    {4, "Meera Patel", "Residential North", "Request new recycling bin", "Low"},
    {5, "Karan Gupta", "Market District", "Foul odour from bins", "Medium"}};

struct SampleEdge {
  int u, v, w;
};

static string sample_city_names[10] = {"Central Depot",     "Main Street",
                                       "Park Avenue",       "Industrial Area",
                                       "Residential North", "Residential South",
                                       "Market District",   "Commercial Hub",
                                       "University Area",   "Recycling Center"};

static SampleEdge sample_edges[12] = {
    {0, 1, 2}, {0, 2, 3}, {1, 3, 4}, {1, 4, 2}, {2, 5, 3}, {2, 6, 5},
    {3, 7, 1}, {4, 7, 3}, {5, 8, 2}, {6, 9, 4}, {7, 8, 6}, {8, 9, 3}};

#endif // SAMPLE_DATA_H
