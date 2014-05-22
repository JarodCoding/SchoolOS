#include "Layout.h"
#include "string.h"
#include "Constants.h"
Layout::Layout(string name)
{
    layoutname = name;
    tiles = vector<Tile>();
}

Layout::~Layout()
{
    tiles.clear();
}

void Layout::save(){
    ofstream output;
    output.open((Constants::Path+"Layouts/"+layoutname+".lay").c_str());
    if(!output.is_open())return;
    int i = 0;
    while(i < tiles.size()){
        output << tiles.at(i).getData()<<"\n";
        i++;
    }
    output.close();
}

void Layout::load(){
    tiles.clear();
    string line;
    ifstream input((Constants::Path+"Layouts/"+layoutname+".lay").c_str());
    if(!input.is_open())return;
    int i = 0;
    Tile *tmp;
    while(getline(input,line)&&i < tiles.size()){

        tmp = new Tile();
        (*tmp).load(line);
        tiles.push_back(*tmp);
        delete(tmp);
        tmp = 0;


        i++;

        }

}
string Layout::getName(){
    return layoutname;
}
vector<Tile> Layout::getTiles(){
    return tiles;
}
void Layout::applyChanges(vector<Tile> newTiles){
    tiles.clear();
    int i = 0;
    while(i < newTiles.size()){
        if(newTiles.at(i).isHidden()){
            i++;
            continue;
        }
        tiles.push_back(newTiles.at(i));
        i++;
    }


}
