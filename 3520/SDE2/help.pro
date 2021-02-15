/**************************************
General Functions
**************************************/

decrementVal(X, Y):-
	Y is X - 1.
addVals(X, Y, Out):-
	Out is X + Y.

/**************************************
Hopfield (-1,1) training function(s)
this computes weight matrix for only one stored state
**************************************/

checkH(H, X):-
	H > 0.0,
	X = 0.0;
	H < 0.0,
	X = -0.0.

hopHelper(UsedHeads, [H|[]], W):-
	checkH(H, Zero),
	X = [Zero],
	append(UsedHeads, X, A),
	createWeight(H, A, WO),
	W = [WO], !.

hopHelper(UsedHeads, [H|T], W):-
	checkH(H, Zero),
	X = [Zero|T] ,
	append(UsedHeads, X, Start) ,
	append(UsedHeads, [H], Heads),
	createWeight(H, Start, WO),
	hopHelper(Heads, T, Y),
	W = [WO|Y].


createWeight(_, [], []).
createWeight(InputHead, [H|T], [Unit|R]):-
	Unit is InputHead * H,
	createWeight(InputHead, T, R),
	!.

hopTrainAstate(S, W) :-
	hopHelper(_, S, W), !.


/**************************************
Hopfield weight matrix for a list of desired stored states
**************************************/

/* add a list */
addList([H|[]], [H2|[]], X):-
	addVals(H, H2, Y),
	X = [Y], !.

addList([H|T], [H2|T2], X):-
	addVals(H, H2, Y),
	addList(T, T2, Result),
	X = [Y|Result], !.

/* add a 2 dimensional list ie matrix */
addMatrix([H|[]], [H2|[]], W):-
	addList(H, H2, X),
	W = [X].

addMatrix([H|T], [H2|T2], W):-
	addList(H, H2, X),
	addMatrix(T, T2, Y),
	W = [X|Y].

hopTrain([H|[]], W):-
	hopTrainAstate(H, W), !.

hopTrain([H|T], W):-
	hopTrainAstate(H, X),
	hopTrain(T, Y),
	addMatrix(X, Y, W), !.
hopTrain([],_).


/**************************************
Next state computation
**************************************/

/*used for when the tail is empty. */
nextState([H|T], [H2|[]], Out) :-
	netUnit([H|T], H2, Unit),
	hop11Activation(Unit, H, Y),
	Out = [Y], !.

/*generates next state until tail empty */
nextState([H|T], [H2|T2], Out) :-
	netUnit([H|T], H2, Unit),
	hop11Activation(Unit, H, State),
	nextState([H|T], T2, Next),
	append([State], Next, Out).

/**************************************
Energy
**************************************/

energy(S, W, Energy) :-
	netAll(S, W, OutAll),
	netUnit(OutAll, S, Unit),
	Energy is ((-0.5) * Unit), !.



/**************************************
  Update network state N iterations (N=0 is current state)
*************************************/

updateN(S, W, N, Result) :-
	N > 0,
	nextState(S, W, Out),
	decrementVal(N, Count),
	updateN(Out, W, Count, Result), !;
	Result = S.

/**************************************
findsEquilibrium succeeds if finds equilibrium in N state transition checks;
fails otherwise
**************************************/

findsEquilibrium(S, W, Dis) :-
	Dis > 0,
	decrementVal(Dis, PDis),
	updateN(S, W, Dis , N1),
	updateN(S, W, PDis, N2),
	N1 == N2, !.
