
MAIN: add    r3    LIST

add ,   r3  ,  LIST
 
add  r3  ,  ,  LIST

add ,  , r3    LIST
 
add  r3 ,LIST fd

add  r3 ,LIST ,r4
 
add  r3


add  r1 		,r3 ,



STR: .string "abcd" fdf


MAIN: add    r3 ,   LIST

9main: prn #7

string: sub r1, r4

macr        Test
cmp r3, #-6
bne END
   endmacr
Test: sub r1, r4

LIST: .data 6, -9
    .data -100
    .extern k

LIST: .data 8