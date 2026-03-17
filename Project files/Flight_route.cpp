#include "sky_airlines.h"
bool flag=false;
int Flight_route::ticketCounter = 1001;

Flight_route::Flight_route(int v) {
    vertices = v;
    Flight_connections.resize(vertices);
    Final_connections.resize(vertices);
    parent.resize(vertices);
    rank_.resize(vertices, 0);
    travelTime.resize(vertices, INT_MAX);  // FIX 6: renamed member
}

// Reset every node's parent to itself (start of Union-Find)
void Flight_route::initialise() {
    for(int i = 0; i < vertices; i++)
        parent[i] = i;
}

// Find the root of node s with path compression
int Flight_route::findparent(int s) {
    if(parent[s] == s) return s;
    return parent[s] = findparent(parent[s]);
}

// FIX 2: Overload 1 — Build simple adjacency list from Flight edge list (used by MST & DFS)
void Flight_route::adjacentlist(vector<Flight>& edges,
                                 vector<vector<tuple<int,int,double>>>& adj) {
    for(int i = 0; i < (int)adj.size(); i++) adj[i].clear();
    for(auto& p : edges) {
        adj[p.Origin_City].push_back({p.Destination_City, p.Cost, p.Time});
        adj[p.Destination_City].push_back({p.Origin_City,  p.Cost, p.Time});
    }
}

// FIX 2: Overload 2 — Build extended adjacency list from dataa list (used by findRoute)
void Flight_route::adjacentlist(vector<vector<tuple<vector<int>,int,int,double>>>& Final_conn,
                                 vector<dataa> adj) {
    for(int i = 0; i < (int)Final_conn.size(); i++) Final_conn[i].clear();
    for(auto& p : adj) {
        Final_conn[p.Origin_City].push_back({p.path, p.Destination_City, p.cost, p.time});
        Final_conn[p.Destination_City].push_back({p.path, p.Origin_City,  p.cost, p.time});
    }
}

// Lookup city name in Route[] and return its index (-1 if not found)
int Flight_route::getindex(const string& src) {
    for(int i = 0; i < (int)Route.size(); i++)
        if(Route[i] == src) return i;
    return -1;
}

// Kruskal's MST — sorts edges by cost, picks non-cycle edges
void Flight_route::kruskal() {
    Result.clear();
    rank_.assign(vertices, 0);
    initialise();

    sort(Flight_connection.begin(), Flight_connection.end(),
         [](const Flight& a, const Flight& b){ return a.Cost < b.Cost; });

    for(auto& p : Flight_connection) {
        int u = findparent(p.Origin_City);
        int v = findparent(p.Destination_City);
        if(u == v) continue;
        if     (rank_[v] < rank_[u]) parent[v] = u;
        else if(rank_[v] > rank_[u]) parent[u] = v;
        else { parent[v] = u; rank_[u]++; }
        Result.push_back(p);
        if((int)Result.size() == vertices - 1) break;
    }
}

//dijkstra now uses extended tuple graph <vector<int>,int,int,double>
RouteResult Flight_route::dijkstra(const string& src, const string& des,
                                    vector<vector<tuple<vector<int>,int,int,double>>>& graph,
                                    bool optimizeByCost) {
    RouteResult result;
    result.found = false;

    int n = graph.size();
    vector<int>    dist(n, INT_MAX);
    vector<double> tim(n, 1e18);
    vector<int>    par(n, -1);

    int s = getindex(src);
    int d = getindex(des);
    if(s == -1 || d == -1) return result;

    priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
    dist[s] = 0; tim[s] = 0;
    pq.push({0, s});

    while(!pq.empty()) {
        double value = pq.top().first;
        int    node  = pq.top().second;
        pq.pop();

        if( optimizeByCost && value > dist[node]) continue;
        if(!optimizeByCost && value > tim[node])  continue;

        for(auto& nb : graph[node]) {
            // FIX 2: correct get<> indices for <vector<int>, int, int, double>
            int    next = get<1>(nb);
            int    cost = get<2>(nb);
            double t    = get<3>(nb);

            if(optimizeByCost) {
                if(dist[next] > dist[node] + cost) {
                    dist[next] = dist[node] + cost;
                    tim[next]  = tim[node]  + t;
                    par[next]  = node;
                    pq.push({(double)dist[next], next});
                }
            } else {
                if(tim[next] > tim[node] + t) {
                    tim[next]  = tim[node]  + t;
                    dist[next] = dist[node] + cost;
                    par[next]  = node;
                    pq.push({tim[next], next});
                }
            }
        }
    }

    vector<int> path;
    for(int v = d; v != -1; v = par[v])
        path.push_back(v);
    reverse(path.begin(), path.end());

    result.path  = path;
    result.cost  = dist[d];
    result.time  = tim[d];
    result.found = true;
    return result;
}

