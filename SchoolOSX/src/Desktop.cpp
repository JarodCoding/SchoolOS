#include "Desktop.h"
#include "stdio.h"
#include "string.h"
#include <algorithm>
#include "Layout.h"
using namespace std;
Desktop* desktop;
Desktop::Desktop(Monitor *PhysicalMonitors,int monitorCount,Layout standartLayout)
{
    monitors = (Monitor*) malloc(sizeof(Monitor)*monitorCount);
    memcpy(monitors,PhysicalMonitors,sizeof(Monitor)*monitorCount);
    monitorSize = monitorCount;
    applyLayout(standartLayout);
    desktop = this;
}
Desktop::Desktop(int width,int height){
    (*this).width = width;
    (*this).height = height;
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
uint16_t Desktop::getWidth(){
        return width;
}
uint16_t Desktop::getHeight(){
        return height;
}
uint16_t Desktop::resizeX(int index,uint16_t requestedX){
    requestedX  = (uint16_t) min(width-requestedX,(int)tiles[index].getWidth()); // Desktop size limit
    if(requestedX == tiles[index].getX()){
        return requestedX;
    }else if(requestedX < tiles[index].getX()){
        uint16_t Width = tiles[index].getWidth();
        uint16_t check = requestedX+Width;
        uint16_t newWidth;
        uint16_t Y = tiles[index].getY();
        uint16_t HEIGHT = tiles[index].getHeight();
        uint16_t X = requestedX;
        int i = 0;
        while(i < tileSize){
            if(i == index ){
                i++;
                continue;
            }
            if(tiles[i].getX() > X && tiles[i].getX() <= check&&((tiles[i].getY()>Y&&tiles[i].getY()<=Y+HEIGHT)||(tiles[i].getY()+tiles[i].getHeight()>Y&&tiles[i].getY()+tiles[i].getHeight()<=Y+HEIGHT))){
                newWidth = tiles[i].getWidth()-(check-tiles[i].getX())-1;
                newWidth = max((int)newWidth,20);
                tiles[i].setLocalWidth(newWidth);
                Width = (tiles[i].setX(check+1))-requestedX;
                Width = max((int)Width,20);
                tiles[index].setLocalWidth(Width);
                requestedX = tiles[i].getX()-Width;
                check = requestedX+Width;
                X = requestedX;
            }
            i++;
        }
            return requestedX;
    }else{
        uint16_t Width = tiles[index].getWidth();
        uint16_t X = requestedX;
        uint16_t check = Width+X;
        uint16_t newWidth;
        uint16_t Y = tiles[index].getY();
        uint16_t HEIGHT = tiles[index].getHeight();
        int i = 0;
        while(i < tileSize){
            if(i == index ){
                i++;
                continue;
            }
            if(tiles[i].getX()+tiles[i].getWidth() > X && tiles[i].getX() <= check&&((tiles[i].getY()>Y&&tiles[i].getY()<=Y+HEIGHT)||(tiles[i].getY()+tiles[i].getHeight()>Y&&tiles[i].getY()+tiles[i].getHeight()<=Y+HEIGHT))){
                newWidth = tiles[i].getWidth()-((tiles[i].getX()+tiles[i].getWidth())-check)-1;
                newWidth = max((int)newWidth,20);
                tiles[i].setLocalWidth(newWidth);
                requestedX = tiles[i].setX(max(requestedX-newWidth,0))+newWidth;
                X = requestedX;
                check = Width+X;
            }
            i++;
        }
         return requestedX;
    }

}
uint16_t Desktop::resizeY(int index,uint16_t requestedY){
    requestedY = min(height-requestedY,(int)tiles[index].getY());// Desktop size limit
    if(requestedY == tiles[index].getY()){
        return requestedY;
    }else if(requestedY < tiles[index].getY()){
        uint16_t HEIGHT = tiles[index].getHeight();
        uint16_t WIDTH = tiles[index].getWidth();
        uint16_t check = requestedY+HEIGHT;
        uint16_t newHeight;
        uint16_t Y = requestedY;
        uint16_t X = tiles[index].getX();
        int i = 0;
        while(i < tileSize){
            if(i == index ){
                i++;
                continue;
            }
        if(tiles[i].getY() > Y && tiles[i].getY() <= check&&((tiles[i].getX()>X&&tiles[i].getX()<=X+WIDTH)||(tiles[i].getX()+tiles[i].getWidth()>X&&tiles[i].getX()+tiles[i].getWidth()<=X+WIDTH))){
                newHeight = tiles[i].getHeight()-(check-tiles[i].getY())-1;
                newHeight = max((int)newHeight,20);
                tiles[i].setLocalHeight(newHeight);
                HEIGHT = (tiles[i].setY(check+1))-requestedY;
                HEIGHT = max((int)HEIGHT,20);
                tiles[index].setLocalHeight(HEIGHT);
                requestedY = tiles[i].getY()-HEIGHT;
                check = requestedY+HEIGHT;
                Y = requestedY;
            }
            i++;
        }
            return requestedY;
    }else{


    uint16_t WIDTH = tiles[index].getWidth();
    uint16_t HEIGHT = tiles[index].getHeight();
    uint16_t X = tiles[index].getX();
    uint16_t Y = requestedY;
    uint16_t check = requestedY+HEIGHT;
    uint16_t newHeight;
    int i = 0;
    while(i < tileSize){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles[i].getY() > Y && tiles[i].getY() <= check&&((tiles[i].getX()>X&&tiles[i].getX()<=X+WIDTH)||(tiles[i].getX()+tiles[i].getWidth()>X&&tiles[i].getX()+tiles[i].getWidth()<=X+WIDTH))){
            newHeight = tiles[i].getHeight()-(check-tiles[i].getY())-1;
            newHeight = max((int)newHeight,20);
            tiles[i].setLocalHeight(newHeight);
            requestedY = tiles[i].setY(check+1)-HEIGHT;
            Y = requestedY;
            check = Y+ HEIGHT;
        }
        i++;
    }
    return requestedY;
    }
}
uint16_t Desktop::resizeWidth(int index,uint16_t requestedWidth){
    requestedWidth = max((int)requestedWidth,20); // minimal Size: Tile size: 20
    requestedWidth = min(width-tiles[index].getX(),(int)requestedWidth); // Desktop size limit
    if(requestedWidth <= tiles[index].getWidth()){
        return requestedWidth;
    }
    uint16_t X = tiles[index].getX();
    uint16_t Y = tiles[index].getY();
    uint16_t HEIGHT = tiles[index].getHeight();

    uint16_t check = X+requestedWidth;
    uint16_t newWidth;
    int i = 0;
    while(i < tileSize){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles[i].getX() > X && tiles[i].getX() <= check&&((tiles[i].getY()>Y&&tiles[i].getY()<=Y+HEIGHT)||(tiles[i].getY()+tiles[i].getHeight()>Y&&tiles[i].getY()+tiles[i].getHeight()<=Y+HEIGHT))){
            newWidth = tiles[i].getWidth()-(check-tiles[i].getX())-1;
            newWidth = max((int)newWidth,20);
            tiles[i].setLocalWidth(newWidth);
            requestedWidth = tiles[i].setX(check+1)-X;
            check = X + requestedWidth;
        }
        i++;
    }
    return requestedWidth;


}
uint16_t Desktop::resizeHeight(int index,uint16_t requestedHeight){
    requestedHeight = max((int)requestedHeight,20); // minimal Size: Tile size: 20
    requestedHeight = min(height-tiles[index].getY(),(int)requestedHeight); // Desktop size limit
    if(requestedHeight <= tiles[index].getHeight()){
        return requestedHeight;
    }
    uint16_t WIDTH = tiles[index].getWidth();
    uint16_t X = tiles[index].getX();
    uint16_t Y = tiles[index].getY();
    uint16_t check = Y+requestedHeight;
    uint16_t newHeight;
    int i = 0;
    while(i < tileSize){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles[i].getY() > Y && tiles[i].getY() <= check&&((tiles[i].getX()>X&&tiles[i].getX()<=X+WIDTH)||(tiles[i].getX()+tiles[i].getWidth()>X&&tiles[i].getX()+tiles[i].getWidth()<=X+WIDTH))){
            newHeight = tiles[i].getHeight()-(check-tiles[i].getY())-1;
            newHeight = max((int)newHeight,20);
            tiles[i].setLocalHeight(newHeight);
            requestedHeight = tiles[i].setY(check+1)-Y;
            check = Y+requestedHeight;
        }
        i++;
    }
    return requestedHeight;
}
