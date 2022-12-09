/*
 *
 */

#include "hbbluesea.ch"

PROCEDURE Main()

   LOCAL w
   LOCAL mod, col, row
   LOCAL c

   w = bs_CreateWindow( 830, 450, "Ascii characters box drawing" )

   bs_FreeType( w, "../../font/9x18.bdf" )

   WHILE( ! bs_MainLoop( w ) .AND. ! bs_GetKey( w, KEY_ESCAPE ) )

      bs_Begin( w )

         bs_Background( w, 0x006994 )

         FOR mod := 0 TO 3
            FOR row := 0 TO 24
               FOR col := 0 TO 91
                  DO CASE
                  CASE col == 0  .AND. row == 0  ; c := "┌"
                  CASE col == 0  .AND. row == 24 ; c := "└"
                  CASE col == 0                  ; c := "├"
                  CASE col == 91 .AND. row == 0  ; c := "┐"
                  CASE col == 91 .AND. row == 24 ; c := "┘"
                  CASE col == 91                 ; c := "┤"
                  CASE                 row == 0  ; c := "┬"
                  CASE                 row == 24 ; c := "┴"
                  OTHERWISE                      ; c := "┼"
                  ENDCASE
                  bs_DispOutAt( w, c, col, row, Color( col, row + mod ) )
               NEXT
            NEXT
         NEXT

      bs_End( w )

      bs_PollEvents( w )
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
