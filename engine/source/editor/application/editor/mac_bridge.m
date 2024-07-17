// bridge.m
#import <Cocoa/Cocoa.h>
#include <SDL.h>
#import "mac_bridge.h"

void createMacMenu() {
    NSLog(@"Creating macOS menu...");

    @autoreleasepool {
//        NSApplication *app = [NSApplication sharedApplication];
        // Initialize menu bar
        NSMenu *mainMenu = [[NSMenu alloc] init];

        // Allocate menu bar items
        NSMenuItem *appMenuItem = [[NSMenuItem alloc] init];
        NSMenuItem *appMenuItem2 = [[NSMenuItem alloc] init];
        [mainMenu addItem:appMenuItem];
        [mainMenu addItem:appMenuItem2];

        NSApp.mainMenu = mainMenu;

        // Initialize menu bar items
        NSMenu *appMenu = [[NSMenu alloc] initWithTitle:@"Application"];
        NSMenu *appMenu2 = [[NSMenu alloc] initWithTitle:@"Window"];

        // Create menu bar options for menu bar 1
        NSString *appName = [[NSProcessInfo processInfo] processName];
        NSString *quitTitle = [NSString stringWithFormat:@"Quit %@", appName];
        NSMenuItem *quitMenuItem = [[NSMenuItem alloc] initWithTitle:quitTitle action:@selector(terminate:) keyEquivalent:@"q"];

        // Add menu sub-items for menu item 1
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

        // Create menu bar options for menu bar 2
        NSMenuItem *option1 = [[NSMenuItem alloc] initWithTitle:@"Open Tracy Profiler" action:@selector(optionOpenTracyProfilerAction:) keyEquivalent:@"1"];

        // Add menu sub-items for menu item 2
        [appMenu2 addItem:option1];
        [appMenuItem2 setSubmenu:appMenu2];

//        [option1 setTarget:NSApp];
//        [mainMenu addItem:option1];
//        NSMenu *appMenu2 = [[NSMenu alloc] initWithTitle:@"File"];
//        [appMenu2 addItem:option1];
//        [appMenuItem setSubmenu:appMenu2];

//        NSMenuItem *option2 = [[NSMenuItem alloc] initWithTitle:@"Option 2"
//                                                         action:@selector(option2Action:)
//                                                  keyEquivalent:@"2"];
//        [option2 setTarget:NSApp];
//        [mainMenu addItem:option2];

//        NSMenuItem *option3 = [[NSMenuItem alloc] initWithTitle:@"Option 3"
//                                                         action:@selector(option3Action:)
//                                                  keyEquivalent:@"3"];
//        [option3 setTarget:NSApp];
//        [mainMenu addItem:option3];
//        [app run];
    }
    NSLog(@"macOS menu created.");
}

@implementation NSApplication (MenuActions)
- (void)optionOpenTracyProfilerAction:(id)sender {
    NSLog(@"Opening Tracy Profiler");

    SDL_Event event;
    SDL_zero(event);
    event.type = SDL_USEREVENT;
    event.user.code = 1;
    SDL_PushEvent(&event);

//    NSFileManager *filemgr;
//    NSString *currentpath;
//
//    filemgr = [[NSFileManager alloc] init];
//    currentpath = [filemgr currentDirectoryPath];
//    NSLog (@"Tracy Profiler Path: \n%@", [NSString stringWithFormat:@"%@/%@", currentpath, @"unix/Tracy-release"]);
//
//    NSTask *task = [[NSTask alloc] init];
//    [task setLaunchPath:[NSString stringWithFormat:@"%@/%@", currentpath, @"build/unix/Tracy-release"]];
//    [task setArguments: @[]];
//
//    NSPipe *pipe = [NSPipe pipe];
//    [task setStandardOutput:pipe];
//    [task setStandardError:pipe];
//
//    NSFileHandle *file = [pipe fileHandleForReading];
//
//    [task launch];
    // below code freezes the main window
//    [task waitUntilExit];

   // NSData *data = [file readDataToEndOfFile];
   // NSString *output = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
   // NSLog(@"Output:\n%@", output);

//    int pid = [[NSProcessInfo processInfo] processIdentifier];
//    NSPipe *pipe = [NSPipe pipe];
//    NSFileHandle *file = pipe.fileHandleForReading;
//
//    NSTask *task = [[NSTask alloc] init];
//    task.launchPath = @"/usr/bin/grep";
//    task.arguments = @[@"foo", [NSString stringWithFormat:@"%@/%@", currentpath, @"bar.txt"]];
//    task.standardOutput = pipe;
//
//    [task launch];
//
//    NSData *data = [file readDataToEndOfFile];
//    [file closeFile];
//
//    NSString *grepOutput = [[NSString alloc] initWithData: data encoding: NSUTF8StringEncoding];
//    NSLog (@"grep returned:\n%@", grepOutput);
}

//- (void)option2Action:(id)sender {
//    NSLog(@"Option 2 selected");
//}
//
//- (void)option3Action:(id)sender {
//    NSLog(@"Option 3 selected");
//}
@end