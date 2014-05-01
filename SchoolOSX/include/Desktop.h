#ifndef DESKTOP
#define DESKTOP
#include "Monitor.h"
#include "stdint.h"
#include "Structures.h"
using namespace std;
class Tile;
class Layout;

class Desktop
{

    public:
        Desktop(Monitor *monitor,int monitorsize,Layout startLayout);
        Desktop(int width,int height);

        virtual ~Desktop();
    void applyLayout(Layout& layout);
    Layout getCurrentLayout();
    Tile *getTiles();
    Monitor *getMonitors();
    int getMonitorArraySize();
    int getTileArraySize();
    uint16_t getWidth();
    uint16_t getHeight();
    void addTile(Tile &tile);
    uint16_t resizeX(int TileIndex,uint16_t requestedX);
    uint16_t resizeY(int TileIndex,uint16_t requestedY);
    uint16_t resizeWidth(int TileIndex,uint16_t requestedWidth);
    uint16_t resizeHeight(int TileIndex,uint16_t requestedHeight);


    private:
    uint16_t width;
    uint16_t height;
    Monitor *monitors;
    int monitorSize;
    Tile    *tiles;
    int tileSize;
    Layout  *currentLayout;


};

#endif
