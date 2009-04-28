//
//  MatrixReloadedView.m
//  MatrixReloaded
//
//  Created by Patrik Sjöberg on 2009-04-28.
//  Copyright (c) 2009, __MyCompanyName__. All rights reserved.
//

#import "MatrixReloadedView.h"


@implementation MatrixReloadedView

- (id)initWithFrame:(NSRect)frameRect isPreview:(BOOL)preview
{
  NSString *identifier = [[NSBundle bundleForClass:[self class]] bundleIdentifier];
  ScreenSaverDefaults *defaults = [ScreenSaverDefaults defaultsForModuleWithName:identifier];
  
  self = [super initWithFrame:frameRect isPreview:preview];
  
  preview_=preview;
  
  isConfiguring_=NO;
  
  if (preview_==YES)
  {
    mainScreen_=YES;
  }
  else
  {
    mainScreen_= (frameRect.origin.x==0 && frameRect.origin.y==0) ? YES : NO;
  }
  
  mainScreenOnly_=[defaults integerForKey:@"MainScreen Only"];
  
  if (self)
  {
    
    if (mainScreenOnly_!=NSOnState || mainScreen_==YES)
    {
      NSOpenGLPixelFormatAttribute attribs[] = 
      {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAMinimumPolicy,
        (NSOpenGLPixelFormatAttribute)0
      };
      
      NSOpenGLPixelFormat *format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attribs] autorelease];
      
      if (format!=nil)
      {
        _view = [[NSOpenGLView alloc] initWithFrame:NSZeroRect pixelFormat:format];
        
        [self addSubview:_view];
        
        [_view release];
        
        [self setAnimationTimeInterval:1.0f/FRAME_RATE_SAMPLES];
      }
    }
  }
  
  return self;
  
}

- (void) setFrameSize:(NSSize)newSize
{
  [super setFrameSize:newSize];
  
  if (_view!=nil)
  {
    [_view setFrameSize:newSize];
  }
}


#pragma mark -

- (void) keyDown:(NSEvent *) theEvent
{
  NSString * tString;
  unsigned int stringLength;
  unsigned int i;
  unichar tChar;
  
  tString= [theEvent characters];
  
  stringLength=[tString length];
  
  for(i=0;i<stringLength;i++)
  {
    tChar=[tString characterAtIndex:i];
    
    RESET_TIMER(settings_);
    
    switch(tChar)
    {
      case NSPageUpFunctionKey: // move the scene into the distance.
        Z_Off -= 0.3f;
        break;
        
      case NSPageDownFunctionKey: // move the scene closer.
        Z_Off += 0.3f;
        break;
      case 'e':
        settings_->exit_mode^=1;
        break;
      default:
        [super keyDown:theEvent];
        return;
    }
  }
}

- (void) drawRect:(NSRect) inFrame
{
  [[NSColor blackColor] set];
  
  NSRectFill(inFrame);
  
  if (_view==nil)
  {    
    if (mainScreenOnly_!=NSOnState || mainScreen_==YES)
    {
      NSRect tFrame=[self frame];
      NSRect tStringFrame;
      NSDictionary * tAttributes;
      NSString * tString;
      NSMutableParagraphStyle * tParagraphStyle;
      
      tParagraphStyle=[[NSParagraphStyle defaultParagraphStyle] mutableCopy];
      [tParagraphStyle setAlignment:NSCenterTextAlignment];
      
      tAttributes = [NSDictionary dictionaryWithObjectsAndKeys:[NSFont systemFontOfSize:[NSFont systemFontSize]],NSFontAttributeName,[NSColor whiteColor],NSForegroundColorAttributeName,tParagraphStyle,NSParagraphStyleAttributeName,nil];
      
      [tParagraphStyle release];
      
      tString=NSLocalizedStringFromTableInBundle(@"Minimum OpenGL requirements\rfor this Screen Effect\rnot available\ron your graphic card.",@"Localizable",[NSBundle bundleForClass:[self class]],@"No comment");
      
      tStringFrame.origin=NSZeroPoint;
      tStringFrame.size=[tString sizeWithAttributes:tAttributes];
      
      tStringFrame=SSCenteredRectInRect(tStringFrame,tFrame);
      
      [tString drawInRect:tStringFrame withAttributes:tAttributes];
      
      return;
    }
  }
}


- (void)animateOneFrame
{
  if (isConfiguring_==NO && _view!=nil)
  {
    if (mainScreenOnly_!=NSOnState || mainScreen_==YES)
    {
      [[_view openGLContext] makeCurrentContext];
      
      if (!cbRenderScene(settings_))
      {
        [self performSelector:@selector(doScroll:) withObject:[NSNumber numberWithInt:settings_->mode2] afterDelay:0.060f];
        
      }
      
      [[_view openGLContext] flushBuffer];
    }
  }
}

- (void)startAnimation
{
  [super startAnimation];
  
  if (isConfiguring_==NO && _view!=nil)
  {
    if (mainScreenOnly_!=NSOnState || mainScreen_==YES)
    {
      NSSize tSize;
      
      [self lockFocus];
      [[_view openGLContext] makeCurrentContext];
      
      glClearColor(0.0, 0.0, 0.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT);
      
      [[_view openGLContext] flushBuffer];
      
      tSize=[_view frame].size;
      
      settings_=(MGLSettings *) malloc(sizeof(MGLSettings));
      
      initSettings(settings_);
      
      load_texture();
      make_text(settings_);
      
      ourInit((int) tSize.width,(int) tSize.height);
      
      [self unlockFocus];
    }
  }
}

- (void) doScroll:(NSNumber *) inNumber
{
  if (!scroll([inNumber intValue],settings_))
  {
    [self performSelector:@selector(doScroll:) withObject:[NSNumber numberWithInt:settings_->mode2] afterDelay:0.060f];
  }
}

- (void)stopAnimation
{
  [super stopAnimation];
  
  if (_view!=nil)
  {
    if (mainScreenOnly_!=NSOnState || mainScreen_==YES)
    {
      [[_view openGLContext] makeCurrentContext];
      
      [NSObject cancelPreviousPerformRequestsWithTarget:self];
      
      free(settings_);
    }
  }
}

- (BOOL) hasConfigureSheet
{
  return NO;
}

- (NSWindow*)configureSheet
{
  return nil;
}

@end
