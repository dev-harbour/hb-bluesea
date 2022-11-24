/*
 *
 */

#ifndef HBBLUESEA_H_
#define HBBLUESEA_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hbapi.h"
#include "hbapierr.h"
#include "hbapiitm.h"
#include "hbapistr.h"
#include "hbstack.h"
#include "hbbluesea.ch"

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#include <freetype2/freetype/freetype.h>

#if defined( GLFW_EXPOSE_NATIVE_WIN32 )
   #include <cairo/cairo-win32.h>
#endif
#if defined( GLFW_EXPOSE_NATIVE_X11 )
   #include <cairo/cairo-xlib.h>
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct
{
   GLFWwindow      *window;      // handle of the created window
   int              width;       // desired width, in screen coordinates, of the window
   int              height;      // desired height, in screen coordinates
   int              tmp_width;
   int              tmp_height;
   const char      *title;       // initial, UTF-8 encoded window title
   bool             closeFlag;   // returns the value of the close flag of the specified window
   // cursor
   double           cursorX;     // new cursor x-coordinate, relative to the left edge of the content area
   double           cursorY;     // new cursor y-coordinate, relative to the top edge of the content area
   // key
   int              keyKey;      // the keyboard key that was pressed or released
   int              keyScancode; // the system-specific scancode of the key
   int              keyAction;   // GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT. Future releases may add more actions
   int              keyMods;     // bit field describing which modifier keys were held down
   // mouse button
   int              mouseButton; // mouse button that was pressed or released
   int              mouseAction; // one of GLFW_PRESS or GLFW_RELEASE, future releases may add more actions
   int              mouseMods;   // bit field describing which modifier keys were held down
   // window maximized
   int              winMaximized;// GLFW_TRUE if the window was maximized, or GLFW_FALSE if it was restored
   // color
   int              background;
   // cairo
   cairo_surface_t   *sf;
   cairo_t           *cr;
   cairo_font_face_t *ff;
   //---
#if defined( GLFW_EXPOSE_NATIVE_WIN32 )
   HDC dc;
#endif
#if defined( GLFW_EXPOSE_NATIVE_X11 )
   Display *xd;
#endif
} BlueSea;

BlueSea *hb_BlueSea_Param( int iParam );
BlueSea *hb_BlueSea_ParamGet( int iParam );
BlueSea *hb_BlueSea_ItemGet( PHB_ITEM pItem );
PHB_ITEM hb_BlueSea_ItemPut( PHB_ITEM pItem, BlueSea *pBlueSea );
void     hb_BlueSea_ItemClear( PHB_ITEM pItem );
void     hb_BlueSea_Return( BlueSea *pBlueSea );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )
#define LEN( n ) ( sizeof( n ) / sizeof( n )[ 0 ] )

#define HB_ERR_ARGS() ( hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS ) )

#endif /* End HBBLUESEA_H_ */
