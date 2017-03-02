//
//  SZGLTextureLoader.m
//  Texture Loading Sample
//
//  Created by numata on 09/08/16.
//  Copyright 2009 Satoshi Numata. All rights reserved.
//

#import "SZGLTextureLoader.h"

#import <AppKit/AppKit.h>
//#import <AppKit/AppKit.h>

#pragma mark -
#pragma mark Texture Loading Implementation by Apple

typedef enum {
	kTexture2DPixelFormat_Automatic = 0,
	kTexture2DPixelFormat_RGBA8888,
	kTexture2DPixelFormat_RGBA4444,
	kTexture2DPixelFormat_RGBA5551,
	kTexture2DPixelFormat_RGB565,
	kTexture2DPixelFormat_RGB888,
	kTexture2DPixelFormat_L8,
	kTexture2DPixelFormat_A8,
	kTexture2DPixelFormat_LA88,
} Texture2DPixelFormat;

static GLuint _GLLoadTextureFromData(const void *data, Texture2DPixelFormat pixelFormat, NSUInteger width, NSUInteger height)
{
    GLuint  _name;
	GLint   saveName;
	
    glGenTextures(1, &_name);
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &saveName);
    glBindTexture(GL_TEXTURE_2D, _name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    switch (pixelFormat) {
        case kTexture2DPixelFormat_RGBA8888:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
            
        case kTexture2DPixelFormat_RGBA4444:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, data);
            break;
            
        case kTexture2DPixelFormat_RGBA5551:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_5_5_5_1, data);
            break;
            
        case kTexture2DPixelFormat_RGB565:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, data);
            break;
            
        case kTexture2DPixelFormat_RGB888:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
            
        case kTexture2DPixelFormat_L8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
            break;
            
        case kTexture2DPixelFormat_A8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
            break;
            
        case kTexture2DPixelFormat_LA88:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, data);
            break;
            
        default:
            [NSException raise:NSInternalInconsistencyException format:@""];
            
    }
    glBindTexture(GL_TEXTURE_2D, saveName);
    
    GLenum error = glGetError();
    if (error) {
        NSLog(@"Texture Loader: OpenGL error 0x%04X", error);
        return GL_INVALID_VALUE;
    }
    
    //imageSize_->width = width * contentSize.width / (float)width;
//    imageSize_->height = height * contentSize.height / (float)height;
//    
//    textureSize_->width = contentSize.width / (float)width;
//    textureSize_->height = contentSize.height / (float)height;
    
    return _name;
}

