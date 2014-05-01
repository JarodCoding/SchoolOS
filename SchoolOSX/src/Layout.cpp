#include "Layout.h"
#include "string.h"

Layout::Layout(string name)
{
    layoutname = name;
}

Layout::~Layout()
{
    free(tiles);
}

void Layout::save(){
    ofstream output;
    output.open(layoutname.c_str());
    if(!output.is_open())return;
    output << tileSize<<"\n";
    int i = 0;
    while(i < tileSize){
        output << tiles[i].getData()<<"\n";
        i++;
    }
    output.close();
}

void Layout::load(){
    string line;
    ifstream input(layoutname.c_str());
    if(!input.is_open())return;
    getline(input,line);
    tileSize =atoi(line.c_str());
    tiles= (Tile*) calloc(tileSize,sizeof(Tile));
    int i = 0;
    Tile *tmp;
    while(getline(input,line)&&i < tileSize){
        tmp = new Tile();
        (*tmp).load(line);
        tiles[i] = *tmp;
        i++;
        delete(tmp);
        tmp = 0;
        }

}
string Layout::getName(){
    return layoutname;
}
Tile *Layout::getTiles(){
    Tile *res = (Tile *)malloc(sizeof(Tile)*tileSize);
    memcpy(res, tiles, sizeof(Tile)*tileSize);
    return res;
}
int Layout::getSize(){
    return tileSize;
}
void Layout::applyChanges(Tile *newTiles,int size){
free(tiles);
tiles = (Tile*) malloc(sizeof(Tile)*size);
memcpy(tiles, newTiles, sizeof(Tile)*size);
}
