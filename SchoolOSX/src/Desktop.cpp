#include "Desktop.h"
#include "stdio.h"
#include "string.h"
#include <algorithm>
#include "Layout.h"
using namespace std;
Desktop* desktop;
        Desktop::Desktop(vector< tr1::shared_ptr<Monitor> > monitors,tr1::shared_ptr<Layout> startLayout)
{
    int scrno;
    // connect to the X Server
    connection = xcb_connect(NULL, &scrno);
    if (xcb_connection_has_error(connection))
    {
        perror("xcb_connect failed!");
        exit(1);
    }
    //find current Secreen
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(xcb_get_setup(connection));
    for (int i = 0; i < scrno; ++ i)
    {
        xcb_screen_next(&iter);
    }

    screen = iter.data;
    if (!screen)
    {
        fprintf (stderr, "SchoolOS: Current screen missing. Exiting :( \n");
        xcb_disconnect(connection);
        exit(1);
    }
    xcb_query_tree_reply_t *reply;
    xcb_query_pointer_reply_t *pointer;
    int i;
    int len;
    xcb_window_t *children;
    xcb_get_window_attributes_reply_t *attr;
    reply = xcb_query_tree_reply(connection,
                                 xcb_query_tree(connection, screen->root), 0);
    len = xcb_query_tree_children_length(reply);
    children = xcb_query_tree_children(reply);


    (*this).monitors = monitors;
    tiles = startLayout->getTiles();
    currentLayout = startLayout;
    desktop = this;
}
Desktop::Desktop(int width,int height){
    (*this).width = width;
    (*this).height = height;
}

Desktop::~Desktop()
{
    tiles.clear();
    int i = 0;
    while(i < monitors.size()){
        monitors.at(i).reset();
        i++;
    }
    monitors.clear();
}
void Desktop::applyLayout(tr1::shared_ptr<Layout> newLayout){
    tiles.clear();
    tiles = newLayout->getTiles();
    (*this).currentLayout.reset();
    currentLayout = newLayout;

}
tr1::shared_ptr<Layout> Desktop::getCurrentLayout(){
    return currentLayout;
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
        while(i < tiles.size()){
            if(i == index ){
                i++;
                continue;
            }
            if(tiles.at(i).getX() > X && tiles.at(i).getX() <= check&&((tiles.at(i).getY()>Y&&tiles.at(i).getY()<=Y+HEIGHT)||(tiles.at(i).getY()+tiles.at(i).getHeight()>Y&&tiles.at(i).getY()+tiles.at(i).getHeight()<=Y+HEIGHT))){
                newWidth = tiles.at(i).getWidth()-(check-tiles.at(i).getX())-1;
                newWidth = max((int)newWidth,20);
                tiles.at(i).setLocalWidth(newWidth);
                Width = (resizeX(i,check+1))-requestedX;
                Width = max((int)Width,20);
                tiles[index].setLocalWidth(Width);
                requestedX = tiles.at(i).getX()-Width;
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
        while(i < tiles.size()){
            if(i == index ){
                i++;
                continue;
            }
            if(tiles.at(i).getX()+tiles.at(i).getWidth() > X && tiles.at(i).getX() <= check&&((tiles.at(i).getY()>Y&&tiles.at(i).getY()<=Y+HEIGHT)||(tiles.at(i).getY()+tiles.at(i).getHeight()>Y&&tiles.at(i).getY()+tiles.at(i).getHeight()<=Y+HEIGHT))){
                newWidth = tiles.at(i).getWidth()-((tiles.at(i).getX()+tiles.at(i).getWidth())-check)-1;
                newWidth = max((int)newWidth,20);
                tiles.at(i).setLocalWidth(newWidth);
                requestedX = resizeX(i,max(requestedX-newWidth,0))+newWidth;
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
        while(i < tiles.size()){
            if(i == index ){
                i++;
                continue;
            }
        if(tiles.at(i).getY() > Y && tiles.at(i).getY() <= check&&((tiles.at(i).getX()>X&&tiles.at(i).getX()<=X+WIDTH)||(tiles.at(i).getX()+tiles.at(i).getWidth()>X&&tiles.at(i).getX()+tiles.at(i).getWidth()<=X+WIDTH))){
                newHeight = tiles.at(i).getHeight()-(check-tiles.at(i).getY())-1;
                newHeight = max((int)newHeight,20);
                tiles.at(i).setLocalHeight(newHeight);
                HEIGHT = (resizeY(i,check+1))-requestedY;
                HEIGHT = max((int)HEIGHT,20);
                tiles[index].setLocalHeight(HEIGHT);
                requestedY = tiles.at(i).getY()-HEIGHT;
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
    while(i < tiles.size()){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles.at(i).getY() > Y && tiles.at(i).getY() <= check&&((tiles.at(i).getX()>X&&tiles.at(i).getX()<=X+WIDTH)||(tiles.at(i).getX()+tiles.at(i).getWidth()>X&&tiles.at(i).getX()+tiles.at(i).getWidth()<=X+WIDTH))){
            newHeight = tiles.at(i).getHeight()-(check-tiles.at(i).getY())-1;
            newHeight = max((int)newHeight,20);
            tiles.at(i).setLocalHeight(newHeight);
            requestedY = resizeY(i,check+1)-HEIGHT;
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
    while(i < tiles.size()){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles.at(i).getX() > X && tiles.at(i).getX() <= check&&((tiles.at(i).getY()>Y&&tiles.at(i).getY()<=Y+HEIGHT)||(tiles.at(i).getY()+tiles.at(i).getHeight()>Y&&tiles.at(i).getY()+tiles.at(i).getHeight()<=Y+HEIGHT))){
            newWidth = tiles.at(i).getWidth()-(check-tiles.at(i).getX())-1;
            newWidth = max((int)newWidth,20);
            tiles.at(i).setLocalWidth(newWidth);
            requestedWidth = resizeX(i,check+1)-X;
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
    while(i < tiles.size()){
        if(i == index ){
            i++;
            continue;
        }
        if(tiles.at(i).getY() > Y && tiles.at(i).getY() <= check&&((tiles.at(i).getX()>X&&tiles.at(i).getX()<=X+WIDTH)||(tiles.at(i).getX()+tiles.at(i).getWidth()>X&&tiles.at(i).getX()+tiles.at(i).getWidth()<=X+WIDTH))){
            newHeight = tiles.at(i).getHeight()-(check-tiles.at(i).getY())-1;
            newHeight = max((int)newHeight,20);
            tiles.at(i).setLocalHeight(newHeight);
            requestedHeight = resizeY(i,check+1)-Y;
            check = Y+requestedHeight;
        }
        i++;
    }
    return requestedHeight;
}
void Desktop::removeTile(int index){
   tiles.erase(tiles.begin()+index);
}
void Desktop::addTile(Tile tile){



}

