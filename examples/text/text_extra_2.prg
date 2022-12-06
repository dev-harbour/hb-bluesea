/*
 *
 */

#include "hbbluesea.ch"

#define TEXTSIZE 36

PROCEDURE Main()

   LOCAL w
   LOCAL text := "Text extra"
   LOCAL textWidth
   LOCAL textHeight

   w := bs_CreateWindow( 720, 450, text )

   bs_FreeType( w, "../../font/freemono/FreeMono.otf" )

   textWidth  := bs_TextWidth( w, text, TEXTSIZE )
   textHeight := bs_TextHeight( w, text, TEXTSIZE )

   WHILE( ! bs_MainLoop( w ) .AND. ! bs_GetKey( w, KEY_ESCAPE ) )

      bs_Begin( w )

         bs_Background( w, 0xffffff )
         bs_TextEx( w, text, ( bs_WinWidth( w ) - textWidth ) / 2, ( bs_WinHeight( w ) + textHeight ) / 2, TEXTSIZE, 0x000000 )

      bs_End( w )

      bs_WaitEvents( w )

   ENDDO

   bs_CloseWindow( w )

RETURN

