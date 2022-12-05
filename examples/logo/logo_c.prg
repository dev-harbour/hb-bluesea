#include "hbbluesea.ch"

PROCEDURE Main()

   LOCAL w
   LOCAL array
   LOCAL x, y, x1, y1, x2, y2
   LOCAL a
   LOCAL r := 200
   LOCAL i

   w := bs_CreateWindow( 680, 450, "Logo C" )

   bs_FreeType( w, "../../font/9x18.bdf" )

   array := hb_ATokens( hb_MemoRead( "logo_c.prg" ), .T. )

   WHILE( ! bs_MainLoop( w ) .AND. ! bs_GetKey( w, KEY_ESCAPE ) )

      bs_Begin( w )

         bs_Background( w, 0x530972 )

         x := bs_WinWidth( w ) / 2
         y := bs_WinHeight( w ) / 2

         FOR i := 1 TO Len( array )
            bs_Text( w, array[ i ], 10, i * 18, 0xb42aee )
         NEXT

         a := 2 * M_PI / 6

         bs_Hexagon( w, x, y, r + 10, 0xfaf2fe )
         bs_HexagonFilled( w, x, y, r, 0x5c6bbf )
         FOR i := 0 TO 1
            x1 := x + r * cos( a * i + M_PI / 6 )
            y1 := y + r * sin( a * i + M_PI / 6 )
            x2 := x + r * cos( a * ( i + 1 ) + M_PI / 6 )
            y2 := y + r * sin( a * ( i + 1 ) + M_PI / 6 )
            bs_TriangleFilled( w, x, y, x1, y1, x2, y2, 0x283492 )
         NEXT

         bs_CircleLineWidth( w, x, y, 100, 66, 0xffffff )

         x1 := x + r * cos( a * 5 + M_PI / 6 )
         y1 := y + r * sin( a * 5 + M_PI / 6 )
         x2 := x + r * cos( a * ( 5 + 1 ) + M_PI / 6 )
         y2 := y + r * sin( a * ( 5 + 1 ) + M_PI / 6 )
         bs_TriangleFilled( w, x, y, x1, y1, x2, y2, 0x3848aa )

      bs_End( w )

      bs_WaitEvents( w )
   ENDDO

   bs_CloseWindow( w )

   RETURN