static GLuint _GLLoadTextureFromCGImage(CGImageRef imageRef)
{
    NSUInteger				width;
    NSUInteger              height;
    NSUInteger              i;
	CGContextRef			context = nil;
	void*					data = nil;;
	CGColorSpaceRef			colorSpace;
	void*					tempData;
	unsigned char*			inPixel8;
	unsigned int*			inPixel32;
	unsigned char*			outPixel8;
	unsigned short*			outPixel16;
	BOOL					hasAlpha;
	CGImageAlphaInfo		info;
	CGAffineTransform		transform;
	CGSize					imageSize;
	
	if (imageRef == NULL) {
        return GL_INVALID_VALUE;
	}
    
    Texture2DPixelFormat pixelFormat = kTexture2DPixelFormat_Automatic;
    BOOL sizeToFit = NO;
	
	if (pixelFormat == kTexture2DPixelFormat_Automatic) {
		info = CGImageGetAlphaInfo(imageRef);
		hasAlpha = ((info == kCGImageAlphaPremultipliedLast) || (info == kCGImageAlphaPremultipliedFirst) || (info == kCGImageAlphaLast) || (info == kCGImageAlphaFirst) ? YES : NO);
		if (CGImageGetColorSpace(imageRef)) {
			if (CGColorSpaceGetModel(CGImageGetColorSpace(imageRef)) == kCGColorSpaceModelMonochrome) {
				if (hasAlpha) {
					pixelFormat = kTexture2DPixelFormat_LA88;
				}
				else {
					pixelFormat = kTexture2DPixelFormat_L8;
				}
			}
			else {
                //int bpp = CGImageGetBitsPerPixel(imageRef);
				if((CGImageGetBitsPerPixel(imageRef) == 16) && !hasAlpha)
                    pixelFormat = kTexture2DPixelFormat_RGBA5551;
				else {
					if(hasAlpha)
                        pixelFormat = kTexture2DPixelFormat_RGBA8888;
					else {
						pixelFormat = kTexture2DPixelFormat_RGB565;
					}
				}
			}		
		}
		else { //NOTE: No colorspace means a mask image
			pixelFormat = kTexture2DPixelFormat_A8;
		}
	}
	
	imageSize = CGSizeMake(CGImageGetWidth(imageRef), CGImageGetHeight(imageRef));
	transform = CGAffineTransformIdentity;

		
	width = imageSize.width;
	if ((width != 1) && (width & (width - 1))) {
		i = 1;
		while ((sizeToFit ? 2 * i : i) < width) {
            i *= 2;
        }
		width = i;
	}
	height = imageSize.height;
	if ((height != 1) && (height & (height - 1))) {
		i = 1;
		while ((sizeToFit ? 2 * i : i) < height)
            i *= 2;
		height = i;
	}
    
	switch (pixelFormat) {
		case kTexture2DPixelFormat_RGBA8888:
		case kTexture2DPixelFormat_RGBA4444:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = malloc(height * width * 4);
            context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
            CGColorSpaceRelease(colorSpace);
            break;
            
		case kTexture2DPixelFormat_RGBA5551:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = malloc(height * width * 2);
            context = CGBitmapContextCreate(data, width, height, 5, 2 * width, colorSpace, kCGImageAlphaNoneSkipFirst | kCGBitmapByteOrder16Little);
            CGColorSpaceRelease(colorSpace);
            break;
            
		case kTexture2DPixelFormat_RGB888:
		case kTexture2DPixelFormat_RGB565:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = malloc(height * width * 4);
            context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaNoneSkipLast | kCGBitmapByteOrder32Big);
            CGColorSpaceRelease(colorSpace);
            break;
            
		case kTexture2DPixelFormat_L8:
            colorSpace = CGColorSpaceCreateDeviceGray();
            data = malloc(height * width);
            context = CGBitmapContextCreate(data, width, height, 8, width, colorSpace, kCGImageAlphaNone);
            CGColorSpaceRelease(colorSpace);
            break;
            
		case kTexture2DPixelFormat_A8:
            data = malloc(height * width);
            context = CGBitmapContextCreate(data, width, height, 8, width, NULL, kCGImageAlphaOnly);
            break;
            
		case kTexture2DPixelFormat_LA88:
            colorSpace = CGColorSpaceCreateDeviceRGB();
            data = malloc(height * width * 4);
            context = CGBitmapContextCreate(data, width, height, 8, 4 * width, colorSpace, kCGImageAlphaPremultipliedLast | kCGBitmapByteOrder32Big);
            CGColorSpaceRelease(colorSpace);
            break;
            
		default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid pixel format"];
            
	}
	if (context == NULL) {
		NSLog(@"Texture Loader: Failed creating CGBitmapContext.");
		free(data);
		return GL_INVALID_VALUE;
	}
	
	if(sizeToFit)
        CGContextScaleCTM(context, (CGFloat)width / imageSize.width, (CGFloat)height / imageSize.height);
	else {
		CGContextClearRect(context, CGRectMake(0, 0, width, height));
		CGContextTranslateCTM(context, 0, height - imageSize.height);
	}
	if (!CGAffineTransformIsIdentity(transform)) {
        CGContextConcatCTM(context, transform);
    }
	CGContextDrawImage(context, CGRectMake(0, 0, CGImageGetWidth(imageRef), CGImageGetHeight(imageRef)), imageRef);
	
	//Convert "-RRRRRGGGGGBBBBB" to "RRRRRGGGGGBBBBBA"
	if (pixelFormat == kTexture2DPixelFormat_RGBA5551) {
		outPixel16 = (unsigned short*)data;
		for (i = 0; i < width * height; ++i, ++outPixel16) {
            *outPixel16 = *outPixel16 << 1 | 0x0001;
        }
	}
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRRRRRGGGGGGGGBBBBBBBB"
	else if (pixelFormat == kTexture2DPixelFormat_RGB888) {
		tempData = malloc(height * width * 3);
		inPixel8 = (unsigned char*)data;
		outPixel8 = (unsigned char*)tempData;
		for(i = 0; i < width * height; ++i) {
			*outPixel8++ = *inPixel8++;
			*outPixel8++ = *inPixel8++;
			*outPixel8++ = *inPixel8++;
			inPixel8++;
		}
		free(data);
		data = tempData;
	}
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGGGBBBBB"
	else if (pixelFormat == kTexture2DPixelFormat_RGB565) {
		tempData = malloc(height * width * 2);
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;
		for(i = 0; i < width * height; ++i, ++inPixel32) {
            *outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 3) << 11) | ((((*inPixel32 >> 8) & 0xFF) >> 2) << 5) | ((((*inPixel32 >> 16) & 0xFF) >> 3) << 0);
        }
		free(data);
		data = tempData;
	}
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "RRRRRGGGGBBBBAAAA"
	else if(pixelFormat == kTexture2DPixelFormat_RGBA4444) {
		tempData = malloc(height * width * 2);
		inPixel32 = (unsigned int*)data;
		outPixel16 = (unsigned short*)tempData;
		for (i = 0; i < width * height; ++i, ++inPixel32) {
            *outPixel16++ = ((((*inPixel32 >> 0) & 0xFF) >> 4) << 12) | ((((*inPixel32 >> 8) & 0xFF) >> 4) << 8) | ((((*inPixel32 >> 16) & 0xFF) >> 4) << 4) | ((((*inPixel32 >> 24) & 0xFF) >> 4) << 0);
        }
		free(data);
		data = tempData;
	}
	//Convert "RRRRRRRRRGGGGGGGGBBBBBBBBAAAAAAAA" to "LLLLLLLLAAAAAAAA"
	else if (pixelFormat == kTexture2DPixelFormat_LA88) {
		tempData = malloc(height * width * 3);
		inPixel8 = (unsigned char*)data;
		outPixel8 = (unsigned char*)tempData;
		for (i = 0; i < width * height; i++) {
			*outPixel8++ = *inPixel8++;
			inPixel8 += 2;
			*outPixel8++ = *inPixel8++;
		}
		free(data);
		data = tempData;
	}
    else if (pixelFormat == kTexture2DPixelFormat_RGBA8888) {
        inPixel8 = (unsigned char *)data;
        unsigned char max = 0;
		for (i = 0; i < width * height; i++) {
            float alpha = (float)*(inPixel8+3) / 0xff;
            if (alpha > 0.0f && alpha < 1.0f) {
                *(inPixel8) = (unsigned char)((float)*(inPixel8) / alpha);
                if (*(inPixel8) > max) {
                    max = *(inPixel8);
                }
                inPixel8++;
                *(inPixel8) = (unsigned char)((float)*(inPixel8) / alpha);
                if (*(inPixel8) > max) {
                    max = *(inPixel8);
                }
                inPixel8++;
                *(inPixel8) = (unsigned char)((float)*(inPixel8) / alpha);
                if (*(inPixel8) > max) {
                    max = *(inPixel8);
                }
                inPixel8 += 2;
            } else {
                inPixel8 += 4;
            }
        }
    }
    
    GLuint ret = _GLLoadTextureFromData(data, pixelFormat, width, height);
	
	CGContextRelease(context);
	free(data);
	
	return ret;
}

