/** Reagan Leonard
  * CPSC 3520
  * SDE 2
  * Hopfield Network in Prolog
  * Apr. 9, 2020 */

/** The following predicates are contained in the given functions file
    so we can use them from here on:
  * netUnit
  * netAll
  * hop11Activation
  * hop11ActAll                                                       */

/***************************************************************************/
/*Next state computation
  This function is the next state computation. It returns next state vector.
  It produces a float list.
  nextState(+CurrentState,+WeightMatrix,-Next)
*/

/*If tail is empty. */
nextState([H|T], [H2|[]], Out) :-
    netUnit([H|T], H2, Unit),
    hop11Activation(Unit, H, Y),
    Out = [Y], !.

/*Creates nextState until tail is empty */
nextState([H|T], [H2|T2], Out) :-
    netUnit([H|T], H2, Unit),
    hop11Activation(Unit, H, State),
    nextState([H|T], T2, Next),
    append([State], Next, Out).
/***************************************************************************/

/***************************************************************************/
/*Update network state N iterations (N=0 is current state)
  This function returns the network state after n time steps (i.e. update of N time steps).
  It produces a float list.
  updateN(+CurrentState,+WeightMatrix,+N,-ResultState)
*/

updateN(State, Weights, N, Result) :-
    N > 0,
    nextState(State, Weights, Out),
    xMinusOne(N, Count),
    updateN(Out, Weights, Count, Result), !;
    Result = State.
/***************************************************************************/

/***************************************************************************/
/*Succeeds if finds equilibrium, fails otherwise.
  This function returns true if the network reaches an equilibrium state within range steps and returns false otherwise.
  findsEquilibrium(+InitialState,+WeightMatrix,+Range)
*/
findsEquilibrium(State, Weights, Range) :-
    Range > 0,
    xMinusOne(Range, RangeMinus),
    updateN(State, Weights, Range, N1),
    updateN(State, Weights, RangeMinus, N2),
    N1 == N2, !.
/***************************************************************************/

/***************************************************************************/
/*determines state energy, i.e., implements Equation (2)
  This function implements the Hopfield Energy equation.
  It returns a float.
  energy(+State,+WeightMatrix,-E)
*/
energy(State, Weights, Energy) :-
    netAll(State, Weights, Out),
    netUnit(Out, State, Unit),
    Energy is ((-0.5) * Unit), !.
/***************************************************************************/

/***************************************************************************/
/*Computes weight matrix for only one (i.e., a single) stored state
  This function returns weight matrix for only one stored state, used as a ’warmup’ for the next function.
  It produces a float list list.
  hopTrainAstate(+Astate,-WforState)
*/
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
    X = [Zero|T],
    append(UsedHeads, X, Start),
    append(UsedHeads, [H], Heads),
    createWeight(H, Start, WO),
    hopHelper(Heads, T, Y),
    W = [WO|Y].

createWeight(_, [], []).

createWeight(InputHead, [H|T], [Unit|R]):-
    Unit is InputHead * H,
    createWeight(InputHead, T, R), !.

hopTrainAstate(State, Weights) :-
    hopHelper(_, State, Weights), !.
/***************************************************************************/

/***************************************************************************/
/*Computes Hopfield weight matrix for a list of desired stored states
  This returns a weight matrix, given a list of stored states (shown previously) using Eqns (4) and (5).
  It produces a float list list.
  hopTrain(+ListofStates,-WeightMatrix)
*/

/* add two lists together */
addList([H|[]], [H2|[]], X):-
    add(H, H2, Y),
    X = [Y], !.

addList([H|T], [H2|T2], X):-
    add(H, H2, Y),
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
/***************************************************************************/

/*Additional helpers*/
xMinusOne(X, Y):-
    Y is X - 1.
add(X, Y, Z):-
    Z is X + Y.
