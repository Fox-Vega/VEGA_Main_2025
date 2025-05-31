#include "Defense.h"

extern LINE line;

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
    }
}

void Defense::get_vector_Line(void)
{
    Dline.theata =line.get_azimuth();
    Dline.dist =(int)line.get_dist();
    Dline.detect=line.read();
}

