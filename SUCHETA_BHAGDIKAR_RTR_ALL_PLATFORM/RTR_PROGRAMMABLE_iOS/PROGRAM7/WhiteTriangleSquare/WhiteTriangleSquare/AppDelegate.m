//
//  AppDelegate.m
//  Window-BlueColor
//
//  Created by ASTROMEDICOMP on 30/05/18.
//

#import "AppDelegate.h"

#import "ViewController.h"

#import "GLESView.h"

@implementation AppDelegate
{
@private
    UIWindow *mainWindow;
    ViewController *mainViewController;
    GLESView *myView;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // get screen bounds for fullscreen
   
    
    return(YES);
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // code
    [myView stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // code
    [myView startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // code
    [myView stopAnimation];
}

- (void)dealloc
{
    // code
    [myView release];
    
    [mainViewController release];
    
    [mainWindow release];
    
    [super dealloc];
}

@end
