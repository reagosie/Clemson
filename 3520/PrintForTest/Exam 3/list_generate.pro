/* list generate example (list_generate.pro) */
/* does not check if N>0 */

theList(0,[]) :- !.

theList(N,[N|T]) :- NM1 is N-1, theList(NM1,T).

/*
?- ['list_generate.pro'].
true.

?- theList(10,Where).
Where = [10, 9, 8, 7, 6, 5, 4, 3, 2|...] .

?- theList(10,Where),write(Where).
[10,9,8,7,6,5,4,3,2,1]
Where = [10, 9, 8, 7, 6, 5, 4, 3, 2|...] .
*/
