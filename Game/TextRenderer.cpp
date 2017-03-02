//
//  TextRenderer.cpp
//  Optiks
//
//  Created by Prashant Sharma on 28/03/13.
//

#include "TextRenderer.h"
#include "FileLoader.h"
#include "Texture2D.h"
#include <GLUT/GLUT.h>
#include "Bitmap.h"

TextRenderer::TextRenderer( const char* fontFile, const char* textureFile )
{
    //m_TextureLoader = new TextureLoader(fontFile);
    //m_FontTexture = (Texture2D*)m_TextureLoader->getTexture();    
    Bitmap *image;
    
    image=new Bitmap();
    
    if(image==NULL) {
    }
    if (image->loadBMP(fontFile)) 
    {
        glGenTextures(1, &m_FontTextureName);
        
        glBindTexture(GL_TEXTURE_2D, m_FontTextureName);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->width, image->height, 0,
                     GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, image->data);
    }
    else {
    }
    
    if (image) {
        delete image;
    }
    
    buildFont();

}

void TextRenderer::print( int x, int y, const char* string, int set)
{
    return;
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if( set > 1 )
        set = 1;
    
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    glBindTexture( GL_TEXTURE_2D, m_FontTextureName );   // Select font texture
    //glDisable( GL_DEPTH_TEST );
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();     // Save the projection matrix
    glLoadIdentity();   // Reset the projection matrix
    // Set up an ortho screen
    glOrtho( 0, 1280, 0, 800,
            -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glPushMatrix();     // Save the modelview matrix
    glLoadIdentity();   // Reset the modelview matrix
    glTranslated( x, y, 0 );   // Position the text
    glScaled(2, 2, 2);
    glListBase( m_Base - 32 + ( 128 * set ) );   // Choose the font set
    // Write the text to the screen
    glCallLists( strlen( string ), GL_BYTE, string );
    glMatrixMode( GL_PROJECTION );
    glPopMatrix();   // Restore the previous projection matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();   // Restore the previous modelview matrix
    //glEnable( GL_DEPTH_TEST );
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_BLEND);
}

void TextRenderer::buildFont()
{
    float cx;
    float cy;
    int loop;
    
    m_Base = glGenLists( 256 );   // Create 256 display lists
    glActiveTexture(GL_TEXTURE0);
    glEnable(GL_TEXTURE_2D);
    glBindTexture( GL_TEXTURE_2D, m_FontTextureName );   // Select font texture
    for( loop = 0; loop < 256; loop++ )
    {
        // X position of current character
        cx = ( (float) ( loop % 16 ) ) / 16.0f;
        // Y position of current character
        cy = ( (float) ( loop / 16 ) ) / 16.0f;
        glNewList( m_Base + loop, GL_COMPILE );   // Start building a list
        glBegin( GL_QUADS );
        glTexCoord2f( cx, 1 - cy - 0.0625f );   // Texture coord, bottom left
        glVertex2i( 0, 0 );                     // Vertex coord, bottom left
        
        // Texture coord, bottom right
        glTexCoord2f( cx + 0.0625f, 1 - cy - 0.0625f );
        glVertex2i( 16, 0 );                    // Vertex coord, bottom right
        
        glTexCoord2f( cx + 0.0625f, 1 - cy );   // Texture coord, top right
        glVertex2i( 16, 16 );                   // Vertex coord, top right
        
        glTexCoord2f( cx, 1 - cy );             // Texture coord, top left
        glVertex2i( 0, 16 );                    // Vertex coord, top left
        glEnd();
        glTranslated( 10, 0, 0 );   // Move to the right of the character
        glEndList();
    }
}