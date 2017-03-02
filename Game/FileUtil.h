//
//  FileUtil.h
//  FireFly_OSX
//
//  Created by Arshad on 04/09/12.
//  Copyright (c) 2012 Sourcebits Technologies. All rights reserved.
//

#ifndef FireFly_OSX_FileUtil_h
#define FireFly_OSX_FileUtil_h

#include <string.h>

using namespace std;

class FileUtil
{
    
public:

    FileUtil();
    
    
    string getAbsolutePath( string relativePath );
    
    string getAbsolutePathX( string relativePath );
    
    string flattenFilename( string directory, string root = "/" ) ;
    
    string deletePathFromPath(string filename, int pathCount);
    
};

#endif
