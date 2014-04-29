#ifndef LAYOUT
#define LAYOUT
#include "Tile.h"

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
        Tile *getTiles();
        void applyChanges(Tile *tiles,int size);
        int getSize();
    protected:
    private:
    string layoutname;
    Tile *tiles;
    int tileSize;
};

#endif
