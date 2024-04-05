#pragma once

class CRGBA
{
public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char A;

    CRGBA(unsigned char red, unsigned char green, unsigned blue);
    CRGBA(unsigned char red, unsigned char green, unsigned blue, unsigned char alpha);
    CRGBA(CRGBA const &rhs);
    CRGBA(unsigned int intValue);
    CRGBA();

    void Set(unsigned char red, unsigned char green, unsigned blue);
    void Set(unsigned char red, unsigned char green, unsigned blue, unsigned char alpha);
    void Set(CRGBA const &rhs);
    void Set(CRGBA const &rhs, unsigned char alpha);
    void Set(unsigned int intValue);

    CRGBA ToRGB() const;
    unsigned int ToInt() const;
    unsigned int ToIntARGB() const;

    void Invert();
    CRGBA Inverted() const;

    bool operator==(CRGBA const &rhs) const;
    CRGBA &operator=(CRGBA const &rhs);
};