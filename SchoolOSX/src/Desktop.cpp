#include "Desktop.h"
#include "stdio.h"
#include "string.h"
using namespace std;

Desktop::Desktop(Monitor *PhysicalMonitors,int monitorCount,Layout standartLayout)
{
    monitors = (Monitor*) malloc(sizeof(Monitor)*monitorCount);
    memcpy(monitors,PhysicalMonitors,sizeof(Monitor)*monitorCount);
    monitorSize = monitorCount;
    applyLayout(standartLayout);
}

Desktop::~Desktop()
{
    free(tiles);
    free(monitors);
    delete(currentLayout);
}
void Desktop::applyLayout(Layout& layout){
    free(tiles);
    currentLayout = &layout;
    tileSize = layout.getSize();
    tiles = (Tile*)malloc(sizeof(Tile)*layout.getSize());
    memcpy(tiles, layout.getTiles(), sizeof(Tile)*layout.getSize());

}
Layout Desktop::getCurrentLayout(){
    return *currentLayout;
}
int Desktop::getMonitorArraySize(){
        return monitorSize;
    }
int Desktop::getTileArraySize(){
        return tileSize;
}
int Desktop::getWidth(){
        return width;
}
int Desktop::getHeight(){
        return height;
}
