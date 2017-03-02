#include "TextureManager.h"
#include "FreeImage.h"
#include "Image.h"
#include "Texture2D.h"
#include <math.h>
#include <fstream>
#include <stdlib.h>
//#include "lodepng.h"
#include <stdio.h>
#include <cassert>

long int getWidthHeight(int pix1, int pix2) {
	long int num=0 ,rem;
	int n = 8;
	do {   //high byte only
		rem = pix2 % 2;
		num = num + rem * (long)(pow((float)2, (float)n));
		pix2 = pix2 /2;
		n++;
	} while(n <= 16);
	num = num + pix1;
	return(num);
}

//pointer to the image, once loaded
FIBITMAP *dib = 0;


TextureManager* TextureManager::s_textureManager = NULL;

TextureManager::TextureManager()
{
    
}

TextureManager* TextureManager::sharedInstance()
{
    if ( s_textureManager == NULL )
    {
        s_textureManager = new TextureManager();
    }
    return s_textureManager;
}

Texture* TextureManager::loadTexture(const string& fileName, E_TEXTURE_LOAD_FLAG flags)
{
    imageFileName = fileName;
    
    FreeImage_Initialise();
    
    size_t found = imageFileName.find(".");
    string extension = imageFileName.substr(found+1,imageFileName.size());
    
    size_t pos = imageFileName.find_last_of("//");
    if(pos != std::string::npos)
        m_Filename.assign(imageFileName.begin() + pos + 1, imageFileName.end());
    else
        m_Filename = imageFileName;
     
    if( !extension.compare("bmp") || !extension.compare("BMP") )
        loadBmp();
    //else if( !extension.compare("tga") || !extension.compare("TGA") )
    //	return loadTga();
    
    else
        loadFile();
//                loadFileUsingCG();
                            
    Texture* tex = NULL;
    
//			((Texture2D*)tex)->setImage(mipMapLevel,m_Image);
    
    bool bCacheImage = false;
    
    if( flags & ETLF_CACHE_IMAGE )
        bCacheImage = true;

    if ( flags & ETLF_GENERATE_MIPMAPS )
    {
                        
        tex = new Texture2D( m_Image->getWidth()  , m_Image->getHeight(), m_Image  );
    }
    else
    {
        tex = new Texture2D( m_Image->getWidth()  , m_Image->getHeight(), m_Image  );
    }
//			if ( flags & GENERATE_MIP_MAPS )
//			{
//                assert(1);
//				do
//				{
//					w /= 2;
//					h /= 2;
//					mipMapLevel++;
//					FIBITMAP *dibscaled = FreeImage_Rescale(dib,w,h,FILTER_BOX);
//					//retrieve the image data
//					BYTE* bits = FreeImage_GetBits(dibscaled);
//					bool bTransparent = FreeImage_IsTransparent(dibscaled);
//					int numChannelPerPixel = 3;
//					numChannelPerPixel = bTransparent ? 4 : 3;
//					
//					IMAGE_FORMAT imageFormat = EIF_RGB;
//					imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;
//					
//					Image *mipMapImage = new Image(w , h , imageFormat);
//					memcpy(mipMapImage->getData() , bits , w*h*numChannelPerPixel);
//					
//					tex->setMipMapMode(E_MIPMAP_MODE_MULTI_LEVEL);
////					((Texture2D*)tex)->setImage(mipMapLevel,mipMapImage);
//					FreeImage_Unload(dibscaled);
//				}
//				while( w !=1 );
//			}
    
    //Free FreeImage's copy of the data
//			if( extension.compare("bmp") )
        FreeImage_Unload(dib);
    
    return tex;
}



void TextureManager::saveFile(const char* fileName)
{
    
}

Texture* TextureManager::getTexture(const string& filepath, E_TEXTURE_LOAD_FLAG flags)
{
    imageFileName.clear();
    m_Image = NULL;
    
    Texture* tex = m_mapTextures.find(filepath)->second;
    if ( !tex )
    {
        tex = loadTexture(filepath, flags);
        if ( tex )
            m_mapTextures[filepath] = tex;
    }
    
    return tex;
}

