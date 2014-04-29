#include "Tile.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include "stdint.h"
#include <sstream>
using namespace std;

Tile::Tile()
{
    //ctor
}

Tile::~Tile()
{
    //dtor
}
void Tile::load(string data){
    vector<string> res;
    using namespace boost::algorithm;
    split(res, data, is_any_of(";"));
    using namespace std;
    position.x = atoi(res.at(0).c_str());
    position.y = atoi(res.at(1).c_str());
    position.width = atoi(res.at(2).c_str());
    position.height = atoi(res.at(3).c_str());


}
string Tile::getData(){
ostringstream s;
s <<position.x<<";"<<position.y<<";"<<position.width<<";"<<position.height;
return s.str();
}