static GLuint SZGLLoadTextureImpl_Apple(NSString *imagePath)
{
	CGDataProviderRef imgDataProvider = CGDataProviderCreateWithCFData((CFDataRef)[NSData dataWithContentsOfFile:imagePath]);
	CGImageRef image = CGImageCreateWithPNGDataProvider(imgDataProvider, NULL, true, kCGRenderingIntentDefault); // Or JPEGDataProvider
    
	GLuint ret = _GLLoadTextureFromCGImage(image);
    
    CFRelease(image);
    
    return ret;
}


#pragma mark -
#pragma mark PVRTC Support

#define PVR_TEXTURE_FLAG_TYPE_MASK	0xff

enum
{
	kPVRTextureFlagTypePVRTC_2 = 24,
	kPVRTextureFlagTypePVRTC_4
};

typedef struct _PVRTexHeader
    {
        uint32_t headerLength;
        uint32_t height;
        uint32_t width;
        uint32_t numMipmaps;
        uint32_t flags;
        uint32_t dataLength;
        uint32_t bpp;
        uint32_t bitmaskRed;
        uint32_t bitmaskGreen;
        uint32_t bitmaskBlue;
        uint32_t bitmaskAlpha;
        uint32_t pvrTag;
        uint32_t numSurfs;
    } PVRTexHeader;


#pragma mark -
#pragma mark Texture Loading Interface

GLuint SZGLLoadTexture(NSString *imagePath)
{
    return SZGLLoadTextureImpl_Apple(imagePath);
}

