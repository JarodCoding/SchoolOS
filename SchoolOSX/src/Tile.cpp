#include "Tile.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "stdint.h"
#include <sstream>
#include "Desktop.h"

Tile::Tile()
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}
void Tile::load(string data){
    vector< string> res;
    using namespace boost::algorithm;
    split(res, data, is_any_of(";"));
    using namespace std;
    position.x = atoi(res.at(0).c_str());
    position.y = atoi(res.at(1).c_str());
    position.width = atoi(res.at(2).c_str());
    position.height = atoi(res.at(3).c_str());
    TileIndex = atoi(res.at(4).c_str());


}


string Tile::getData(){
ostringstream s;
s <<position.x<<";"<<position.y<<";"<<position.width<<";"<<position.height<<";"<<TileIndex;
return s.str();
}
void Tile::setIndex(int index){
TileIndex = index;
}
int Tile::getIndex(){
return TileIndex;
}
void Tile::setLocalX(uint16_t x){
position.x = x;
}
void Tile::setLocalY(uint16_t y){
position.y = y;
}
void Tile::setLocalWidth(uint16_t width){
position.width = width;
}
void Tile::setLocalHeight(uint16_t height){
position.height = height;
}
uint16_t Tile::setX(uint16_t x){

    setLocalX((*desktop).resizeX(getIndex(),x));
}
uint16_t Tile::setY(uint16_t y){
    setLocalY((*desktop).resizeY(getIndex(),y));

}
uint16_t Tile::setWidth(uint16_t width){
        setLocalWidth((*desktop).resizeWidth(getIndex(),width));

}
uint16_t Tile::setHeight(uint16_t height){
        setLocalHeight((*desktop).resizeHeight(getIndex(),height));

}
uint16_t Tile::getX(){
    return position.x;
}
uint16_t Tile::getY(){
    return position.y;
}
uint16_t Tile::getWidth(){
    return position.width;
}
uint16_t Tile::getHeight(){
    return position.height;
}

