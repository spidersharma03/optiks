//
//  FileUtil.cpp
//  FireFly_OSX
//
//  Created by Arshad on 04/09/12.
//  Copyright (c) 2012 Sourcebits Technologies. All rights reserved.
//

#include <iostream>
#include "FileUtil.h"

//#ifdef OSX
//    #include <CoreFoundation/CoreFoundation.h>
//#endif

//#ifdef iOS
    #include <CoreFoundation/CoreFoundation.h>
//#endif  

#include "stdlib.h"

FileUtil::FileUtil()
{
    // Constructor
}

string FileUtil::getAbsolutePath( string relativePath )
{
    
//#ifdef OSX
    CFBundleRef mainBundle;     
    mainBundle = CFBundleGetMainBundle();
    
    if(mainBundle)
    {
        CFURLRef resourceURL;
        
        // Look for the resource in the main bundle by name and type. 
        resourceURL = CFBundleCopyResourceURL(mainBundle,CFStringCreateWithCString(NULL,relativePath.c_str(),kCFStringEncodingASCII),NULL,NULL);
        
        if(!resourceURL)
        {
            //TODO: Throw Exception
            return NULL;
        }
        
        char fileurl[4024];

        if(!CFURLGetFileSystemRepresentation(resourceURL,true,(UInt8*)fileurl,4024))
            
        {
            //TODO: Throw Exception
            return NULL;
        }
        else
        {
            string url = fileurl;
            return url;
        }        
    }
    
//#endif

#ifdef QT_CORE_LIB
    //TODO strip the prefix path in case relative
    //path is passed instead of only filename

    string prefix("../Resources/");
    relativePath.assign(prefix.append(relativePath));
#endif
    
    return relativePath;
    
}

string FileUtil::getAbsolutePathX(string filename)
{
#if defined(_IRR_WINDOWS_CE_PLATFORM_)
	return filename;
#elif defined(_IRR_WINDOWS_API_)
	char *p=0;
	char fpath[_MAX_PATH];
#if defined(_IRR_WCHAR_FILESYSTEM )
    p = _wfullpath(fpath, filename.c_str(), _MAX_PATH);
    core::stringw tmp(p);
    tmp.replace(L'\\', L'/');
#else
    p = _fullpath(fpath, filename.c_str(), _MAX_PATH);
    core::stringc tmp(p);
    tmp.replace('\\', '/');
#endif
	return tmp;
//#elif (defined(_IRR_POSIX_API_) || defined(_IRR_OSX_PLATFORM_))
#elif (defined(OSX) || defined(iOS))    
	char* p=0;
	char fpath[4096];
	fpath[0]=0;
	p = realpath(filename.c_str(), fpath);
	if (!p)
	{
		// content in fpath is unclear at this point
		if (!fpath[0]) // seems like fpath wasn't altered, use our best guess
		{
			string tmp(filename);
			return flattenFilename(tmp);
		}
		else
			return fpath;
	}
    string s(p);
	if (filename[filename.size()-1]=='/')
    {
        string s(p);
        s+="/";
		return s;
    }
	else
		return s;
#else
	return filename;
#endif
}

string FileUtil::flattenFilename(string directory, string root)
{
    for (int i=0; i<directory.size(); ++i)
        if (directory[i] == '\\')
            directory[i] = '/';
    
//	directory.replace('\\', '/');
    
//	if (directory.lastChar() != '/')
//		directory.append('/');
    
    const char a = '/';
    
    if ( directory[directory.size()-1] != '/' )
        directory = directory.append(&a);
    
	string dir;
	string subdir;
    
	int lastpos = 0;
	int pos = 0;
	bool lastWasRealDir=false;
    
	while ((pos = (int)directory.find('/', lastpos)) >= 0)
	{
		subdir = directory.substr(lastpos, pos - lastpos + 1);
        
		if (subdir == "../")
		{
			if (lastWasRealDir)
			{
				deletePathFromPath(dir, 2);
				lastWasRealDir=(dir.size()!=0);
			}
			else
			{
				dir.append(subdir);
				lastWasRealDir=false;
			}
		}
		else if (subdir == "/")
		{
			dir = root;
		}
		else if (subdir != "./" )
		{
			dir.append(subdir);
			lastWasRealDir=true;
		}
        
		lastpos = pos + 1;
	}
	directory = dir;
	return directory;
}

//! trim paths
string FileUtil::deletePathFromPath(string filename, int pathCount)
{
	// delete path from filename
	int i = (int)filename.size();
    
	// search for path separator or beginning
	while ( i>=0 )
	{
		if ( filename[i] == '/' || filename[i] == '\\' )
		{
			if ( --pathCount <= 0 )
				break;
		}
		--i;
	}
    
	if ( i>0 )
	{
		filename [ i + 1 ] = 0;
	}
	else
		filename="";
	return filename;
}
