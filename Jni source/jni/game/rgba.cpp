#include "rgba.h"

CRGBA::CRGBA(unsigned char red, unsigned char green, unsigned blue)
{
    Set(red, green, blue, 255);
}

CRGBA::CRGBA(unsigned char red, unsigned char green, unsigned blue, unsigned char alpha)
{
    Set(red, green, blue, alpha);
}

CRGBA::CRGBA(CRGBA const &rhs)
{
    Set(rhs);
}

CRGBA::CRGBA(unsigned int intValue)
{
    Set(intValue);
}

CRGBA::CRGBA(){}

void CRGBA::Set(unsigned char red, unsigned char green, unsigned blue)
{
    R = red;
    G = green;
    B = blue;
}

void CRGBA::Set(unsigned char red, unsigned char green, unsigned blue, unsigned char alpha)
{
    Set(red, green, blue);
    A = alpha;
}  

void CRGBA::Set(CRGBA const &rhs)
{
    Set(rhs.R, rhs.G, rhs.B, rhs.A);
}

void CRGBA::Set(CRGBA const &rhs, unsigned char alpha)
{
    Set(rhs.R, rhs.G, rhs.B, alpha);
}

void CRGBA::Set(unsigned int intValue)
{
    R = (intValue >> 24) & 0xFF;
    G = (intValue >> 16) & 0xFF;
    B = (intValue >> 8) & 0xFF;
    A = intValue & 0xFF;
}

CRGBA CRGBA::ToRGB() const
{
    return CRGBA(R, G, B, 255);
}

unsigned int CRGBA::ToInt() const
{
    return A | (B << 8) | (G << 16) | (R << 24);
}

unsigned int CRGBA::ToIntARGB() const
{
    return B | (G << 8) | (R << 16) | (A << 24);
}

void CRGBA::Invert()
{
    Set(255 - R, 255 - G, 255 - B);
}

CRGBA CRGBA::Inverted() const
{
    CRGBA invertedColor = *this;
    invertedColor.Invert();
    return invertedColor;
}

bool CRGBA::operator==(CRGBA const &rhs) const
{

}

CRGBA &CRGBA::operator=(CRGBA const &rhs)
{
	this->A = rhs.A;
	this->B = rhs.B;
	this->G = rhs.G;
	this->R = rhs.R;
}