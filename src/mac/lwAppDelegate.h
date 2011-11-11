//
//  lwAppDelegate.h
//  lw
//
//  Created by Li Wei on 11-4-2.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class lwViewController;

@interface lwAppDelegate : NSObject <UIApplicationDelegate> {

}

@property (nonatomic, retain) IBOutlet UIWindow *window;

@property (nonatomic, retain) IBOutlet lwViewController *viewController;

@end