/*void TextureManager::loadPNG()
{
    IMAGE_FORMAT imageFormat = EIF_RGB;
    bool bTransparent = true;
    imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;
    
    // Load file and decode image.
    std::vector<unsigned char> image;
    unsigned width, height;

    unsigned error = lodepng::decode(image, width, height, imageFileName.c_str());
    // If there's an error, display it.
    if(error != 0)
    {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }
    
    // Texture size must be power of two for the primitive OpenGL version this is written for. Find next power of two.
    size_t u2 = 1; while(u2 < width) u2 *= 2;
    size_t v2 = 1; while(v2 < height) v2 *= 2;
    
    // Make power of two version of the image.
    std::vector<unsigned char> image2(u2 * v2 * 4);
    for(size_t y = 0; y < height; y++)
        for(size_t x = 0; x < width; x++)
            for(size_t c = 0; c < 4; c++)
            {
                image2[4 * u2 * y + 4 * x + c] = image[4 * width * y + 4 * x + c];
            }
    m_Image = new Image(width , height , imageFormat);
    m_Image->SetImageData(image2);
    m_Image->flipVertical();

}
*/
void TextureManager::loadFile()
{
    //image format
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    //pointer to the image data
    BYTE* bits = 0;
    //image width and height
    unsigned int width(0), height(0);
    //check the file signature and deduce its format
    fif = FreeImage_GetFileType(imageFileName.c_str(), 0);
    //if still unknown, try to guess the file format from the file extension
    if(fif == FIF_UNKNOWN) 
        fif = FreeImage_GetFIFFromFilename(imageFileName.c_str());
    //if still unkown, return failure
    if(fif == FIF_UNKNOWN)
        return;
    //check that the plugin has reading capabilities and load the file
    if(FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, imageFileName.c_str());
    //if the image failed to load, return failure
    if(!dib)
        return;

    // Premultiply alpha to support particle system blending mode.
    FreeImage_PreMultiplyWithAlpha(dib);
    //retrieve the image data
    bits = FreeImage_GetBits(dib);
    //get the image width and height
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    
    bool bTransparent = true;//FreeImage_IsTransparent(dib);
    //if this somehow one of these failed (they shouldn't), return failure
    if((bits == 0) || (width == 0) || (height == 0))
        return;
    int numChannelPerPixel = 3;
    numChannelPerPixel = bTransparent ? 4 : 3;

    unsigned bpp = FreeImage_GetBPP(dib);

    BYTE* bits4444 = NULL;
    
    // Dont do 4444 compression for the BG for now.
    if ( m_Filename != "bg_race_human.png" && m_Filename != "fg_human_vignette.png")
    {
        bpp = 16;
        bits4444 = (BYTE*)ConvertToRGBA4444(bits, width*height*4);
        bits = bits4444;
    }

    //printf("%d%s%d%s%d%s" , bits[i]," ", bits[i+1], " ", bits[i+2], " ");
    IMAGE_FORMAT imageFormat = EIF_RGB;

    imageFormat = bTransparent ? EIF_RGBA : EIF_RGB;
                
    float bytesPerPixel = bpp / 8;
    
    // TODO: Fix this.
    {
        
        for(int i = 0;i<width*height*numChannelPerPixel;i+=numChannelPerPixel)
        {
            BYTE temp = bits[i];
            bits[i]   = bits[i+2];
            bits[i+2]   = temp;

        }
    }
    
    m_Image = new Image((unsigned)width , (unsigned)height , imageFormat);
    
    memcpy(m_Image->getData() , bits , width * height * bytesPerPixel );
    
    if(bits4444)
        delete[] bits4444;
    
}
        

void* TextureManager::ConvertToRGBA4444( void* src, // IN, pointer to source buffer
                           int cb)    // IN size of source buffer, in bytes
{

    // this code assumes that a long is 4 bytes and short is 2.
    //on some compilers this isnt true
    int i;
    // compute the actual number of pixel elements in the buffer.
    int cpel = cb/4;
    unsigned * psrc = (unsigned*)src;
    // create the RGBA4444 buffer
    unsigned short* pdst = (unsigned short*)malloc(cpel*2);
    // convert every pixel
    for(i=0;i<cpel; i++)
    {
        // read a source pixel
        unsigned int pel = psrc[i];
        // unpack the source data as 8 bit values
        unsigned b = pel & 0xff;
        unsigned g = (pel >> 8) & 0xff;
        unsigned r = (pel >> 16) & 0xff;
        unsigned a = (pel >> 24) & 0xff;
        //convert to 4 bit vales
        r >>= 4;
        g >>= 4;
        b >>= 4;
        a >>= 4;
        // and store
        pdst[i] = r << 12 | g << 8  | b << 4 | a;
    
    }
    return pdst;
}

