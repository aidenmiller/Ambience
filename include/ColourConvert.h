#ifndef COLOURCONVERT_H
#define COLOURCONVERT_H

struct xy {
    float x;
    float y;
    float brightness;
};


struct rgb {
    float r;
    float g;
    float b;
    float brightness;
};

class ColourConvert
{
    //static public methods
    public:
        static struct xy *rgb2xy(float red, float green, float blue);
        static struct rgb *xy2rgb(float x, float y, float brightness);

    protected:

    private:
};

#endif // COLOURCONVERT_H
