//
//  GameSettings.h
//  Optiks
//
//  Created by Prashant Sharma on 21/11/13.
//
//

#ifndef Optiks_GameSettings_h
#define Optiks_GameSettings_h

#define NUM_CHAPTERS 2
#define NUM_STAGES  10
#define NUM_LEVELS_PER_STAGE 12
#define MAX_SOURCES 20
#define RAY_COLOR  Color4f(1.0f,0.4f,0.0f,1.0f)
#define MAX_RAY_COUNT 400
#define RAY_SPEED 0.5f
#define BASIC_ELEMENT_IMAGE_SIZE 105.0f
#define BASIC_ELEMENT_SIZE (BASIC_ELEMENT_IMAGE_SIZE * 0.99f)
//#define RAY_WIDTH
#define MAX_PARTICLES 1000

struct LevelProgressionInfo
{
    LevelProgressionInfo()
    {
        bLocked = 1;
    }
    
    int bLocked;
};

struct StageProgressionInfo
{
    StageProgressionInfo()
    {
        bLocked = 1;
        numLevelsCleared = 0;
    }
    
    LevelProgressionInfo levelProgInfo[NUM_LEVELS_PER_STAGE];
    int bLocked;
    int numLevelsCleared;
};

struct ChapterProgressionInfo
{
    ChapterProgressionInfo()
    {
        bLocked = 1;
        numStagesCleared = 0;
    }
    
    StageProgressionInfo stageProgInfo[NUM_STAGES];
    int bLocked;
    int numStagesCleared;
};

struct GameProgressionInfo
{
    GameProgressionInfo()
    {
        
    }
    
    ChapterProgressionInfo chapterProgInfo[NUM_CHAPTERS];
};


#endif
