//
//  EAGLView.m
//  OpenGLES_iPhone
//
//  Created by mmalc Crawford on 11/18/10.
//  Copyright 2010 Apple Inc. All rights reserved.
//

#import <QuartzCore/QuartzCore.h>
#import "EAGLView.h"
#include "app.h"

@interface EAGLView (PrivateMethods)
- (void)createFramebuffer;
- (void)deleteFramebuffer;
@end

@implementation EAGLView

@synthesize context;

// You must implement this method
+ (Class)layerClass
{
    return [CAEAGLLayer class];
}

//The EAGL view is stored in the nib file. When it's unarchived it's sent -initWithCoder:.
- (id)initWithCoder:(NSCoder*)coder
{
    self = [super initWithCoder:coder];
	if (self) {
        CAEAGLLayer *eaglLayer = (CAEAGLLayer *)self.layer;
        
        eaglLayer.opaque = TRUE;
        eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
                                        [NSNumber numberWithBool:FALSE], kEAGLDrawablePropertyRetainedBacking,
                                        kEAGLColorFormatRGBA8, kEAGLDrawablePropertyColorFormat,
                                        nil];
        
        self.multipleTouchEnabled = TRUE;
    }
    
    return self;
}

- (void)dealloc
{
    [self deleteFramebuffer];    
    [context release];
    
    [super dealloc];
}

- (void)setContext:(EAGLContext *)newContext
{
    if (context != newContext) {
        [self deleteFramebuffer];
        
        [context release];
        context = [newContext retain];
        
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)createFramebuffer
{
    if (context && !defaultFramebuffer) {
        [EAGLContext setCurrentContext:context];
        
        // Create default framebuffer object.
        glGenFramebuffers(1, &defaultFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        // Create color render buffer and allocate backing store.
        glGenRenderbuffers(1, &colorRenderbuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        [context renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)self.layer];
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &framebufferWidth);
        glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &framebufferHeight);
        
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
        
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            NSLog(@"Failed to make complete framebuffer object %x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }
}

- (void)deleteFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (defaultFramebuffer) {
            glDeleteFramebuffers(1, &defaultFramebuffer);
            defaultFramebuffer = 0;
        }
        
        if (colorRenderbuffer) {
            glDeleteRenderbuffers(1, &colorRenderbuffer);
            colorRenderbuffer = 0;
        }
    }
}

- (void)setFramebuffer
{
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        if (!defaultFramebuffer)
            [self createFramebuffer];
        
        glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebuffer);
        
        glViewport(0, 0, framebufferWidth, framebufferHeight);
    }
}

- (BOOL)presentFramebuffer
{
    BOOL success = FALSE;
    
    if (context) {
        [EAGLContext setCurrentContext:context];
        
        glBindRenderbuffer(GL_RENDERBUFFER, colorRenderbuffer);
        
        success = [context presentRenderbuffer:GL_RENDERBUFFER];
    }
    
    return success;
}

- (void)layoutSubviews
{
    // The framebuffer will be re-created at the beginning of the next setFramebuffer method call.
    [self deleteFramebuffer];
}


void transXY(lw::TouchEvent& evt, CGPoint& pt, CGPoint& prevPt){
	lw::App::Orientation orientation = lw::App::s().getConfig().orientation;
	switch (orientation) {
		case lw::App::ORIENTATION_UP:
			evt.x = pt.x;
			evt.y = pt.y;
			evt.prevX = prevPt.x;
			evt.prevY = prevPt.y;
			break;
		case lw::App::ORIENTATION_RIGHT:
			evt.x = pt.y;
			evt.y = 320-pt.x;
			evt.prevX = prevPt.y;
			evt.prevY = 320 - prevPt.x;
			break;
		case lw::App::ORIENTATION_LEFT:
			evt.x = 480-pt.y;
			evt.y = pt.x;
			evt.prevX = 480-prevPt.y;
			evt.prevY = prevPt.x;
			break;
		default:
			break;
	}
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event{
	std::vector<lw::TouchEvent> events;
	for ( UITouch* touch in touches ){
		lw::TouchEvent evt;
		evt.type = lw::TouchEvent::TOUCH;
		CGPoint pt = [touch locationInView:self];
		CGPoint prevPt = [touch previousLocationInView:self];
		transXY(evt, pt, prevPt);
		//lwinfo(evt.x << ":" << evt.y);
		events.push_back(evt);
	}
	lw::g_gestrueMgr.onTouchEvent(events);
	
	if ( !lw::UIEventProcess(events) ){
		lw::TaskMgr::onTouchEvent(events);
	}
	lw::g_gestrueMgr.main();
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event{
	std::vector<lw::TouchEvent> events;
	//lwinfo([touches count]);
	for ( UITouch* touch in touches ){
		lw::TouchEvent evt;
		evt.type = lw::TouchEvent::MOVE;
		CGPoint pt = [touch locationInView:self];
		CGPoint prevPt = [touch previousLocationInView:self];
		transXY(evt, pt, prevPt);
		//lwinfo(evt.x << ":" << evt.y);
		events.push_back(evt);
	}
	lw::g_gestrueMgr.onTouchEvent(events);
	
	if ( !lw::UIEventProcess(events) ){
		lw::TaskMgr::onTouchEvent(events);
	}
	lw::g_gestrueMgr.main();
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event{
	std::vector<lw::TouchEvent> events;
	for ( UITouch* touch in touches ){
		lw::TouchEvent evt;
		evt.type = lw::TouchEvent::UNTOUCH;
		CGPoint pt = [touch locationInView:self];
		CGPoint prevPt = [touch previousLocationInView:self];
		transXY(evt, pt, prevPt);
		events.push_back(evt);
	}
	lw::g_gestrueMgr.onTouchEvent(events);
	
	if ( !lw::UIEventProcess(events) ){
		lw::TaskMgr::onTouchEvent(events);
	}
	lw::g_gestrueMgr.main();
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event{
	std::vector<lw::TouchEvent> events;
	for ( UITouch* touch in touches ){
		lw::TouchEvent evt;
		evt.type = lw::TouchEvent::UNTOUCH;
		CGPoint pt = [touch locationInView:self];
		CGPoint prevPt = [touch previousLocationInView:self];
		transXY(evt, pt, prevPt);
		events.push_back(evt);
	}
	lw::g_gestrueMgr.onTouchEvent(events);
	
	if ( !lw::UIEventProcess(events) ){
		lw::TaskMgr::onTouchEvent(events);
	}
	lw::g_gestrueMgr.main();
}


@end
