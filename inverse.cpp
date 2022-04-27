#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "bmp.h" //	Simple .bmp library

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@', '#', '%', '*', '|', '-', '.', ' '};

#define SAFE_FREE(p)    \
    {                   \
        if (p)          \
        {               \
            free(p);    \
            (p) = NULL; \
        }               \
    }

int GetShadesIndex(char c)
{
    int returnVal = -1;

    for (int i = 0; i < MAX_SHADES; i++)
    {
        if (c == shades[i])
        {
            returnVal = i;
            break;
        }
    }
    return returnVal;
}

int main(int argc, char **argv)
{
    //
    // 1. Open the txt file
    //
    FILE *fp = fopen(argv[1], "r");
    if (!fp)
        return -1;

    // 2. read the ascii art file.
    //
    int width, height;
    fscanf(fp, "%d %d", &width, &height);

    int **arr = new int *[height];
    for (int i = 0; i < height; i++)
    {
        arr[i] = new int[width];
    }

    int iCoord = 0, jCoord = 0;
    while (iCoord < height && jCoord < width)
    {
        int ch = fgetc(fp);
        int shadesIndex = GetShadesIndex(ch);
        if (shadesIndex != -1)
        {
            arr[iCoord][jCoord] = shadesIndex;
            jCoord += 1;
            iCoord += jCoord / width;
            jCoord = jCoord % width;
        }
    }

    //
    // 3. Get RGB values for the resultant bitmap
    //
    Bitmap RGB = Bitmap(width, height);
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            unsigned char color = arr[x][y] * (255 / (MAX_SHADES - 1));
            RGB.setColor(y, x, color, color, color);
        }
    }

    //
    // 4. Store the BMP file to the given place
    //
    RGB.save(argv[2]);
    // free memory if any

    for (int i = 0; i < height; i++)
        SAFE_FREE(arr[i]);
    SAFE_FREE(arr);

    return 0;
}