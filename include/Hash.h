#ifndef HASH_H
#define HASH_H

#include <string>

struct xy {
    float x;
    float y;
};


struct rgb {
    float r;
    float g;
    float b;
};

class Hash
{
    // static public methods
    public:
        static std::string sha256_hash(const std::string str);

};

#endif // HASH_H
