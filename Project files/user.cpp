#include "sky_airlines.h"
//#include<ifstream>
void printRoute(const string& label, RouteResult& r, int num) {
    cout << HEADER << "Option " << num << ": " << label << RESET << "\n";
    cout << INFO << " -----------------------------------------------------\n" << RESET;
    cout << INFO << " Route : " << RESET;
    for(int i = 0; i < (int)r.path.size(); i++) {
        cout << cityColor(Route[r.path[i]]) << BOLD << Route[r.path[i]] << RESET;
        if(i < (int)r.path.size() - 1)
            cout << PLANE << " \u2708 " << RESET;
    }
    int stops = (int)r.path.size() - 2;
    cout << "\n" << INFO << " Stops : " << RESET
         << (stops <= 0 ? SUCCESS : WARNING)
         << (stops <= 0 ? "Non-stop" : to_string(stops) + " stop(s)") << RESET;
    cout << "\n" << INFO << " Cost  : " << RESET << MONEY << "Rs." << r.cost << RESET;
    cout << "\n" << INFO << " Time  : " << RESET << WARNING << r.time << " Hours" << RESET << "\n";
    cout << INFO << " -----------------------------------------------------\n\n" << RESET;
}

void view_my_booking() {
    string name;
    cout <<MAGENTA<< BOLD<<"Enter your Name: "<<RESET;
    cin.ignore();
    getline(cin, name);

    ifstream file("booking_history.txt");
    string line;
    vector<string> matchedLines;  // store all matching lines
    while(getline(file, line)) {
        if(line.find(name) != string::npos) {
            matchedLines.push_back(line);
        }
    }
    file.close();

    if(matchedLines.size() == 0) {
        cout << MAGENTA<<BOLD<<"No bookings found for: " << name << endl;
        return;
    }

    if(matchedLines.size() == 1) {
        cout << "\nBooking Details:\n";
        cout <<MAGENTA<<BOLD<< matchedLines[0] <<RESET<< endl;
        return;
    }

    // Case 3 — Multiple bookings found for same name
    if(matchedLines.size() > 1) {
        cout << MAGENTA<<BOLD<<"\nMultiple bookings found for: " << name << RESET<<endl;

        // Show all matches first
        for(auto booking : matchedLines) {
            cout << booking << endl;
        }

        // Step 2 — Ask for Ticket ID to narrow down
        cout << MAGENTA<<BOLD<<"\nEnter your Ticket ID to get exact booking: "<<RESET;
        string ticketID;
        cin >> ticketID;

        bool found = false;
        for(auto booking : matchedLines) {
            if(booking.find(ticketID) != string::npos) {
                cout <<MAGENTA<<BOLD<<"\nYour Exact Booking:\n"<<RESET;
                cout << booking << endl;
                found = true;
                break;  
            }
        }

        if(!found) {
            cout << "No booking found for Ticket ID: " 
                 << ticketID << endl;
        }
    }
}

void bookFlight(Flight_route& f)
{
    cout << "\n\033[1;96m  \u2708  Welcome to";
    cout << BLR_COLOR << " MIRAFRA Airlines" << RESET;
    cout << "\033[1;96m  — Your Journey Begins Here!\n" << RESET;
    cout << INFO   << "  Mirafra Airline Booking System\n" << RESET;
    cout << HEADER << "--------------------------------------------------------------\n" << RESET;
    cout << HEADER << "              SEARCH FLIGHTS\n" << RESET;
    cout << HEADER << "--------------------------------------------------------------\n" << RESET;

    f.available_airports();
    cout << "\n";

    string origin, destination;
    cout << INFO << "Enter Origin City      : " << RESET;
    cin  >> origin;
    cout << INFO << "Enter Destination City : " << RESET;
    cin  >> destination;
	if(origin==destination){
	cout<<"Invalid Option"<<endl;
	return;
	}
    if(f.cityIndex(origin) == -1 || f.cityIndex(destination) == -1) {
        cout << ERROR << "\nInvalid city code. Please choose from:\n" << RESET;
        f.available_airports();
        return;
    }

    cout << "\n" << INFO << "Select Class:\n" << RESET;
    cout << "\033[1;33m" <<"1.Economy\n  2.Business\n"<<RESET;
    cout << INFO << "Enter option: " << RESET;
    int op;
    cin >> op;

    f.printAllRoutes(origin, destination);//storing all routes in newresult vector
    f.seperating(f.newresult, f.economy, f.business);//separating based on economy type/business type

    if(op == 1) {
        cout << SUCCESS << "\nEconomy Class All Possible Routes:\n" << RESET;
        f.printfinal(origin, destination, f.economy);
        f.suggestion(origin, destination, f.economy,op);
    } else if(op == 2) {
        cout << SUCCESS << "\nBusiness Class All PossibleRoutes:\n" << RESET;
        f.printfinal(origin, destination, f.business);
        f.suggestion(origin, destination, f.business,op);
    } else {
        cout << ERROR << "Invalid class selection.\n" << RESET;
    }
}  // FIX 1: this closing brace properly ends bookFlight

// User menu: Book or Cancel
void runUser(Flight_route& f)
{
    cout << "\n" << HEADER << "---------------------------------------------------------------\n" << RESET;
    cout << HEADER << "                     USER PANEL\n" << RESET;
    cout << HEADER << "---------------------------------------------------------------\n" << RESET;
    cout << INFO   << "  1) Book a Flight\n"   << RESET;
    cout << INFO   << "  2) Cancel a Ticket\n" << RESET;
     cout << INFO   << "  3) View My Bookings\n" << RESET;
    cout << INFO   << "  Enter choice: "        << RESET;

    int opt; cin >> opt;

    if     (opt == 1) bookFlight(f);
    else if(opt == 2) f.cancelTicket();
    else if(opt==3) view_my_booking();
    else cout << ERROR << "Invalid choice.\n\n" << RESET;
}