// Add one flight edge to the system
void Flight_route::add_routes(Flight f) {
    Flight_connection.push_back(f);
}

// Resize all vectors when a new airport is added
void Flight_route::expand(int newSize) {
    vertices = newSize;
    Flight_connections.resize(newSize);
    Final_connections.resize(newSize);
    parent.resize(newSize);
    rank_.resize(newSize, 0);
    travelTime.resize(newSize, INT_MAX);  // FIX 6
}

//computeMST uses overload 1 of adjacentlist (Flight → simple graph)
void Flight_route::computeMST() {
    //adjacentlist(Flight_connection, Flight_connections);
    kruskal();
}

//  findRoute takes classRoutes, uses overload 2 of adjacentlist
RouteResult Flight_route::findRoute(const string& src, const string& des,
                                     vector<dataa> classRoutes, bool optimizeByCost) {
    Final_connections.assign(vertices, vector<tuple<vector<int>,int,int,double>>{});
    adjacentlist(Final_connections, classRoutes);
    return dijkstra(src, des, Final_connections, optimizeByCost);
}

// Public wrapper around private getindex()
int Flight_route::cityIndex(const string& name) {
    return getindex(name);
}

// Print all currently available airports
void Flight_route::available_airports() {
    cout << INFO << "Available Airports: " << RESET << endl;
    for(int i = 0; i < vertices; i++)
        cout << cityColor(Route[i]) << BOLD << Route[i] << RESET << endl;
}

// FIX 4: printAllRoutes uses overload 1 of adjacentlist (Flight → simple graph)
void Flight_route::printAllRoutes(string src, string dest) {
    newresult.clear();
    adjacentlist(Flight_connection, Flight_connections);
    int s = cityIndex(src);
    int d = cityIndex(dest);
    if(s == -1 || d == -1) return;

    vector<bool> visited(vertices, false);
    vector<int>  path;
    int routeCount = 0;

    cout << HEADER << "--------------------------------------------------------------\n" << RESET;
    cout << HEADER << "              ALL POSSIBLE ROUTES\n" << RESET;
    cout << HEADER << "--------------------------------------------------------------\n" << RESET;

    dfsUtil(s, d, visited, path, 0, 0.0, routeCount);
    if(routeCount == 0)
        cout << "No routes available.\n";
    cout << endl;
}

//dfsUtil uses correct get<> indices for <int,int,double>
void Flight_route::dfsUtil(int u, int dest, vector<bool>& visited,
                            vector<int>& path, int totalCost, double totalTime,
                            int& routeCount) {
    visited[u] = true;
    path.push_back(u);

    if(u == dest) {
        routeCount++;
        newresult.push_back({path.front(), dest, path, totalCost, totalTime});
    } else {
        for(auto& edge : Flight_connections[u]) {
            int    v    = get<0>(edge);   // FIX 8: correct index for int dest
            int    cost = get<1>(edge);   // FIX 8: correct index for int cost
            double t    = get<2>(edge);   // FIX 8: correct index for double time

            if(!visited[v]) {
                dfsUtil(v, dest, visited, path,
                        totalCost + cost,
                        totalTime + t,
                        routeCount);
            }
        }
    }

    path.pop_back();
    visited[u] = false;
}

