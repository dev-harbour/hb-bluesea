/*
 *
 */
#include "hbbluesea.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
/* BlueSea */
static HB_GARBAGE_FUNC( hb_bluesea_Destructor )
{
   BlueSea **ppBlueSea = ( BlueSea **) Cargo;

   if( *ppBlueSea )
   {
      *ppBlueSea = NULL;
   }
}

static const HB_GC_FUNCS s_gcBlueSeaFuncs =
{
   hb_bluesea_Destructor,
   hb_gcDummyMark
};

BlueSea *hb_BlueSea_Param( int iParam )
{
   BlueSea **ppBlueSea = ( BlueSea **) hb_parptrGC( &s_gcBlueSeaFuncs, iParam );

   if( ppBlueSea && *ppBlueSea )
   {
      return *ppBlueSea;
   }
   else
   {
      HB_ERR_ARGS();
      return NULL;
   }
}

BlueSea *hb_BlueSea_ParamGet( int iParam )
{
   BlueSea **ppBlueSea = ( BlueSea **) hb_parptrGC( &s_gcBlueSeaFuncs, iParam );

   return ppBlueSea ? *ppBlueSea : NULL;
}

BlueSea *hb_BlueSea_ItemGet( PHB_ITEM pItem )
{
   BlueSea **ppBlueSea = ( BlueSea **) hb_itemGetPtrGC( pItem, &s_gcBlueSeaFuncs );

   return ppBlueSea ? *ppBlueSea : NULL;
}

PHB_ITEM hb_BlueSea_ItemPut( PHB_ITEM pItem, BlueSea *pBlueSea )
{
   BlueSea **ppBlueSea = ( BlueSea **) hb_gcAllocate( sizeof( BlueSea *), &s_gcBlueSeaFuncs );
   *ppBlueSea = pBlueSea;
   return hb_itemPutPtrGC( pItem, ppBlueSea );
}

void hb_BlueSea_ItemClear( PHB_ITEM pItem )
{
   BlueSea **ppBlueSea = ( BlueSea **) hb_itemGetPtrGC( pItem, &s_gcBlueSeaFuncs );

   if( ppBlueSea )
   {
      *ppBlueSea = NULL;
   }
}

