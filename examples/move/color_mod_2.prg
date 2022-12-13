/*
 *
 */

#include "hbbluesea.ch"

#define WIDTH  9
#define HEIGHT 18

PROCEDURE Main()

   LOCAL w
   LOCAL mod, col, row

   w := bs_CreateWindow( 830, 450, "bs_RectFilled( w, x, y, width, height, radius, hexColor )" )

   WHILE( ! bs_MainLoop( w ) .AND. ! bs_GetKey( w, KEY_ESCAPE ) )

      FOR mod := 0 TO 3

         bs_Begin( w )

            bs_Background( w, 0x006994 )

               FOR row := 0 TO 24
                  FOR col := 0 TO 91
                     bs_RectFilled( w, col * WIDTH, row * HEIGHT, WIDTH, HEIGHT, 0, Color( col, row + mod ) )
                  NEXT
               NEXT

         bs_WaitEventsTimeout( w, 1.0 )
         bs_End( w )

      NEXT

   ENDDO

   bs_CloseWindow( w );

   RETURN

STATIC FUNCTION Color( col, row )

   SWITCH Int( row % 4 * 10 + col % 4 )
   CASE  2
   CASE 11
   CASE 20
   CASE 33 ; RETURN 0xffffff
   CASE  0
   CASE 13
   CASE 22
   CASE 31 ; RETURN 0xff0000
   ENDSWITCH

   RETURN 0x323232
