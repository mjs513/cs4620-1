/*
 * Color.h
 *
 *  Created on: 30/03/2010
 *      Author: user
 */

#ifndef COLOR_H_
#define COLOR_H_


#include <ostream>


struct Color
{
    union {
        struct { float r,g,b,a; };
        float v[4];
    };
    
    /// Creates black color (0,0,0,1).
    Color();
    
    /// Creates color with (r,g,b,1).
    Color(float r, float g, float b);
    
    /// Creates color with (r,g,b,a).
    Color(float r, float g, float b, float a);
    
    static const Color black();
    static const Color white();
    static const Color gray();
    static const Color blue();
    static const Color red();
    static const Color green();
    static const Color yellow();
    static const Color brown();
    static const Color orange();
    static const Color magenta();
    static const Color cyan();
    static const Color clear();
};

std::ostream& operator<<(std::ostream &out, const Color &c);


#endif /* COLOR_H_ */
