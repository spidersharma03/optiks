#ifndef _FILE_LOADER_H
#define _FILE_LOADER_H

#include <string>
#include <map>

using namespace std;

// TODO : PNG Loader is not functioning. Make it work. JPG loader is fine. Check other formats also
class Image;
class Texture;

enum E_TEXTURE_LOAD_FLAG
{
    ETLF_NONE = 0x0,
    ETLF_GENERATE_MIPMAPS = 0x01,
    ETLF_CACHE_IMAGE = 0x02
};

class TextureManager
{
private:
    // Private constructor
    TextureManager();
    
    // Cache textures
    map<string,Texture*> m_mapTextures;
    
    static TextureManager* s_textureManager;
    Image *m_Image;
    // BMP Loader
    void loadBmp();
    void loadTga();
    void loadFile();
    void loadFileUsingCG();
    
    void* ConvertToRGBA4444( void* src, // IN, pointer to source buffer
                            int cb);    // IN size of source buffer, in bytes
    
    // Load PNG using png loader, this function doesnt use freeimage library.
    //void loadPNG();
    
    void saveFile(const char* fileName);
    string imageFileName;
    
    std::string m_Filename; // actual filename without path info.
    
    // Load texture from a file path.
    Texture* loadTexture(const string& fileName, E_TEXTURE_LOAD_FLAG flags = ETLF_NONE);

public:
    //static const int GENERATE_MIP_MAPS = 0x01;
    void clearTextureCache();
    
    // Shared instance of this singleton class.
    static TextureManager* sharedInstance();
    
    // Find cached texture if present or else crate a new texture.
    Texture* getTexture(const string& filepath, E_TEXTURE_LOAD_FLAG flag = ETLF_NONE );
};


#endif