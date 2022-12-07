#ifndef STRUCT_H
#define STRUCT_H

typedef struct Line
{
    int xStart;
    int yStart;
    int xEnd;
    int yEnd;
} Line;

typedef struct Dot
{
    int X;
    int Y;
} Dot;

typedef struct Square
{
    Dot tl;
    Dot tr;
    Dot bl;
    Dot br;
} Square;

typedef struct Blob
{
    int length;
    Dot *dots;
} Blob;

#endif
