//
//  FNTLoader.h
//  RTSGame
//
//  Created by Rajavanya on 30/05/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#ifndef __RTSGame__FNTLoader__
#define __RTSGame__FNTLoader__

#include <iostream>
#include <map>
#include "Texture.h"

typedef struct CharInfo
{
    unsigned short char_id;
    unsigned short x, y, width, height;
    int xoffset, yoffset;
    int xadvance;
    unsigned short page, chnl;
} CharInfo;

typedef map<unsigned short,CharInfo> CharInfoMap;

typedef struct Font
{
	unsigned short LineHeight;
	unsigned short Base;
	unsigned short Width, Height;
	unsigned short Pages;
    CharInfoMap MapCharInfo;
    Texture *texture;
} Font;


typedef std::map<std::string, Font> FontMap;

class FNTLoader
{
public:
    Font loadFont(std::string basename);    //pass in the name of FNT file without extension, this will except a PNG file with same base name

    static FNTLoader* Instance();
    
private:
    FNTLoader();
    
    void ParseFont(std::istream& Stream, Font& font);
    FontMap m_fontMap;

    static FNTLoader *loader;
};

#endif /* defined(__RTSGame__FNTLoader__) */