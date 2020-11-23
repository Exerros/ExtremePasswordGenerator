//
// Private colormap support for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2018 by Bill Spitzak and others.
//
// This library is free software. Distribution and use rights are outlined in
// the file "COPYING" which should have been included with this file.  If this
// file is missing or damaged, see the license at:
//
//     https://www.fltk.org/COPYING.php
//
// Please see the following page on how to report bugs and issues:
//
//     https://www.fltk.org/bugs.php
//

// Using the default system colormap can be a bad idea on PseudoColor
// visuals, since typically every application uses the default colormap and
// you can run out of colormap entries easily.
//
// The solution is to always create a new colormap on PseudoColor displays
// and copy the first 16 colors from the default colormap so that we won't
// get huge color changes when switching windows.

#include "config_lib.h"
#include <FL/Fl.H>
#include <FL/platform.H>
#include "Fl_System_Driver.H"


#if defined(FL_CFG_WIN_X11) && !defined(FL_DOXYGEN)
// X version
#include "drivers/X11/Fl_X11_System_Driver.H"

void Fl_X11_System_Driver::own_colormap() {
  fl_open_display();
#if USE_COLORMAP
  switch (fl_visual->c_class) {
  case GrayScale :
  case PseudoColor :
  case DirectColor :
    break;
  default:
    return; // don't do anything for non-colormapped visuals
  }
  int i;
  XColor colors[16];
  // Get the first 16 colors from the default colormap...
  for (i = 0; i < 16; i ++) colors[i].pixel = i;
  XQueryColors(fl_display, fl_colormap, colors, 16);
  // Create a new colormap...
  fl_colormap = XCreateColormap(fl_display,
                                RootWindow(fl_display,fl_screen),
                                fl_visual->visual, AllocNone);
  // Copy those first 16 colors to our own colormap:
  for (i = 0; i < 16; i ++)
    XAllocColor(fl_display, fl_colormap, colors + i);
#endif // USE_COLORMAP
}

#endif // FL_CFG_WIN_X11

/** \fn Fl::own_colormap()
 Makes FLTK use its <a href="fltk-colormap.png">own colormap</a>.  This may make FLTK display better
 and will reduce conflicts with other programs that want lots of colors.
 However the colors may flash as you move the cursor between windows.

 <P>This does nothing if the current visual is not colormapped.
 */
void Fl::own_colormap() {
  Fl::system_driver()->own_colormap();
}
