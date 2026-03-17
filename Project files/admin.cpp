#include "sky_airlines.h"

void runAdmin(Flight_route& f) {
    cout << "\n" << HEADER << "--------------------------------------------------------\n" << RESET;
    cout << HEADER << "             ADMINISTRATOR PANEL\n" << RESET;
    cout << HEADER << "--------------------------------------------------------\n" << RESET;
    cout << INFO   << "  1) View MST (Minimum Cost Network)\n" << RESET;
    cout << INFO   << "  2) Add New Airport\n" << RESET;
    cout << INFO   << "  3) View Booking History\n" << RESET;
    cout << INFO   << "  4) Export Network Graph (PNG Image)\n" << RESET;
    cout << INFO   << "Enter choice: " << RESET;

    int choice; cin >> choice;

    if     (choice == 1) viewMST(f);
    else if(choice == 2) addAirport(f);
    else if(choice == 3) view_bookings();
    else if(choice == 4) {
        cout << INFO << "\n  1) Full Network   2) MST Only\n" << RESET;
        cout << INFO << "  Enter choice: " << RESET;
        int g; cin >> g;
        if(g == 2) { f.computeMST();
         generateGraphDOT(f, true); }
        else         generateGraphDOT(f, false);
    }
    else cout << ERROR << "Invalid choice.\n\n" << RESET;
}
void view_bookings() {
    ifstream file("booking_history.txt");  
    if(!file) {
        cout << ERROR << "No bookings found!\n" << RESET;
        return;
    }

    string line;
    while(getline(file, line)) {    // reads one full line
        cout << line << endl;       // prints full line
    }
    file.close();
}
void viewMST(Flight_route& f) {
    f.computeMST();   // calls private kruskal() inside Flight_route

    cout << "\n" << HEADER << "---------------------------------------------------\n" << RESET;
    cout << HEADER << "  MST — MINIMUM COST NETWORK (Kruskal's Algorithm)\n" << RESET;
    cout << HEADER << "---------------------------------------------------\n\n" << RESET;

    int    totalCost = 0;
    double totalTime = 0.0;
    int    idx = 1;

    for(auto& p : f.Result) {
        cout << INFO    << "  Route " << idx++ << "  :  " << RESET
             << cityColor(Route[p.Origin_City])      << BOLD << Route[p.Origin_City]      << RESET
             << PLANE   << "  \u2708  " << RESET
             << cityColor(Route[p.Destination_City]) << BOLD << Route[p.Destination_City] << RESET
             << INFO    << "   |   " << RESET
             << MONEY   << "Cost: Rs." << p.Cost << RESET
             << INFO    << "   |   " << RESET
             << WARNING << "Time: " << p.Time << " hrs\n" << RESET;
        totalCost += p.Cost;
        totalTime += p.Time;
    }

    cout << "\n" << INFO << "  -----------------------------------------------------\n" << RESET;
    cout << INFO    << "  Total Routes : " << RESET << SUCCESS << f.Result.size() << " (minimum needed)\n" << RESET;
    cout << INFO    << "  Total Cost   : " << RESET << MONEY   << "Rs." << totalCost   << "\n" << RESET;
    cout << INFO    << "  Total Time   : " << RESET << WARNING << totalTime << " hrs\n" << RESET;
    cout << INFO    << "  -----------------------------------------------------\n\n" << RESET;
}

void addAirport(Flight_route& f) {
    string city;
    cout << INFO << "Enter new airport name: " << RESET;
    cin >> city;

    Route.push_back(city);
    f.expand(Route.size());

    int count;
    cout << INFO << "How many airports to connect with " << RESET << cityColor(city) << BOLD << city << RESET << ": ";
    cin >> count;

    for(int i = 0; i < count; i++) {
        f.available_airports();
        string neighbour;
        cout << INFO << "Enter city to connect: " << RESET;
        cin >> neighbour;
	
        int ni = f.cityIndex(neighbour);
        int ci = f.cityIndex(city);
        if(ni == -1 || ci == -1) {
            cout << ERROR << "Invalid city name. Try again.\n" << RESET;
            i--; continue;
        }

        int cost; double t;
        while(true) {
            cout << INFO << "Enter cost (Rs.) and time (hrs): " << RESET;
            cin >> cost >> t;

            if(cost <= 0) {
                cout << ERROR << "  [!] Cost must be greater than 0. Re-enter.\n" << RESET;
                continue;
            }
            if(t <= 0 || t > 24) {
                cout << ERROR << "  [!] Time must be between 0 and 24 hours. Re-enter.\n" << RESET;
                continue;
            }

            cout << INFO << "  Confirm: " << RESET  << cityColor(neighbour) << BOLD << neighbour << RESET << " \u2708 " << cityColor(city) << BOLD << city << RESET << MONEY   << "  Cost=Rs." << cost << RESET<< WARNING << "  Time=" << t << "hrs" << RESET<< INFO    << "  (y/n): " << RESET;
            char ok; cin >> ok;
            if(ok == 'y' || ok == 'Y') break;
            cout << WARNING << "  Re-enter values.\n" << RESET;
        }

        f.add_routes({ni, ci, cost, t});//adding new city to an existing airport infrastructure
    }

    cout << "\n" << SUCCESS << "Airport '" << city << "' added successfully.\n\n" << RESET;
}
