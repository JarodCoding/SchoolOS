#ifndef TILE
#define TILE
#include "Structures.h"
#include <string>
#include <stdlib.h>
#include "iostream"
#include "Data.h"
#include <xcb/xcb_util.h>

using namespace std;

class Tile
{
    public:
        Tile();
        virtual ~Tile();
    void load(string data);
    string getData();
    uint16_t setX(uint16_t x);
    uint16_t setY(uint16_t y);
    uint16_t setWidth(uint16_t width);
    uint16_t setHeight(uint16_t height);
    uint16_t getX();
    uint16_t getY();
    uint16_t getWidth();
    uint16_t getHeight();
    int getIndex();
    void setIndex(int index);
    void setLocalX(uint16_t x);
    void setLocalY(uint16_t y);
    void setLocalWidth(uint16_t width);
    void setLocalHeight(uint16_t height);
    void setHidden(bool hidden);
    bool isHidden();
    void focus();
    private:
    struct sizepos position;
    int TileIndex;
    xcb_window_t runtime;
    bool hidden;
};

#endif // TILE
