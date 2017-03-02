//
//  AudioManager.h
//  Optiks
//
//  Created by Prashant on 21/04/14.
//
//

#ifndef __Optiks__AudioManager__
#define __Optiks__AudioManager__

#include <iostream>
#include <map>
#import <AVFoundation/AVFoundation.h>

using namespace std;

class AudioManager
{
public:
    
    AudioManager* getInstance()
    {
        if( !s_pAudioManager )
        {
            s_pAudioManager = new  AudioManager();
        }
        return s_pAudioManager;
    }
    
    void          loadSound( const char* soundFile );
    
    void          playSound( const char* soundFile );
    
    void          pauseSound( const char* soundFile );
    
    void          adjustVolume(int soundID);
    
private:
    
    static AudioManager* s_pAudioManager;
    
    AudioManager()
    {
        
    }
    
private:
    // Audioplayer for each sound.
    map<string, AVAudioPlayer*> m_mapAudioPlayers;
};

#endif /* defined(__Optiks__AudioManager__) */
