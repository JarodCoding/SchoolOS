#ifndef DESKTOP
#define DESKTOP
#include "Monitor.h"
#include "stdint.h"
#include "Layout.h"
using namespace std;

class Desktop
{
    public:
        Desktop(Monitor *monitor,int monitorsize,Layout startLayout);
        virtual ~Desktop();
    void applyLayout(Layout& layout);
    Layout getCurrentLayout();
    Tile *getTiles();
    Monitor *getMonitors();
    int getMonitorArraySize();
    int getTileArraySize();
    int getWidth();
    int getHeight();


    protected:
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
