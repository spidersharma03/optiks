//
//  AudioManager.cpp
//  Optiks
//
//  Created by Prashant on 21/04/14.
//
//

#include "AudioManager.h"

AudioManager* AudioManager::s_pAudioManager = NULL;


void AudioManager::loadSound( const char* soundFile )
{
    NSError *error;
    NSURL *url = [NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:soundFile] ofType:@"mp3"]];
    
    std::map<string,AVAudioPlayer*>::iterator it;
    it = m_mapAudioPlayers.find(soundFile);
    
    if( it == m_mapAudioPlayers.end() )
    {
        AVAudioPlayer *audioPlayer = [[AVAudioPlayer alloc]
                       initWithContentsOfURL:url
                       error:&error];
        if (error)
        {
            NSLog(@"Error in audioPlayer: %@",
                  [error localizedDescription]);
        } else {
            //audioPlayer.delegate = self;
            [audioPlayer prepareToPlay];
        }
    }
}

void AudioManager::playSound( const char* soundFile )
{
    std::map<string,AVAudioPlayer*>::iterator it;
    it = m_mapAudioPlayers.find(soundFile);
    
    if( it != m_mapAudioPlayers.end() )
    {
        [it->second play];
    }
}

void AudioManager::pauseSound( const char* soundFile )
{
    std::map<string,AVAudioPlayer*>::iterator it;
    it = m_mapAudioPlayers.find(soundFile);
    
    if( it != m_mapAudioPlayers.end() )
    {
        [it->second pause];
    }
}