#ifndef _FILE_LOADER_H
#define _FILE_LOADER_H

#include <string>
#include <map>

using namespace std;


class Image;
class Texture;


		class TextureLoader
		{
		private:
			static TextureLoader* m_pTextureLoader;
                        
            map<string, Texture*> m_TextureMap;
            
            Image* loadFile(const string& fileName);

            static const int GENERATE_MIP_MAPS = 0x01;

            TextureLoader();

		public:
            static TextureLoader* Instance();            

			Texture* getTexture(const string& fileName);
		};


#endif