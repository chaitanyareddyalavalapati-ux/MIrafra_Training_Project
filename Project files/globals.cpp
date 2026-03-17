#include "sky_airlines.h"

vector<string> Route = { "BLR","DEL","HYD","BOM","MAA","KOL","AMD","GOI" };
string cityColor(const string& city) {
    if(city == "BLR") return BLR_COLOR;
    if(city == "DEL") return DEL_COLOR;
    if(city == "HYD") return HYD_COLOR;
    if(city == "BOM") return BOM_COLOR;
    if(city == "MAA") return MAA_COLOR;
    if(city == "KOL") return KOL_COLOR;
    if(city == "AMD") return AMD_COLOR;
    if(city == "GOI") return GOI_COLOR;
    return RESET;
}
