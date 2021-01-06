(* Topol Sort *)
(* Author: Michał Wiśniewski *)
(* Code Reviewer: Mikołaj Grzebieluch *)


open List


(** Exception raised when graph in [topol] is cyclic. *)
exception Cykliczne


(** Status of node in graph. *)
type status = Processing | Processed


(** Bound values in map [m] to nodes as specified in [lst]: value -> list of values. *)
let rec fill_map lst m =
  if lst = [] then m
  else
    match (hd lst) with (value, paths) ->
      if (PMap.mem value m) then
        fill_map (tl lst) (PMap.add value (paths @ (PMap.find value m)) m)
      else
        fill_map (tl lst) (PMap.add value paths m)


(** Fill map [m] with values that were absent in nodes of [lst] but were pointed to. *)
let rec fill_missing lst m = 
  if lst = [] then m
  else
    (** Bound values in [nodes_list] to empty lists if absent in [m]. *)
    let rec helper nodes_list m =
      if nodes_list = [] then m
      else
        if (PMap.mem (hd nodes_list) m) then
          helper (tl nodes_list) m
        else
          helper (tl nodes_list) (PMap.add (hd nodes_list) [] m)
    in
      match (hd lst) with (_, nodes_list) ->
        fill_missing (tl lst) (helper nodes_list m)


(** Convert list [lst] describing a graph into a map where
    value in node is bounded to list of it's neighbours. *)
let convert_to_map lst =
  let filled_map = fill_map lst PMap.empty
  in
    fill_missing lst filled_map


(** For a given list [(a_1,[a_11;...;a_1n]); ...; (a_m,[a_m1;...;a_mk])] 
    return a list, where each of elements a_i and a_ij occurs only once
    and each element a_i is before a_i1 ... a_il elements. *)
let topol lst =
  let graph_map = ref (convert_to_map lst) in
  let status_map = ref PMap.empty in
  let result = ref [] in
  (** Recurrent dfs function, which takes [value] and call itself for
      all it's neighbours specified in [graph_map].
      When graph specified in [graph_map] is cyclic, [Cykliczne] is raised.
      Function adds the [value] to [result] list before all it's neighbours. *)
  let rec dfs value =
      if (PMap.mem value !status_map) &&
         ((PMap.find value !status_map) = Processing) then
              raise Cykliczne
      else if not (PMap.mem value !status_map) then
          begin
              status_map := PMap.add value Processing !status_map;
              List.iter dfs (PMap.find value !graph_map);
              status_map := PMap.add value Processed !status_map;
              result := value :: (!result);
          end
  in
      begin
          List.iter (function (value, _) -> dfs value) lst;
          !result;
      end






(* TESTS *)

(* Autor: Agnieszka Świetlik
 * Licence: Unlicensed
 * Original repo: https://github.com/aswietlik/wpf *)

(* let zle = ref 0
let test n b =
  if not b then begin
    Printf.printf "Zly wynik testu %d!!\n" n;
    incr zle
  end

let isOk input output =
	let rec where a = function
		| [] -> []
		| h::t -> if h = a then h::t else (where a t) in 
	let rec length used = function
		| [] -> List.length used
		| (h, l1)::t -> let newOne used a = 
					if (where a used) = [] then a::used else used
				in length (newOne (List.fold_left newOne used l1) h) t in
	let size = length [] input in
	let rec find acc wh = function
		| [] -> acc
		| h::t -> acc && ((where h wh) <> []) && (find acc wh t) in
	let rec pom acc = function
		| [] -> acc
		| (a, l)::t -> acc && find acc (where a output) l && pom acc t
	in (size = List.length output) && (pom true input);;

let genTest n =
	let res = ref [] in
	for i = 1 to (n-1) do
		if Random.int 10 < 6 then
		let l = ref [] and m = Random.int (n - i) in
		let used = Array.make (n+1) false in
		let unUsed _ =
			let getInt n = min n ((Random.int (max (n - i - 1) 1)) + i + 1) in
			let a = ref (getInt n) in
			while used.(!a) = true do
				if (!a) = n then a := (i + 1) else a := (!a) + 1;
			done; used.(!a) <- true; !a in
		for j = 0 to m do
			l := (unUsed j)::(!l)
		done;
		res := (i, !l)::(!res)
	done; (!res);;


(* Printf.printf "=== Podstawowe...\n";; *)

let a = [];;
let b = [(1, [2;3]);(3, [6;7]);(6, [4;7]);(7, [5]);(2, [5]);(8, [])];;
let c = [(5, []);(2, [])];;
let d = [];;

test 1 (isOk a (topol a));;
test 2 (isOk b (topol b));;
test 3 (isOk c (topol c));;
test 4 (isOk d (topol d));;

(* Printf.printf "=== Inne typy...\n";; *)

let a = [("a", ["c"]);("e", ["g"]);("f", ["a";"e"]);("g", ["c";"a"])];;
let b = [(false, [true])];;
let c = [("z", ["c"; "f"; "a"]);("f", ["x"; "a"]);("g", ["h"])];;
let d = [("xx", ["aa"; "gg"]);("ab", ["uw"; "mim"]);("mim", ["uw";"xx"])];;
let e = [("d", ["c"]);("c", ["b"]);("b", ["a"])];;

test 25 (isOk a (topol a));;
test 26 (isOk b (topol b));;
test 27 (isOk c (topol c));;
test 28 (isOk d (topol d));;
test 29 (isOk e (topol e));;


(* Printf.printf "=== Cykliczne..\n";; *)

let a = [("a", ["b"]);("b", ["a"]);("c", ["a"])];;
let a = try topol a with 
	| Cykliczne -> [];;

let b = [("a", ["a"])];;
let b = try topol b with
	| Cykliczne -> [];;

let c = [(1, [4; 5]);(3, [2]);(2, [3])];;
let c = try topol c with
	| Cykliczne -> [];;

let d = [(1, [2]);(2, [3; 4]);(3, [5; 6]);(6, [2])];;
let d = try topol d with
	| Cykliczne -> [];;

test 50 (a = []);;
test 51 (b = []);;
test 52 (c = []);;
test 53 (d = []);;

(* Printf.printf "=== Losowe..\n";; *)

for i = 100 to 500 do
	let a = genTest 30 in
	test i (isOk a (topol a));
done;;

let _ = 
  if !zle <> 0 then Printf.printf "\nBlednych testow: %d...\n" !zle
;; *)