void Flight_route::cancelTicket() {
    string ticketID;
    cout << "\n" << HEADER << "---------------------------------------------------------------\n" << RESET;
    cout << HEADER << "                   CANCEL TICKET\n" << RESET;
    cout << HEADER << "---------------------------------------------------------------\n" << RESET;
    cout << INFO << "Choose The Reason:\n" << RESET;
    cout << WARNING << "  1) Personal Reason\n" << RESET;
    cout << "\033[38;5;208m" << "  2) Medical\n" << RESET;          // Orange
    cout << "\033[1;96m"     << "  3) Visa/Documentation Issues\n" << RESET; // Cyan
    cout << "\033[38;5;88m"  << "  4) Airline Related Issue\n" << RESET;    // Dark Red
    cout << INFO << "Enter option: " << RESET;
    int op; cin >> op;

    switch(op) {
    case 1: {
        cout << INFO << "\n  A) Change Of Plan\n  B) Family Emergency\n  C) Personal Commitment\n" << RESET;
        cout << INFO << "  Enter option: " << RESET;
        char ch; cin >> ch;
        switch(ch) {
        case 'A': cout << WARNING << "  Reason: Change Of Plan\n"<< RESET; break;
        case 'B': cout << WARNING << "  Reason: Family Emergency\n" << RESET; break;
        case 'C': cout << WARNING << "  Reason: Personal Commitment\n"<< RESET; break;
        default:  cout << ERROR   << "  Invalid option.\n"<< RESET; break;
        }
        break;
    }
    case 2: {
        flag = true;
        cout << "\033[38;5;208m" << "\n  A) Sudden Illness\n  B) Hospitalization\n  C) Doctor Advised Not To Travel\n" << RESET;
        cout << INFO << "  Enter option: " << RESET;
        char ch; cin >> ch;
        switch(ch) {
        case 'A': cout << "\033[38;5;208m" << "  Reason: Sudden Illness\n"<< RESET; break;
        case 'B': cout << "\033[38;5;208m" << "  Reason: Hospitalization\n"<< RESET; break;
        case 'C': cout << "\033[38;5;208m" << "  Reason: Doctor Advised Not To Travel\n"  << RESET; break;
        default:  cout << ERROR<< "  Invalid option.\n"<< RESET; break;
        }
        break;
    }
    case 3: {
        cout << "\033[1;96m" << "\n  A) Visa Rejected\n  B) Passport Issue\n  C) Incomplete Travel Documents\n" << RESET;
        cout << INFO << "  Enter option: " << RESET;
        char ch; cin >> ch;
        switch(ch) {
        case 'A': cout << "\033[1;96m" << "  Reason: Visa Rejected\n"<< RESET; break;
        case 'B': cout << "\033[1;96m" << "  Reason: Passport Issue\n"<< RESET; break;
        case 'C': cout << "\033[1;96m" << "  Reason: Incomplete Travel Documents\n"<< RESET; break;
        default:  cout << ERROR<< "  Invalid option.\n"<< RESET; break;
        }
        break;
    }
    case 4: {
        cout << "\033[38;5;88m" << "\n  A) Flight Delayed\n  B) Flight Rescheduled\n  C) Poor Previous Experience\n" << RESET;
        cout << INFO << "  Enter option: " << RESET;
        char ch; cin >> ch;
        switch(ch) {
        case 'A': cout << "\033[38;5;88m" << "  Reason: Flight Delayed\n"<< RESET; break;
        case 'B': cout << "\033[38;5;88m" << "  Reason: Flight Rescheduled\n"<< RESET; break;
        case 'C': cout << "\033[38;5;88m" << "  Reason: Poor Previous Experience\n" << RESET; break;
        default:  cout << ERROR            << "  Invalid option.\n"<< RESET; break;
        }
        break;
    }
    default: cout << ERROR << "  Invalid Option.\n" << RESET; break;
    }
    cout << INFO << "  Enter Ticket ID to cancel (e.g. AIR1001): " << RESET;
    cin  >> ticketID;

    for(auto& b : bookings) {
        if(b.ticketID == ticketID) {
            if(b.cancelled=="Cancelled") {
                cout << ERROR << "\n  Ticket " << ticketID << " has already been cancelled.\n\n" << RESET;
                return;
            }
            time_t now         = std::time(nullptr);   // FIX 6: use std::time explicitly
            double secsElapsed = difftime(now, b.bookingTime);
            double refundRate  = 0.0;
            string refundNote;
		if(flag){
		refundRate=1.0;
		refundNote="Cancelled With Medical Reason -100% refund";
		}
            else if(secsElapsed < 60) {
                refundRate = 0.90;
                refundNote = "Cancelled within 60 secs — 90% refund";
            } else if(secsElapsed <= 120) {
                refundRate = 0.70;
                refundNote = "Cancelled after 60 secs — 70% refund";
            } else {
                refundRate = 0.0;
                refundNote = "Cancelled after 2 min — No refund";
            }

            int refundAmount = (int)(b.totalCost * refundRate);
            cout << "\n" << HEADER << "  -------------------------------------------------------\n" << RESET;
            cout << INFO   << "  Ticket ID       : " << RESET << SUCCESS << b.ticketID      << "\n" << RESET;
            cout << INFO   << "  Passenger       : " << RESET << SUCCESS << b.passengerName << "\n" << RESET;
            cout << INFO   << "  Route           : " << RESET
                           << cityColor(b.origin)      << BOLD << b.origin      << RESET
                           << PLANE << "  \u2708  " << RESET
                           << cityColor(b.destination) << BOLD << b.destination << RESET << "\n";
            cout << INFO   << "  Amount Paid     : " << RESET << MONEY   << "Rs." << b.totalCost  << "\n" << RESET;
            cout << INFO   << "  Booked          : " << RESET << WARNING << secsElapsed << " secs ago\n" << RESET;
            cout << INFO   << "  Policy          : " << RESET;
            if(refundRate == 0.0) cout << ERROR   << refundNote << "\n" << RESET;
            else                  cout << SUCCESS << refundNote << "\n" << RESET;
            cout << INFO   << "  Refund Amount   : " << RESET << MONEY << "Rs." << refundAmount << "\n" << RESET;
            cout << HEADER << "  -------------------------------------------------------\n\n" << RESET;

            if(refundRate == 0.0)
                cout << ERROR   << "  No refund applicable. Do you still want to cancel? (y/n): " << RESET;
            else
                cout << SUCCESS << "  Confirm cancellation? (y/n): " << RESET;

            char ch; cin >> ch;
            if(ch == 'y' || ch == 'Y') {
                b.cancelled = "Cancelled";
                savebooking();
                cout << "\n";
                if(refundRate > 0.0) {
                    cout << SUCCESS << "  Ticket cancelled successfully!\n" << RESET;
                    cout << MONEY   << "  Refund of Rs." << refundAmount << " will be credited within 5-7 business days.\n" << RESET;
                } else {
                    cout << WARNING << "  Ticket cancelled. No refund as per policy.\n" << RESET;
                }
                cout << HEADER << "\n  -------------------------------------------------------\n\n" << RESET;
            } else {
                cout << INFO << "\n  Cancellation aborted. Your ticket remains active.\n\n" << RESET;
            }
            return;
        }
        
    }
    cout << ERROR << "\n Ticket ID '" << ticketID << "' not found. Please check and try again.\n\n" << RESET;
}