void TextureManager::loadBmp()
{
    unsigned char pixel;
    int i, j , k=0 ,x, pixlow, pixhigh, pix3, pix4, BMok;
    ifstream inpaint( imageFileName.c_str(), ios::binary);
    if(!inpaint) {
        printf("File not opened / found\n");
        exit(1);
    }
    BMok = 0;
    inpaint >> pixel;
    if(pixel == 'B') BMok++;
    inpaint >> pixel;
    if(pixel == 'M') BMok++;
    if(BMok == 2) printf("is a valid bit map file\n");
    else exit(1);
    for(x=3; x<=54; x++) {
        inpaint >> pixel; //go to start of data after header stuff
        if(x == 19) pixlow = pixel; //low
        if(x == 20) pixhigh = pixel; //high
        if(x == 23) pix3 = pixel; //low
        if(x == 24) pix4 = pixel; //high
    }
    int width =  (int)getWidthHeight( pixlow, pixhigh);
    int height = (int)getWidthHeight( pix3, pix4);
    //cout << "\nx width = " << xw << "y height = " << yh << endl;
    unsigned char* data = new unsigned char[4*width*height];
    //xpack = ww % 4; // multiples of 4 bytes/x line
    for (j = 0; j < height; j++) {
        for (i = 0; i < width; i++) {
            data[k+3] = 255;
            data[k+2] = inpaint.get( );
            data[k+1] = inpaint.get( );
            data[k+0] = inpaint.get( );
            //cout<<data[k+3]<<" "<<data[k+2]<<" "<<data[k+1]<<" "<<data[k]<<endl;
            k += 4;
        }
        //if (xpack != 0) for (i=1; i<=xpack; i++) inpaint.get(pixel);
    }

    m_Image = new Image(width , height , EIF_RGBA);
    memcpy(m_Image->getData() , data , width*height*4);
    delete[] data;
}

void TextureManager::loadTga()
{
    unsigned char   TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
    unsigned char	TGAcompare[12];								// Used To Compare TGA Header
    unsigned char	header[6];									// First 6 Useful Bytes From The Header
    unsigned		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
    unsigned		imageSize;									// Used To Store The Image Size When Setting Aside Ram
    unsigned		temp;										// Temporary Variable

    FILE *file = fopen(imageFileName.c_str(), "rb");						// Open The TGA File

    if(	file==NULL ||										// Does File Even Exist?
        fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
        memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
        fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
    {
        if (file == NULL)									// Did The File Even Exist? *Added Jim Strong*
            return;									// Return False
        else
        {
            fclose(file);									// If Anything Failed, Close The File
            return;									// Return False
        }
    }

    float width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
    float height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)

    if(	width	<=0	||								// Is The Width Less Than Or Equal To Zero
        height	<=0	||								// Is The Height Less Than Or Equal To Zero
        (header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
    {
        fclose(file);										// If Anything Failed, Close The File
        return;										// Return False
    }

    unsigned char bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
    bytesPerPixel	= bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
    imageSize		= width*height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

    //texture->imageData=(osmUByte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data
    unsigned char* data = new unsigned char[imageSize];

    if(	data ==0 ||							// Does The Storage Memory Exist?
        fread(data, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
    {
        if(data!=0)							// Was Image Data Loaded
            free(data);						// If So, Release The Image Data

        fclose(file);										// Close The File
        return;										// Return False
    }

    for(unsigned i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
    {														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
        temp = data[i];							// Temporarily Store The Value At Image Data 'i'
        data[i] = data[i + 2];				// Set The 1st Byte To The Value Of The 3rd Byte
        data[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
    }

    fclose (file);											// Close The Fil
    
    m_Image = new Image(width , height , EIF_RGBA);
    memcpy(m_Image->getData() , data , imageSize);
    delete[] data;
}

void TextureManager::clearTextureCache()
{
    map<string,Texture*>::iterator it;
    for( it = m_mapTextures.begin(); it != m_mapTextures.end(); it++ )
    {
        Texture* pTexture = it->second;
        delete pTexture;
    }
    
    m_mapTextures.clear();
}

