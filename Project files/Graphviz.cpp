#include "sky_airlines.h"
#include <set>
#include <map>

void generateGraphDOT(Flight_route& f, bool showMST) {
    string dotfile = showMST ? "mst_graph.dot"  : "airline_graph.dot";
    string pngfile = showMST ? "mst_graph.png"  : "airline_graph.png";
    string title   = showMST ? "MST - Minimum Cost Network (Kruskal)"
                             : "Sky Airlines - Full Route Network";

    ofstream dot(dotfile);
    if(!dot) { cout << ERROR << "  Cannot create .dot file.\n" << RESET; return; }

    dot << "graph SkyAirlines {\n";
    dot << "  label=\"" << title << "\";\n";
    dot << "  labelloc=t;\n";
    dot << "  fontsize=30;\n";
    dot << "  fontname=\"Arial Bold\";\n";
   // dot << "  fontcolor=\"white\";\n";
   dot << "    fontcolor=\"black\";\n";
   dot << "  bgcolor=\"#FFFFFF\";\n";
    dot << "  pad=2.0;\n";
    dot << "  splines=line;\n";          // straight lines — no curve overlap
    dot << "  overlap=false;\n";
    dot << "  sep=\"+80\";\n";           // very large node separation
    dot << "  esep=\"+40\";\n";

    // ── Nodes ─────────────────────────────────────────────────────
    dot << "\n  node [\n";
    dot << "    shape=circle,\n";
    dot << "    style=filled,\n";
    dot << "    fontcolor=\"white\",\n";
    dot << "    fontname=\"Arial Bold\",\n";
    dot << "    fontsize=20,\n";
    dot << "    width=1.5,\n";
    dot << "    height=1.5,\n";
    dot << "    penwidth=3,\n";
    dot << "    fixedsize=true\n";
    dot << "  ];\n";

    // ── Edges — larger font, label background ─────────────────────
    dot << "\n  edge [\n";
    dot << "    fontname=\"Arial Bold\",\n";
    dot << "    fontsize=20,\n";
   // dot << "    fontcolor=\"white\",\n";  // white label text
   dot << "    fontcolor=\"black\",\n";
    dot << "    penwidth=2.5\n";
    dot << "  ];\n\n";

    // ── City colors ───────────────────────────────────────────────
    map<string,string> cityFill = {
        {"BLR","#27AE60"}, {"DEL","#2980B9"}, {"HYD","#E67E22"}, {"BOM","#8E44AD"},
        {"MAA","#16A085"}, {"KOL","#C0392B"}, {"AMD","#D4AC0D"}, {"GOI","#00BCD4"}
    };
    map<string,string> cityBorder = {
        {"BLR","#1E8449"}, {"DEL","#1F618D"}, {"HYD","#CA6F1E"}, {"BOM","#6C3483"},
        {"MAA","#0E6655"}, {"KOL","#922B21"}, {"AMD","#B7950B"}, {"GOI","#0097A7"}
    };

    for(int i = 0; i < f.vertices; i++) {
        string city   = Route[i];
        string fill   = cityFill.count(city)   ? cityFill[city]   : "#4A9EDB";
        string border = cityBorder.count(city) ? cityBorder[city] : "#2C7AB5";
        dot << "  " << city
            << " [fillcolor=\"" << fill   << "\""
            << ", color=\""     << border << "\""
            << ", label=\""     << city   << "\"];\n";
    }
    dot << "\n";

    // ── Edges ─────────────────────────────────────────────────────
    vector<Flight>& edges = showMST ? f.Result : f.Flight_connection;
    set<pair<int,int>> printed;

    for(auto& fl : edges) {
        int u = fl.Origin_City, v = fl.Destination_City;
        if(printed.count({min(u,v), max(u,v)})) continue;
        printed.insert({min(u,v), max(u,v)});

        string edgeColor;
        int    penwidth;
        if     (fl.Cost >= 6000) { edgeColor="#E74C3C"; penwidth=4; }
        else if(fl.Cost >= 4000) { edgeColor="#F39C12"; penwidth=3; }
        else if(fl.Cost >= 3000) { edgeColor="#3498DB"; penwidth=2; }
        else                     { edgeColor="#2ECC71"; penwidth=2; }

dot << "  " << Route[u] << " -- " << Route[v]
    << " [label=\"Rs." << fl.Cost << "  |  " << fl.Time << "h\""
    << ", color=\"black\""
    << ", penwidth=" << penwidth
    << "];\n";
    }

    dot << "}\n";
    dot.close();

    cout << SUCCESS << "\n  .dot file created : " << dotfile << "\n" << RESET;

    // large canvas — 20x12 gives plenty of room for labels
    string cmd = "fdp -Tpng -Gsize=60,16\\! -Gdpi=400 "
                 + dotfile + " -o " + pngfile + " 2>&1";
    int ret = system(cmd.c_str());

    if(ret == 0) {
        cout  << pngfile << "\n" << RESET;
        cout << INFO    <<  pngfile << "\n" << RESET;
        system(("xdg-open " + pngfile + " 2>/dev/null &").c_str());
    } else {
        cout << WARNING << "\n  Graphviz not found. Install:\n"       << RESET;
        cout << INFO    << "  sudo apt-get install graphviz\n"        << RESET;
        cout << INFO    << "  Then run: fdp -Tpng " << dotfile
                        << " -o " << pngfile << "\n\n"                << RESET;
    }
    cout << "\n";
}