void Flight_route::seperating(const vector<dataa>& newresult,
                               vector<dataa>& economy, vector<dataa>& business) {
    economy.clear();
    business.clear();
    for(const auto& p : newresult) {
    int stops=(int)p.path.size()-2;
        if(stops>=1) economy.push_back(p);
        else     
        business.push_back(p);
    }
}

void Flight_route::printfinal(string origin, string destination, const vector<dataa>& result) {
    
    int idx = 1;
    for(const auto& p : result) {
        int c = 0;
        cout << INFO << "  " << setw(2) << setfill(' ') << idx++ << ". " << RESET
             << cityColor(Route[p.Origin_City]) << BOLD 
             << setw(4) << left << Route[p.Origin_City] << RESET
             << INFO << " : " << RESET
             << cityColor(Route[p.Destination_City]) << BOLD
             << setw(4) << left << Route[p.Destination_City] << RESET
             << INFO << "  |  " << RESET;

        // path with plane symbol
        for(auto q : p.path) {
            c++;
            cout << cityColor(Route[q]) << BOLD << Route[q] << RESET;
            if(c < (int)p.path.size())
                cout << PLANE << " \u2708 " << RESET;
        }

        // cost and time aligned
        cout << "\n" << INFO << "      "
     << MONEY   << "  Cost: Rs." << p.cost
     << "      "
     << WARNING << "  Time: " << p.time << "h"
     << RESET   << "\n\n";
    }
}
void Flight_route::savebooking(){
fstream file("booking_history.txt",ios::out|ios::trunc);
if(!file){
cout<<"No Booking history found!"<<endl;
}
for(auto &b:bookings){
file<<b.ticketID<<", ";
    file<<b.passengerName<<",";
    file<<b.origin<<",";
    file<<b.destination<<",";
    file<<b.totalCost<<",";
    file<<b.totalTime<<",";
}
file.close();

}
 RouteResult Flight_route::findshort(vector<dataa>& selected, int op) {
    RouteResult res;
    res.found = false;

    if(selected.empty()) return res;

    res.path  = selected[0].path;
    res.cost  = selected[0].cost;
    res.time  = selected[0].time;
    res.found = true;

    if(op == 1) {
        // based on minimum cost
        for(auto& p : selected) {
            if(p.cost < res.cost) {  
                res.path = p.path;
                res.cost = p.cost;  
                res.time = p.time;
            }
        }
    } else {
        // based on minimum time
        double minTime = selected[0].time;  
        for(auto& p : selected) {
            if(p.time < minTime) {         
                minTime  = p.time;
                res.path = p.path;
                res.cost = p.cost;          
                res.time = p.time;
            }
        }
    }
    return res;
}
void Flight_route::suggestion(string origin, string destination,
                               vector<dataa> classRoutes, int op) {
    int choice = 1, c = 0;
    RouteResult selected;
    selected.found = false;

    if(classRoutes.size() == 0) {
        cout << ERROR << "No Flights Found!\n" << RESET;
        return;
    }

    // Economy — size 1
    if(op == 1 && economy.size() == 1) {
        cout << HEADER << "---------------------------------------------------------------\n" << RESET;
        cout << HEADER << "        ONLY ONE ECONOMY FLIGHT AVAILABLE\n" << RESET;
        cout << HEADER << "---------------------------------------------------------------\n\n" << RESET;
        c = 0;
        for(auto q : economy[0].path) {
            c++;
            cout << cityColor(Route[q]) << BOLD << Route[q] << RESET;
            if(c < (int)economy[0].path.size()) cout << " \u2708 ";
        }
        cout << MONEY << "  Cost: " << economy[0].cost
             << WARNING << "  Time: " << economy[0].time << RESET << endl;
        selected.path  = economy[0].path;
        selected.cost  = economy[0].cost;
        selected.time  = economy[0].time;
        selected.found = true;
    }

 
    else if(op == 2 && business.size() == 1) {
        cout << HEADER << "---------------------------------------------------------------\n" << RESET;
        cout << HEADER << "        ONLY ONE BUSINESS FLIGHT AVAILABLE\n" << RESET;
        cout << HEADER << "---------------------------------------------------------------\n\n" << RESET;
        c = 0;
        for(auto q : business[0].path) {
            c++;
            cout << cityColor(Route[q]) << BOLD << Route[q] << RESET;
            if(c < (int)business[0].path.size()) cout << " \u2708 ";
        }
        cout << MONEY << "  Cost: " << business[0].cost
             << WARNING << "  Time: " << business[0].time << RESET << endl;
        selected.path  = business[0].path;
        selected.cost  = business[0].cost;
        selected.time  = business[0].time;
        selected.found = true;
    }

    else {
        cout << HEADER << "---------------------------------------------------------------\n" << RESET;
        cout << HEADER << "           SELECT OPTIMIZATION PREFERENCE\n" << RESET;
        cout << HEADER << "---------------------------------------------------------------\n\n" << RESET;
        cout << MONEY   << "1) Based on Minimum Cost\n" << RESET;
        cout << WARNING << "2) Based on Minimum Time\n" << RESET;
        cout << INFO    << "Enter choice (1 or 2): " << RESET;
        cin  >> choice;

        if(choice != 1 && choice != 2) {
            cout << ERROR << "\nInvalid choice.\n\n" << RESET;
            return;
        }
        selected = findshort(classRoutes, choice);
    }

    if(!selected.found) {
        cout << ERROR << "\nNo valid flights found.\n" << RESET;
        return;
    }

    cout << "\n";
    printRoute("OPTIMAL ROUTE", selected, 1);

    cout << SUCCESS << "Do you want to confirm booking? (y/n): " << RESET;
    char confirm; cin >> confirm;
    if(confirm != 'y' && confirm != 'Y') {
        cout << ERROR << "\nBooking cancelled.\n\n" <<RESET;
        return;
    }

    cin.ignore();
    string passenger;
    cout << "\n" << INFO << "Enter Passenger Name: " << RESET;
    getline(cin, passenger);

    string routeType = (op == 1) ? "Economy" : "Business";
    string ticketID  = "AIR" + to_string(Flight_route::ticketCounter++);

    cout << HEADER << "-----------------------------------------------------------------\n" << RESET;
    cout << HEADER << "             BOOKING CONFIRMATION\n" << RESET;
    cout << HEADER << "-----------------------------------------------------------------\n" << RESET;
    cout << INFO   << "  Passenger Name        : " << RESET << SUCCESS << passenger     << RESET << "\n";
    cout << INFO   << "  Booking Reference ID  : " << RESET << SUCCESS << ticketID      << RESET << "\n";
    cout << INFO   << "  Class                 : " << RESET << WARNING << routeType     << RESET << "\n";
    cout << INFO   << "  Total Amount Paid     : " << RESET << MONEY   << "Rs." << selected.cost  << RESET << "\n";
    cout << INFO   << "  Estimated Travel Time : " << RESET << WARNING << selected.time << " Hours" << RESET << "\n";
    cout << INFO   << "  Booking Status        : " << RESET << SUCCESS << "CONFIRMED\n" << RESET;
    cout << HEADER << "-----------------------------------------------------------------\n\n" << RESET;

    Booking b;
    b.ticketID      = ticketID;
    b.passengerName = passenger;
    b.origin        = origin;
    b.destination   = destination;
    b.totalCost     = selected.cost;
    b.totalTime     = selected.time;
    b.bookingTime   = std::time(nullptr);
    b.cancelled     = "Confirmed";
    bookings.push_back(b);
    savebooking();
}
