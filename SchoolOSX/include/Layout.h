#ifndef LAYOUT
#define LAYOUT
#include "Tile.h"
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "stdint.h"
#include "iostream"
using namespace std;
class Layout
{
    public:
        Layout(string name);
        virtual ~Layout();
        void save();
        void load();
        string getName();
        vector<Tile> getTiles();
        void applyChanges(vector<Tile> tiles);
    protected:
    private:
    string layoutname;
    vector<Tile> tiles;
};

#endif