void hb_BlueSea_Return( BlueSea *pBlueSea )
{
   if( pBlueSea )
   {
      hb_BlueSea_ItemPut( hb_param( -1, HB_IT_ANY ), pBlueSea );
   }
   else
   {
      hb_ret();
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static functions
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   BlueSea *s_w = glfwGetWindowUserPointer( window );

   s_w->cursorX = x;
   s_w->cursorY = y;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   BlueSea *s_w = glfwGetWindowUserPointer( window );

   s_w->keyKey      = key;
   s_w->keyScancode = scancode;
   s_w->keyAction   = action;
   s_w->keyMods     = mods;

   if( action != GLFW_PRESS )
      return;

   switch( key )
   {
      case KEY_PRINT_SCREEN:

         cairo_surface_write_to_png( s_w->sf, "output.png" );
         break;

   }
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   BlueSea *s_w = glfwGetWindowUserPointer( window );

   s_w->mouseButton = button;
   s_w->mouseAction = action;
   s_w->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   BlueSea *s_w = glfwGetWindowUserPointer( window );

   s_w->winMaximized = maximized;
}

static void hex_to_rgb( cairo_t *cr, uint32_t hexColor )
{
   double r, g, b;
   r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
   g = ( ( hexColor >> 8  ) & 0xFF ) / 255.0;
   b = (   hexColor         & 0xFF ) / 255.0;

   cairo_set_source_rgba( cr, r, g, b, 1.0 );
}

static double *coord( cairo_t *cr, double x, double y )
{
   double *a =  malloc( sizeof( double[ 2 ] ) );

   cairo_user_to_device( cr, &x, &y );

   a[ 0 ] = round( x ) + 0.5;
   a[ 1 ] = round( y ) + 0.5;

   return a;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
//BlueSea *bs_CreateWindow( int width, int height, const char *title )
HB_FUNC( BS_CREATEWINDOW )
{
   if( hb_param( 1, HB_IT_NUMERIC ) != NULL && hb_param( 2, HB_IT_NUMERIC ) != NULL &&  hb_param( 3, HB_IT_STRING ) != NULL )
   {
      BlueSea *w;

      if( ! glfwInit() )
      {
         exit( EXIT_FAILURE );
      }

      glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

      w = calloc( 1, sizeof( BlueSea ) );

      w->width  = hb_parni( 1 );
      w->height = hb_parni( 2 );
      w->title  = hb_parc( 3 );

      w->window = glfwCreateWindow( w->width, w->height, w->title, NULL, NULL );
      if( ! w->window )
      {
         free( w );
         glfwTerminate();
         exit( EXIT_FAILURE );
      }
      glfwMakeContextCurrent( w->window );
      glfwSetWindowUserPointer( w->window, w );

      glfwSetCursorPosCallback( w->window, cursor_position_callback );
      glfwSetKeyCallback( w->window, key_callback );
      glfwSetMouseButtonCallback( w->window, mouse_button_callback );
      glfwSetWindowMaximizeCallback( w->window, window_maximize_callback );

      glfwSetWindowSizeLimits( w->window, w->width, w->height, GLFW_DONT_CARE , GLFW_DONT_CARE );

   #if defined( GLFW_EXPOSE_NATIVE_WIN32 )
      w->dc = GetDC( glfwGetWin32Window( w->window ) );
      w->sf = cairo_win32_surface_create_with_format( w->dc, CAIRO_FORMAT_ARGB32 );
      w->cr = cairo_create( w->sf );
   #endif
   #if defined( GLFW_EXPOSE_NATIVE_X11 )
      w->xd = glfwGetX11Display();
      w->sf = cairo_xlib_surface_create( w->xd, glfwGetX11Window( w->window ), DefaultVisual( w->xd, DefaultScreen( w->xd ) ), w->width, w->height );
      w->cr = cairo_create( w->sf );
   #endif

      hb_BlueSea_Return( w );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

//bool bs_MainLoop( BlueSea *w )
HB_FUNC( BS_MAINLOOP )
{
   BlueSea *w = hb_BlueSea_Param( 1 );

   if( w )
   {
      w->closeFlag = glfwWindowShouldClose( w->window );
      glfwSetWindowShouldClose( w->window, GLFW_FALSE );

      hb_retl( w->closeFlag );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

// bool bs_CloseWindow( BlueSea *w )
HB_FUNC( BS_CLOSEWINDOW )
{
   BlueSea *w = hb_BlueSea_Param( 1 );

   if( w )
   {
      cairo_font_face_destroy( w->ff );

      glfwDestroyWindow( w->window );
      free( w );
      glfwTerminate();

      exit( EXIT_SUCCESS );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// void begin_drawing( pBlueSea w )
HB_FUNC( BEGIN_DRAWING )
{
   BlueSea *w = hb_BlueSea_Param( 1 );

   if( w )
   {
      glfwGetFramebufferSize( w->window, &w->width, &w->height );

   #if defined( GLFW_EXPOSE_NATIVE_WIN32 )
      if( w->tmp_width != w->width || w->tmp_height != w->height )
      {
         w->sf = cairo_win32_surface_create_with_format( w->dc, CAIRO_FORMAT_ARGB32 );
         cairo_destroy( w->cr );
         w->cr = cairo_create( w->sf );
         cairo_surface_destroy( w->sf );
         w->tmp_width  = w->width;
         w->tmp_height = w->height;
      }
   #endif
   #if defined( GLFW_EXPOSE_NATIVE_X11 )
      cairo_xlib_surface_set_size( w->sf, w->width, w->height );
   #endif

      cairo_push_group( w->cr );

      hex_to_rgb( w->cr, w->background );
      cairo_paint ( w->cr );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

//void end_drawing( pBlueSea w )
HB_FUNC( END_DRAWING )
{
   BlueSea *w = hb_BlueSea_Param( 1 );

   if( w )
   {
      cairo_pop_group_to_source( w->cr );
      cairo_paint( w->cr );
      cairo_surface_flush( w->sf );
   }
   else
   {
      HB_ERR_ARGS();
   }
}

//int cairo_functions( pBlueSea w, iCairo type, int par1, int par2, int par3, int par4, int par5, int par6, int par7 )
HB_FUNC( CAIRO_FUNCTIONS )
{
   BlueSea *w = hb_BlueSea_Param( 1 );
   int par1 = hb_parni( 3 );
   int par2 = hb_parni( 4 );
   int par3 = hb_parni( 5 );
   int par4 = hb_parni( 6 );
   int par5 = hb_parni( 7 );
   int par6 = hb_parni( 8 );
   int par7 = hb_parni( 9 );

   int ret = 1;
   double *p1 = NULL;
   double *p2 = NULL;
   double a = 0;
   double x1 = 0;
   double y1 = 0;

   switch( hb_parni( 2 ) )
   {
      case CAIRO_ARC:
         break;

      case CAIRO_ARC_FILLED:
         break;

      case CAIRO_BACKGROUND:

         w->background = par1;
         break;

      case CAIRO_CIRCLE:

         hex_to_rgb( w->cr, par4 );
         cairo_set_line_width( w->cr, 1.0 );
         cairo_arc( w->cr, par1, par2, par3, 0, 2 * M_PI );
         cairo_stroke( w->cr );
         break;

      case CAIRO_CIRCLE_FILLED:

         hex_to_rgb( w->cr, par4 );
         cairo_arc( w->cr, par1, par2, par3, 0, 2 * M_PI );
         cairo_fill( w->cr );
         break;

      case CAIRO_CIRCLELINEWIDTH:

         hex_to_rgb( w->cr, par5 );
         cairo_set_line_width( w->cr, par4 );
         cairo_arc( w->cr, par1, par2, par3, 0, 2 * M_PI );
         cairo_stroke( w->cr );
         break;

      case CAIRO_ELLIPSE:

         break;

      case CAIRO_ELLIPSE_FILLED:

         break;

      case CAIRO_CURVE:

         break;

      case CAIRO_GETPIXEL:

         break;

      case CAIRO_HEXAGON:

         hex_to_rgb( w->cr, par4 );
         cairo_set_line_width( w->cr, 6.0 );
         a = 2 * M_PI / 6;
         x1 = par1 + par3 * cos( a * 5 + M_PI_2 );
         y1 = par2 + par3 * sin( a * 5 + M_PI_2 );
         cairo_move_to( w->cr, x1, y1 );
         for( uint8_t i = 0; i < 6; i++ )
         {
            cairo_line_to( w->cr, par1 + par3 * cos( a * i + M_PI_2 ), par2 + par3 * sin( a * i + M_PI_2 ) );
         }
         cairo_close_path( w->cr );
         cairo_stroke( w->cr );
         break;

      case CAIRO_HEXAGON_FILLED:

         hex_to_rgb( w->cr, par4 );
         a = 2 * M_PI / 6;
         x1 = par1 + par3 * cos( a * 5 + M_PI_2 );
         y1 = par2 + par3 * sin( a * 5 + M_PI_2 );
         cairo_move_to( w->cr, x1, y1 );
         for( uint8_t i = 0; i < 6; i++ )
         {
            cairo_line_to( w->cr, par1 + par3 * cos( a * i + M_PI_2 ), par2 + par3 * sin( a * i + M_PI_2 ) );
         }
         cairo_close_path( w->cr );
         cairo_fill( w->cr );
         break;

      case CAIRO_IMAGE:

         break;

      case CAIRO_LINE:

         hex_to_rgb( w->cr, par5 );
         cairo_set_line_width( w->cr, 1.0 );
         cairo_move_to( w->cr, par1, par2 );
         cairo_line_to( w->cr, par3, par4 );
         cairo_stroke( w->cr );
         break;

      case CAIRO_LINES:

         p1 = coord( w->cr, par1, par2 );
         p2 = coord( w->cr, par3, par4 );

         hex_to_rgb( w->cr, par5 );
         cairo_set_line_width( w->cr, 1.0 );
         cairo_move_to( w->cr, p1[ 0 ], p1[ 1 ] );
         cairo_line_to( w->cr, p2[ 0 ], p2[ 1 ] );
         cairo_stroke( w->cr );

         free( p1 );
         free( p2 );
         break;

      case CAIRO_RGBTODEC:

         ret = ( ( par1 & 0xff ) << 16 ) + ( ( par2 & 0xff ) << 8 ) + ( par3 & 0xff );
         break;

      case CAIRO_POLYGON:

         break;

      case CAIRO_POLYGON_FILLED:

         break;

      case CAIRO_POLYLINE:

         break;

      case CAIRO_PUTPIXEL:

         hex_to_rgb( w->cr, par3 );
         cairo_set_line_width( w->cr, 1.0 );
         cairo_rectangle( w->cr, par1, par2, 1.0, 1.0 );
         cairo_fill( w->cr );
         break;

      case CAIRO_RECT:

         hex_to_rgb( w->cr, par6 );
         cairo_set_line_width( w->cr, 1.0 );
         if( par5 == 0 )
         {
            cairo_rectangle( w->cr, par1, par2, par3, par4 );
         }
         else
         {
            cairo_new_sub_path( w->cr );
            cairo_arc( w->cr, par1 + par5, par2 + par5, par5, M_PI, 3 * M_PI / 2 );
            cairo_arc( w->cr, par1 + par3 - par5, par2 + par5, par5, 3 * M_PI / 2, 2 * M_PI );
            cairo_arc( w->cr, par1 + par3 - par5, par2 + par4 - par5, par5, 0, M_PI / 2 );
            cairo_arc( w->cr, par1 + par5, par2 + par4 - par5, par5, M_PI / 2, M_PI );
            cairo_close_path( w->cr );
         }
         cairo_stroke( w->cr );
         break;

      case CAIRO_RECTS:

         p1 = coord( w->cr, par1, par2 );

         hex_to_rgb( w->cr, par6 );
         cairo_set_line_width( w->cr, 1.0 );
         if( par5 == 0 )
         {
            cairo_rectangle( w->cr, p1[ 0 ], p1[ 1 ], par3, par4 );
         }
         else
         {
            cairo_new_sub_path( w->cr );
            cairo_arc( w->cr, p1[ 0 ] + par5, p1[ 1 ] + par5, par5, M_PI, 3 * M_PI / 2 );
            cairo_arc( w->cr, p1[ 0 ] + par3 - par5, p1[ 1 ] + par5, par5, 3 * M_PI / 2, 2 * M_PI );
            cairo_arc( w->cr, p1[ 0 ] + par3 - par5, p1[ 1 ] + par4 - par5, par5, 0, M_PI / 2 );
            cairo_arc( w->cr, p1[ 0 ] + par5, p1[ 1 ] + par4 - par5, par5, M_PI / 2, M_PI );
            cairo_close_path( w->cr );
         }
         cairo_stroke( w->cr );

         free( p1 );
         break;

      case CAIRO_RECT_FILLED:

         hex_to_rgb( w->cr, par6 );
         if( par5 == 0 )
         {
            cairo_rectangle( w->cr, par1, par2, par3, par4 );
         }
         else
         {
            cairo_new_sub_path( w->cr );
            cairo_arc( w->cr, par1 + par5, par2 + par5, par5, M_PI, 3 * M_PI / 2 );
            cairo_arc( w->cr, par1 + par3 - par5, par2 + par5, par5, 3 * M_PI / 2, 2 * M_PI );
            cairo_arc( w->cr, par1 + par3 - par5, par2 + par4 - par5, par5, 0, M_PI / 2 );
            cairo_arc( w->cr, par1 + par5, par2 + par4 - par5, par5, M_PI / 2, M_PI );
            cairo_close_path( w->cr );
         }
         cairo_fill( w->cr );
         break;

      case CAIRO_RECT_MULTI_COLOR:

         break;

      case CAIRO_TRIANGLE:

         hex_to_rgb( w->cr, par7 );
         cairo_set_line_width( w->cr, 1.0 );
         cairo_move_to( w->cr, par1, par2 );
         cairo_line_to( w->cr, par3, par4 );
         cairo_line_to( w->cr, par5, par6 );
         cairo_close_path( w->cr );
         cairo_stroke( w->cr );
         break;

      case CAIRO_TRIANGLE_FILLED:

         hex_to_rgb( w->cr, par7 );
         cairo_move_to( w->cr, par1, par2 );
         cairo_line_to( w->cr, par3, par4 );
         cairo_line_to( w->cr, par5, par6 );
         cairo_close_path( w->cr );
         cairo_fill( w->cr );
         break;

      default:
         hb_retni( 0 );
   }
   return hb_retni( ret );
}

static char *malloc_strdup( const char *pszText )
{
   size_t nLen = strlen( pszText ) + 1;
   return memcpy( malloc( nLen ), pszText, nLen );
}

// int text_functions( pBlueSea w, iText type, const char *par1, int par2, int par3, int par4  )
HB_FUNC( TEXT_FUNCTIONS )
{
   BlueSea *w = hb_BlueSea_Param( 1 );
   const char *par1 = hb_parc( 3);
   int par2 = hb_parni( 4 );
   int par3 = hb_parni( 5 );
   int par4 = hb_parni( 6 );
   int par5 = hb_parni( 7 );

   int ret = 1;
   FT_Library alibrary;
   FT_Error   error;
   FT_Face    aface;

   switch( hb_parni( 2 ) )
   {
   case TEXT_FREE_TYPE:

      error = FT_Init_FreeType( &alibrary );
      if( error )
      {
         fprintf( stderr, "Error %d opening library. \n", error );
         exit( EXIT_FAILURE );
      }

      error = FT_New_Face( alibrary, par1, 0, &aface);
      if( error )
      {
         fprintf( stderr, "Error %d opening %s \n", error, par1 );
         exit( EXIT_FAILURE );
      }

      w->ff = cairo_ft_font_face_create_for_ft_face( aface, 0 );
      break;

   case TEXT_CONST:

      cairo_set_font_face( w->cr, w->ff );
      cairo_set_font_size( w->cr, 18 );
      hex_to_rgb( w->cr, par4 );
      cairo_move_to( w->cr, par2, par3 );
      cairo_show_text( w->cr, par1 ? malloc_strdup( par1 ) : NULL );
      break;

   case TEXT_EXTRA:

      cairo_set_font_face( w->cr, w->ff );
      cairo_set_font_size( w->cr, par4 );
      hex_to_rgb( w->cr, par5 );
      cairo_move_to( w->cr, par2, par3 );
      cairo_show_text( w->cr, par1 );

      break;

   case TEXT_WIDTH:

      cairo_set_font_size( w->cr, par2 );
      cairo_text_extents( w->cr, par1, &w->te );
      ret = w->te.width;
      break;

   case TEXT_HEIGHT:

      cairo_set_font_size( w->cr, par2 );
      cairo_text_extents( w->cr, par1, &w->te );
      ret = w->te.height;
      break;

   default:

      hb_retni( 0 );
   }

   hb_retni( ret );
}

// int glfw_functions( pBlueSea w, iGlfw type, int par1 )
HB_FUNC( GLFW_FUNCTIONS )
{
   BlueSea *w = hb_BlueSea_Param( 1 );
   int par1 = hb_parni( 3 );

   int ret = 1;

   switch( hb_parni( 2 ) )
   {
   case GLFW_GET_KEY:

      ret = ( w->keyKey == par1 ) == GLFW_PRESS ? T : F;
      break;

   case GLFW_GET_MOUSEBUTTON:

      ret = ( w->mouseButton == par1 ) == GLFW_PRESS ? T : F;
      break;

   case GLFW_WIN_WIDTH:

      ret = w->width;
      break;

   case GLFW_WIN_HEIGHT:

      ret = w->height;
      break;

   case GLFW_WIN_MAXIMIZED:

      ret = w->winMaximized;
      break;

   case GLFW_POLLEVENTS:

      glfwPollEvents();
      break;

   case GLFW_WAITEVENTS:

      glfwWaitEvents();
      break;

   case GLFW_WAITEVENTSTIMEOUT:

      glfwWaitEventsTimeout( par1 );
      break;

   default:

      hb_retni( 0 );
   }

   if( GLFW_GET_KEY == hb_parni( 2 ) || GLFW_GET_MOUSEBUTTON == hb_parni( 2 ) )
   {
      hb_retl( ret );
   }
   else
   {
      hb_retni( ret );
   }
}
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
