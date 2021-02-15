(** sde1_sp2020_given_functions.caml  
    4 GIVEN FUNCTIONS FOR CODE READING AND POSSIBLE REUSE
    Hopfield ocaml simulation/extension for Sp2020 *)

open List;;

let rec inner = function (i, w) ->
if i==[] && w == [] then 0.0
 else 
 (hd i) *. (hd w) 
     +. inner((tl i), (tl w));;

let rec netUnit = function (inputs, weights) ->
if inputs==[] && weights == [] then 0.0
 else 
((hd inputs) *. (hd weights))
     +. netUnit((tl inputs), (tl weights));;

let rec netAll = function (state, weightMatrix) ->
if weightMatrix==[] then []
 else 
netUnit (state,(hd weightMatrix))
     :: netAll(state, (tl weightMatrix));;

let hop11Activation = function (net,oldo) ->
 if (net > 0.0) then 1.0
                  else if (net = 0.0) then oldo
                                         else -1.0;;

let rec hop11ActAll = function (allNet,oldState) ->
if (allNet==[]) then []
                else
                hop11Activation((hd allNet),(hd oldState)) :: 
                     hop11ActAll((tl allNet),(tl oldState));;
