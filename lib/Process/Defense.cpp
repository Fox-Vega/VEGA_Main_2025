#include "Defense.h"

// extern LINE line;

data Dball;
data Dline;

void Defense::setup(void)
{
}

void Defense::defense_(void)
{
    general.setup();
    while(true)
    {
        get_vector();
    }
}

void Defense::get_vector_Line(void)
{
    Dline.theata = line.get_azimuth();
    Dline.dist = line.get_magnitude();
    Dline.detect = (Dline.dist) ? false : true; // ラインが検出されていない場合はfalse
    Dline.x = Dline.dist * cos(radians(Dline.theata));
    Dline.y = Dline.dist * sin(radians(Dline.theata));
}

void Defense::get_vector_Ball(void)
{
    Dball.theata = ball.get_azimuth();
    Dball.dist = (int)ball.get_magnitude();
    Dball.detect = ((int)ball.get_magnitude() > 0) ? true : false;
    myvector.get_cord(Dball.theata, Dball.dist);
    Dball.x = myvector.get_x();
    Dball.y = myvector.get_y();
}