//
//  FNTLoader.cpp
//  RTSGame
//
//  Created by Rajavanya on 30/05/13.
//  Copyright (c) 2013 Sourcebits. All rights reserved.
//

#include "FNTLoader.h"
#include "FileLoader.h"
#include "FileUtil.h"
#include <fstream>
#include <sstream>
#include <assert.h>


FNTLoader* FNTLoader::loader = 0;

FNTLoader::FNTLoader()
{
    FileUtil f;
    
    //std::string fullpath = f.getAbsolutePath("/Users/prashantsharma/Desktop/Optiks/font_0.fnt");
    
    //std::string fullpath = string("/Users/prashantsharma/Desktop/Optiks/font_0.fnt");
    
    //std::ifstream filestream(fullpath.c_str());
    
    //Font font;
    
    //ParseFont(filestream, font);
}

FNTLoader* FNTLoader::Instance()
{
    if( !loader )
    {
        loader = new FNTLoader();
    }
    return loader;
}

Font FNTLoader::loadFont(std::string basename)
{
    Font f;
    
    FontMap::iterator it = m_fontMap.find(basename);
    if (it == m_fontMap.end())
    {
        string absPath = basename;
#ifdef IOS
#else
        absPath = "/Users/prashantsharma/Downloads/Backup_MacBookPro/Optiks/" +  basename;
#endif

        std::string fullfntfilepath = absPath, fullpngfilepath = absPath;
        
        Texture* pTexture = NULL;
        //now load both the fnt file and texture
#ifdef IOS
        FileUtil fu;
        fullfntfilepath = fu.getAbsolutePath(fullfntfilepath.append(".fnt"));
        fullpngfilepath = fu.getAbsolutePath(fullpngfilepath.append(".png"));
#else
        fullfntfilepath = string(fullfntfilepath.append(".fnt"));
        fullpngfilepath = string(fullpngfilepath.append(".png"));
        pTexture = TextureLoader::Instance()->getTexture(fullpngfilepath);
#endif
        
        //assert(texture);
        
        std::ifstream filestream(fullfntfilepath.c_str());
        ParseFont(filestream, f);
        f.texture = pTexture;
        
        m_fontMap[basename] = f;
        return f;
    }
    
    return it->second;
}


void FNTLoader::ParseFont(std::istream& Stream, Font& font)
{
    string Line;
    string Read, Key, Value;
    std::size_t i;
    
    while( !Stream.eof() )
	{
		std::stringstream LineStream;
		std::getline( Stream, Line );
		LineStream << Line;
        LineStream >> Read;
        
        if( Read == "common" )
		{
			//this holds common data
			while( !LineStream.eof() )
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );
                
				//assign the correct value
				Converter << Value;
				if( Key == "lineHeight" )
					Converter >> font.LineHeight;
				else if( Key == "base" )
					Converter >> font.Base;
				else if( Key == "scaleW" )
					Converter >> font.Width;
				else if( Key == "scaleH" )
					Converter >> font.Height;
				else if( Key == "pages" )
					Converter >> font.Pages;
			}
		}
        else if( Read == "char" )
		{
			//this is data for a specific char
			unsigned short CharID = 0;
            
			while( !LineStream.eof() )
			{
				std::stringstream Converter;
				LineStream >> Read;
				i = Read.find( '=' );
				Key = Read.substr( 0, i );
				Value = Read.substr( i + 1 );
                
				//assign the correct value
				Converter << Value;
				if( Key == "id" )
                {
					Converter >> CharID;
                    CharInfo charInfo;
                    charInfo.char_id = CharID;
                    font.MapCharInfo[CharID] = charInfo;
                }
				else if( Key == "x" )
					Converter >> font.MapCharInfo[CharID].x;
				else if( Key == "y" )
					Converter >> font.MapCharInfo[CharID].y;
				else if( Key == "width" )
					Converter >> font.MapCharInfo[CharID].width;
				else if( Key == "height" )
					Converter >> font.MapCharInfo[CharID].height;
				else if( Key == "xoffset" )
					Converter >> font.MapCharInfo[CharID].xoffset;
				else if( Key == "yoffset" )
					Converter >> font.MapCharInfo[CharID].yoffset;
				else if( Key == "xadvance" )
					Converter >> font.MapCharInfo[CharID].xadvance;
				else if( Key == "page" )
					Converter >> font.MapCharInfo[CharID].page;
			}
        }
    }
}