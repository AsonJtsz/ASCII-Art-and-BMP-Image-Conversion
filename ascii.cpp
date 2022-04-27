#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "bmp.h" //	Simple .bmp library
#include <math.h>

#define MAX_SHADES 8

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

char shades[MAX_SHADES] = {'@', '#', '%', '*', '|', '-', '.', ' '};

#define SAFE_FREE(p)    \
    {                   \
        if (p)          \
        {               \
            free(p);    \
            (p) = NULL; \
        }               \
    }

float **createFloatArray(int m, int n)
{
    float *values = (float *)calloc(m * n, sizeof(float));
    float **rows = (float **)malloc(m * sizeof(float *));
    for (int i = 0; i < m; ++i)
    {
        rows[i] = values + i * n;
    }
    return rows;
}

unsigned char **createUnsignedCharArray(int m, int n)
{
    unsigned char *values = (unsigned char *)calloc(m * n, sizeof(unsigned char));
    unsigned char **rows = (unsigned char **)malloc(m * sizeof(unsigned char *));
    for (int i = 0; i < m; ++i)
    {
        rows[i] = values + i * n;
    }
    return rows;
}

void destroyArray(float **arr)
{
    free(*arr);
    free(arr);
}

float **resizedImage(float **inputArry, int inputWidth, int inputHeight, int width, int height)
{
    float **resizedArr = createFloatArray(height + 1, width + 1);
    float x_ratio = (inputWidth - 1) * 1.0f / (width - 1);
    float y_ratio = (inputHeight - 1) * 1.0f / (height - 1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int x_l = floor(x_ratio * j);
            int y_l = floor(y_ratio * i);
            int x_h = ceil(x_ratio * j);
            int y_h = ceil(y_ratio * i);

            float x_weight = x_ratio * j - x_l;
            float y_weight = y_ratio * i - y_l;

            float a = inputArry[y_l][x_l];
            float b = inputArry[y_l][x_h];
            float c = inputArry[y_h][x_l];
            float d = inputArry[y_h][x_h];

            float pixel = a * (1 - x_weight) * (1 - y_weight) + b * x_weight * (1 - y_weight) +
                          c * y_weight * (1 - x_weight) +
                          d * x_weight * y_weight;

            resizedArr[i][j] = pixel;
        }
    }
    return resizedArr;
}

unsigned char **resizedRGB(unsigned char **inputArry, int inputWidth, int inputHeight, int width, int height)
{
    unsigned char **resizedArr = createUnsignedCharArray(height + 1, width + 1);
    float x_ratio = (inputWidth - 1) * 1.0f / (width - 1);
    float y_ratio = (inputHeight - 1) * 1.0f / (height - 1);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            int x_l = floor(x_ratio * j);
            int y_l = floor(y_ratio * i);
            int x_h = ceil(x_ratio * j);
            int y_h = ceil(y_ratio * i);

            float x_weight = x_ratio * j - x_l;
            float y_weight = y_ratio * i - y_l;

            float a = inputArry[y_l][x_l];
            float b = inputArry[y_l][x_h];
            float c = inputArry[y_h][x_l];
            float d = inputArry[y_h][x_h];

            float pixel = a * (1 - x_weight) * (1 - y_weight) + b * x_weight * (1 - y_weight) +
                          c * y_weight * (1 - x_weight) +
                          d * x_weight * y_weight;

            resizedArr[i][j] = (unsigned char)pixel;
        }
    }
    return resizedArr;
}

int main(int argc, char **argv)
{
    int compress, color;
    int compressWidth, compressHeight;
    printf("do you want to compress the image? Type 0 or 1(0: No, 1: Yes)\n");
    scanf("%d", &compress);
    printf("do you want to color ascii? Type 0 or 1(0: No, 1: Yes)\n");
    scanf("%d", &color);
    if (compress == 1)
    {
        printf("enter scaled width and height: <width> <height>\n");
        scanf("%d %d", &compressWidth, &compressHeight);
    }
    //
    //	1. Open BMP file
    //
    Bitmap image_data(argv[1]);

    if (image_data.getData() == NULL)
    {
        printf("unable to load bmp image!\n");
        return -1;
    }

    //
    //	2. Obtain Luminance
    //
    int width = image_data.getWidth();
    int height = image_data.getHeight();
    float **arr = createFloatArray(height + 1, width + 1);

    unsigned char **arrR;
    unsigned char **arrG;
    unsigned char **arrB;
    if (color == 1)
    {
        arrR = createUnsignedCharArray(height + 1, width + 1);
        arrG = createUnsignedCharArray(height + 1, width + 1);
        arrB = createUnsignedCharArray(height + 1, width + 1);
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            unsigned char R, G, B;
            image_data.getColor(j, i, R, G, B);
            arr[i][j] = 0.299 * R + 0.587 * G + 0.114 * B;
            if (color == 1)
            {
                arrR[i][j] = R;
                arrG[i][j] = G;
                arrB[i][j] = B;
            }
        }
    }
    //
    //	3. Quantization & ascii mapping
    //
    if (compress == 1)
    {
        arr = resizedImage(arr, width - 1, height - 1, compressWidth, compressHeight);
        if (color == 1)
        {
            arrR = resizedRGB(arrR, width - 1, height - 1, compressWidth, compressHeight);
            arrG = resizedRGB(arrG, width - 1, height - 1, compressWidth, compressHeight);
            arrB = resizedRGB(arrB, width - 1, height - 1, compressWidth, compressHeight);
        }
        width = compressWidth;
        height = compressHeight;
    }
    char **ascii = new char *[height];
    for (int i = 0; i < height; i++)
    {
        ascii[i] = new char[width];
    }
    int quantize = 255 / MAX_SHADES;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int idx = arr[i][j] / (float)quantize;
            idx = MIN(idx, MAX_SHADES - 1);
            ascii[i][j] = shades[idx];
        }
    }

    //
    //  4. ASCII art txt file
    //
    FILE *fp = fopen(argv[2], "w");
    if (!fp)
        return -1;

    if (color != 1)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                fprintf(fp, "%c", ascii[i][j]);
            }
            fprintf(fp, "\n");
        }
    }
    else
    {
        fprintf(fp, "<!DOCTYPE thml>\n<html>\n<head>\n</head>\n");
        fprintf(fp, "<style>\nbody\n{font-family:Courier New;\n}\np.small{line-height:0.1em}\n</style>\n");
        fprintf(fp, "<body>\n");
        for (int i = 0; i < height; i++)
        {
            fprintf(fp, "<p class=\"small\">\n");
            for (int j = 0; j < width; j++)
            {
                fprintf(fp, "<a style=\"color:rgb(%d,%d,%d);\">%c", arrR[i][j], arrG[i][j], arrB[i][j], ascii[i][j]);
            }
            fprintf(fp, "</p>\n");
        }
        fputs("</body></html>", fp);
    }
    fclose(fp);

    return 0;
}