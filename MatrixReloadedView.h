//
//  MatrixReloadedView.h
//  MatrixReloaded
//
//  Created by Patrik Sjšberg on 2009-04-28.
//  Copyright (c) 2009, __MyCompanyName__. All rights reserved.
//

#import <AppKit/AppKit.h>
#import <ScreenSaver/ScreenSaver.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

#include "matrix.h"

@interface MatrixReloadedView : ScreenSaverView 
{
  NSOpenGLView *_view;
  
  int mainScreenOnly_;
  
  BOOL isConfiguring_;
  BOOL preview_;
  BOOL mainScreen_;
  
  IBOutlet id IBmainScreen_;
  
  IBOutlet id IBversion_;
  
  MGLSettings * settings_;
}

- (void) doScroll:(NSNumber *) inNumber;

@end
