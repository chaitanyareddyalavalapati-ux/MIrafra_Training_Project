#ifndef SKY_AIRLINES_H
#define SKY_AIRLINES_H
#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<tuple>
#include<climits>
#include<string>
#include<ctime>
#include<fstream>
#include<iomanip>
using namespace std;

// Stores confirmed booking details for cancellation support
struct Booking {
    string ticketID;
    string passengerName;
    string origin;
    string destination;
    int    totalCost;
    double totalTime;
    time_t bookingTime;   // Unix timestamp at time of booking
    string  cancelled;
};

#define RESET   "\033[0m"
#define BOLD    "\033[1m"

// City Colors
#define BLR_COLOR  "\033[32m"
#define DEL_COLOR  "\033[34m"
#define HYD_COLOR  "\033[33m"
#define BOM_COLOR  "\033[35m"
#define MAA_COLOR  "\033[36m"
#define KOL_COLOR  "\033[31m"
#define AMD_COLOR  "\033[93m"
#define GOI_COLOR  "\033[96m"
#define MAGENTA    "\033[95m"
#define HEADER   "\033[1;34m"
#define SUCCESS  "\033[1;32m"
#define WARNING  "\033[1;33m"
#define ERROR    "\033[1;31m"
#define INFO     "\033[1;36m"
#define MONEY    "\033[1;35m"
#define PLANE    "\033[1;37m"

struct dataa {
    int Origin_City;
    int Destination_City;
    vector<int> path;
    int cost;
    double time;
};

struct Flight {
    int    Origin_City;
    int    Destination_City;
    int    Cost;
    double Time;
};

struct RouteResult {
    vector<int> path;
    int         cost;
    double      time;
    bool        found;
};

extern vector<string> Route;
string cityColor(const string& city);

class Flight_route {

private:
    void initialise();
    int  findparent(int s);

    // FIX 2: Two adjacentlist overloads
    // Overload 1 — for MST/DFS: Flight edges → simple tuple graph
    void adjacentlist(vector<Flight>& edges,
                      vector<vector<tuple<int,int,double>>>& adj);
    // Overload 2 — for routing: dataa edges → extended tuple graph
    void adjacentlist(vector<vector<tuple<vector<int>,int,int,double>>>& Final_connections,
                      vector<dataa> adj);

    int  getindex(const string& src);
    void kruskal();

    // FIX 2: dijkstra signature matches extended tuple graph
    RouteResult dijkstra(const string& src, const string& des,vector<vector<tuple<vector<int>,int,int,double>>>& graph,bool optimizeByCost);

public:
    int vertices;
    vector<vector<tuple<int,int,double>>>Flight_connections;  // for DFS/MST
    vector<vector<tuple<vector<int>,int,int,double>>>Final_connections;   // for routing
    vector<Flight>Flight_connection;
    vector<int>parent;
    vector<int>rank_;
    vector<Flight>Result;
    vector<double>  travelTime;   // FIX 6: renamed from 'time' to avoid shadowing std::time
    static int ticketCounter;
    vector<dataa>   newresult;
    vector<Booking> bookings;
    vector<dataa>   economy;
    vector<dataa>   business;

    Flight_route(int v);
    void add_routes(Flight f);
    void expand(int newSize);
    void computeMST();

    // FIX 3: findRoute takes classRoutes parameter
    RouteResult findRoute(const string& src, const string& des,
                          vector<dataa> classRoutes, bool optimizeByCost);

    int  cityIndex(const string& name);
    void available_airports();
    void dfsUtil(int u, int dest, vector<bool>& visited, vector<int>& path,
                 int totalCost, double totalTime, int& routeCount);
    void printAllRoutes(string src, string dest);
    void cancelTicket();
    void seperating(const vector<dataa>& newresult,
                    vector<dataa>& economy, vector<dataa>& business);
    void printfinal(string origin, string destination, const vector<dataa>& result);
    void suggestion(string origin, string destination, vector<dataa> classRoutes,int);
    void savebooking();
   RouteResult findshort(vector<dataa>&,int );
};

void runAdmin(Flight_route& f);
void viewMST(Flight_route& f);
void addAirport(Flight_route& f);
void runUser(Flight_route& f);
void view_bookings();
void printRoute(const string& label, RouteResult& r, int num);
void generateGraphDOT(Flight_route& f, bool showMST);
//void showTerminalGraph(Flight_route& f);

#endif
