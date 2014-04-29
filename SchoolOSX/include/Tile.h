#ifndef TILE
#define TILE
#include "Structures.h"
#include <string>
#include <stdlib.h>
#include "iostream"
using namespace std;
class Tile
{
    public:
        Tile();
        virtual ~Tile();
    void load(string data);
    string getData();
    protected:

    private:
    struct sizepos position;

};

#endif // TILE
