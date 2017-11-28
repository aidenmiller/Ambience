/**
 *  @file       ColourConvert.cpp
 *  @author     CS 3307 - Team 13
 *  @date       11/27/2017
 *  @version    1.0
 *
 *  @brief      CS 3307, Colour converter for Hue Light Application.
 *
 *  @section    DESCRIPTION
 *
 *              This is a helper class used to convert colour values from RGB format
 *              to the XY format, or from XY to RGB format.
 */

#include "ColourConvert.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>

using namespace std;

/**
 *   @brief  RGB to XY converter
 *
 *   @param  r is the red component of the colour
 *   @param  g is the green component of the colour
 *   @param  b is the blue component of the colour
 */
struct xy *ColourConvert::rgb2xy(float r, float g, float b)
{

    //convert values 0-255 to between 0 and 1
    float red = r/255.0f;
    float green = g/255.0f;
    float blue = b/255.0f;

    // gamma correction to rgb values

    red = (red > 0.04045f) ? pow((red + 0.055f) / (1.0f + 0.055f), 2.4f) : (red / 12.92f);
    green = (green > 0.04045f) ? pow((green + 0.055f) / (1.0f + 0.055f), 2.4f) : (green / 12.92f);
    blue = (blue > 0.04045f) ? pow((blue + 0.055f) / (1.0f + 0.055f), 2.4f) : (blue / 12.92f);

    // convert the RGB values to xyz using wide rgb d65 conversion formula

    float X = red * 0.649926f + green * 0.103455f + blue * 0.197109f;
    float Y = red * 0.234327f + green * 0.743075f + blue * 0.022598f;
    float Z = red * 0.000000f + green * 0.053077f + blue * 1.035763f;

    // calculate the xy values from the xyz values

    float x = X / (X + Y + Z);
    float y = Y / (X + Y + Z);

    // TO DO : check for if the xy value lays in the color gamut triangle for the light
    // use the Y value of the XYZ as brightness


    struct xy *xyStruct = (struct xy*) malloc(sizeof(struct xy));

    xyStruct->brightness = Y;
    xyStruct->x = x;
    xyStruct->y = y;

    return xyStruct;
}

/**
 *   @brief  XY to RGB converter
 *
 *   @param  inputX is the X component of the colour
 *   @param  inputY is the Y component of the colour
 *   @param  brightness is the brightness value of the light
 */
struct rgb *ColourConvert::xy2rgb(float inputX, float inputY, float brightness)
{

    /*
    float z = 1.0 -inputX - inputY;
    float Y = brightness/255.0;
    float X = (Y/ inputY) * inputX;
    float Z = (Y / inputY) * z;

    float r = X * 1.612 - Y * 0.203 - Z * 0.302;
    float g = -X * 0.509 + Y * 1.412 + Z + 0.066;
    float b = X * 0.026 - Y * 0.072 + Z * 0.962;

    r = r<= 0.0031308 ? 12.92 * r : (1.0 + 0.055) * pow(r, (1.0/2.4)) - 0.055;
    g = g<= 0.0031308 ? 12.92 * g : (1.0 + 0.055) * pow(g, (1.0/2.4)) - 0.055;
    r = b<= 0.0031308 ? 12.92 * b : (1.0 + 0.055) * pow(b, (1.0/2.4)) - 0.055;

    float maxValue;

    if (r > g && r > b)
        maxValue = r;
    else if (g > r && g > b)
        maxValue = g;
    else
        maxValue = b;

    r /= maxValue;
    g /= maxValue;
    b /= maxValue;

    r = r*255;
    if (r<0) {
            r = 255;
    }
    g = g * 255;
    if (g < 0) {
            g = 255;
    }
    b = b * 255;
    if (b < 0) {
            b = 255;
    } */

    // CONVERT TO XYZ
    float x = inputX;
    float y = inputY;

    float z = 1.0f - x - y;
    float Y = brightness / 254.0f; // the given brightness level

    float X = (Y / y) * x;
    float Z = (Y / y) * z;

    // COnvert to rgb using wide rgb d65 conversion

    float r = X * 3.2410f - Y * 1.5374f - Z * 0.4986f;
    float g = -X * 0.9692f + Y * 1.8760f + Z * 0.0416f;
    float b = X * 0.0556f - Y * 0.2040f + Z * 1.0570f;

    //apply reverse gamma correction
    r = r <= 0.0031308f ? 12.92f * r : (1.0f + 0.055f) * pow(r, (1.0f / 2.4f)) - 0.055f;
    g = g <= 0.0031308f ? 12.92f * g : (1.0f + 0.055f) * pow(g, (1.0f / 2.4f)) - 0.055f;
    b = b <= 0.0031308f ? 12.92f * b : (1.0f + 0.055f) * pow(b, (1.0f / 2.4f)) - 0.055f;

    //convert to 0-255
    if (r > 0)
        r = (r * 255);
    else
        r = 0;

    if (g > 0)
        g = g * 255;
    else
        g = 0;

    if (b > 0)
        b = b * 255;
    else
        b = 0;

    if ( r > 255 )
        r = 255;
    if (g > 255)
        g = 255;
    if (b > 255)
        b = 255;


    // create pointer of rgb structure
    struct rgb *rgbStruct = (struct rgb*) malloc(sizeof(struct rgb));
    rgbStruct->r = r;
    rgbStruct->b = b;
    rgbStruct->g = g;

    return rgbStruct;
}
