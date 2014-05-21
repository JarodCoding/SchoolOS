#ifndef DESKTOP
#define DESKTOP
#include "Monitor.h"
#include "stdint.h"
#include <vector>
#include "Structures.h"
#include <tr1/memory>
using namespace std;
class Tile;
class Layout;

class Desktop
{

    public:
        Desktop(vector< tr1::shared_ptr<Monitor> > monitors,tr1::shared_ptr<Layout> startLayout);
        Desktop(int width,int height);

        virtual ~Desktop();
    void applyLayout(tr1::shared_ptr<Layout> layout);
    tr1::shared_ptr<Layout> getCurrentLayout();
    vector<Tile> getTiles();
    vector< tr1::shared_ptr<Monitor> > getMonitors();
    void addTile(Tile tile);

    void removeTile(int index);
    uint16_t getWidth();
    uint16_t getHeight();
    uint16_t resizeX(int TileIndex,uint16_t requestedX);
    uint16_t resizeY(int TileIndex,uint16_t requestedY);
    uint16_t resizeWidth(int TileIndex,uint16_t requestedWidth);
    uint16_t resizeHeight(int TileIndex,uint16_t requestedHeight);


    private:
    uint16_t width;
    uint16_t height;
    vector< tr1::shared_ptr<Monitor> > monitors   ;
    vector< Tile >                        tiles   ;
    tr1::shared_ptr<Layout>       currentLayout   ;
    xcb_screen_t                  screen          ;

};

#endif
