/*
 *
 */

#include "hbbluesea.ch"

PROCEDURE Main()

   LOCAL w

   w := bs_CreateWindow( 680, 450, "The blue sea" )

   bs_FreeType( w, "../../font/9x18.bdf" )

   WHILE( ! bs_MainLoop( w ) .AND. ! bs_GetKey( w, KEY_ESCAPE ) )

      bs_Begin( w )

         bs_Background( w, 0x006994 )
         bs_Text( w, "Hello Harbour!", ( bs_WinWidth( w ) - 126 ) / 2, ( bs_WinHeight( w ) + 11 ) / 2, 0x000000 )

      bs_End( w )

      bs_WaitEvents( w )

   ENDDO

   bs_CloseWindow( w )

RETURN
