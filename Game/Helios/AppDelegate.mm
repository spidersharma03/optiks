//
//  AppDelegate.m
//  Helios
//
//  Created by Prashant on 25/01/14.
//
//

#import "AppDelegate.h"
#include "Game.h"

@implementation AppDelegate

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event

{
    
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint touchLocation = [touch locationInView:self.window];
    
    Event gameEvent;
    gameEvent.eventType = SMOUSE_EVENT;
    gameEvent.mouseInput.Event = spider3d::LEFT_MOUSE_PRESSED_DOWN;

    gameEvent.mouseInput.X = 1024 - (int)touchLocation.y;
    gameEvent.mouseInput.Y = 768 - (int)touchLocation.x;
    
    Game::getInstance()->OnEvent(gameEvent);
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint touchLocation = [touch locationInView:self.window];
    
    Event gameEvent;
    gameEvent.eventType = SMOUSE_EVENT;
    gameEvent.mouseInput.Event = spider3d::SMOUSE_DRAGGED;
    
    gameEvent.mouseInput.X = 1024 - (int)touchLocation.y;
    gameEvent.mouseInput.Y = 768 - (int)touchLocation.x;
    
    
    Game::getInstance()->OnEvent(gameEvent);
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event

{
    UITouch *touch = [[event allTouches] anyObject];
    
    CGPoint touchLocation = [touch locationInView:self.window];
    
    Event gameEvent;
    gameEvent.eventType = SMOUSE_EVENT;
    gameEvent.mouseInput.Event = spider3d::LEFT_MOUSE_LEFT_UP;
    
    gameEvent.mouseInput.X = 1024 - (int)touchLocation.y;
    gameEvent.mouseInput.Y = 768 - (int)touchLocation.x;
    
    Game::getInstance()->OnEvent(gameEvent);
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    //self.mainViewController.view.multipleTouchEnabled=NO;
    //self.mainViewController.view.exclusiveTouch =YES;
    // Override point for customization after application launch.
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
