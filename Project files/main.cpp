#include "sky_airlines.h"
void loadFlights(Flight_route& f) {  
    f.add_routes({3, 1, 6500, 2.2});   // BOM - DEL  
    f.add_routes({1, 0, 6000, 2.8});   // DEL - BLR  
    f.add_routes({3, 5, 6500, 2.2});   // BOM - KOL  
    f.add_routes({1, 5, 5500, 2.5});   // DEL - KOL  
    f.add_routes({0, 5, 6000, 3.0});   // BLR - KOL  
    f.add_routes({5, 2, 5500, 2.0});   // KOL - HYD  
    f.add_routes({1, 6, 4000, 1.6});   // DEL - AMD  
    f.add_routes({3, 2, 4000, 1.3});   // BOM - HYD  
    f.add_routes({3, 0, 4000, 1.5});   // BOM - BLR  

    f.add_routes({3, 6, 3000, 1.0});   // BOM - AMD  
    f.add_routes({3, 7, 3000, 1.0});   // BOM - GOI  
    f.add_routes({0, 2, 2800, 1.2});   // BLR - HYD  
    f.add_routes({0, 4, 2200, 1.0});   // BLR - MAA   
    f.add_routes({4, 2, 3200, 1.3});   // MAA - HYD  
}
int main() {
    Flight_route f(Route.size());
    loadFlights(f);

    while(1) {
        cout << HEADER << "-----------------------------------------------------\n" << RESET;
        cout << HEADER << "  1) Administrator   2) User   3) Exit\n" << RESET;
        cout << HEADER << "-----------------------------------------------------\n" << RESET;
        cout << INFO   << "Enter option: " << RESET;
        int op; 
        cin >> op;
        switch(op) {
            case 1: runAdmin(f); break;
            case 2: runUser(f);  break;
            case 3: cout << "\033[1;96m" << "  \u2708  Thank you for choosing" << RESET;
                cout<<BLR_COLOR<<" Mirafra Airlines"<<RESET<<endl;;
                cout << "\033[1;33m"<< "  We wish you safe travels and " << RESET;
                cout << "\033[1;96m"<< "blue skies" << RESET;
                cout << "\033[1;33m"<< " ahead.\n" << RESET;
                exit(0);
            default:
                cout << ERROR << "Invalid option.\n\n" << RESET;
                break;
        }
    }
}
