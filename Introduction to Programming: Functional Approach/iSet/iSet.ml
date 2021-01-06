(*
 * ISet - Interval sets
 * Copyright (C) 1996-2003 Xavier Leroy, Nicolas Cannasse, Markus Mottl,
 * Jacek Chrzaszcz, Michał Wiśniewski
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version,
 * with the special exception on linking described in file LICENSE.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARsetICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *)

(** Interval Set.

    This is an interval set, i.e. a set of integers, where large
    intervals can be stored as single elements. Intervals stored in the
    set are disjoint. 
*)

(* Modified by: Michał Wiśniewski *)
(* Code reviewer: Tymoteusz Wiśniewski *)


(* Interval consists of two numbers *)
type interval = {a : int; b : int}


(* Set consists of left subset, interval, right subset *)
(* height of the set and how many elements are there in *)
(* it's left and right subsets *)
type t = 
    | Empty
    | Set of {lset : t; intv : interval; rset : t; h : int; sums : interval} 


(* safe addition of numbers *)
let plus a b = 
  if (a >= 0) && (b >= 0) then
    if a + b < 0 then
      max_int 
    else 
      a + b
  else 
    a + b


(* absolute difference between numbers *)
let diff_abs a b = 
  if b = min_int then
    plus (plus a 1) max_int
  else
    plus a (-b)


(* height of the set *)
let height s = 
  match s with 
    | Set(set) -> set.h
    | Empty -> 0


(* number of elements in Set's interval *)
let num_in_interval s = 
  match s with
  | Empty -> 0
  | Set(set) -> plus (diff_abs set.intv.b set.intv.a) 1


(* number of elements in Set and it's subsets *)
let num_in_set s = 
  match s with
    | Set(set) -> plus (num_in_interval s) (plus set.sums.a set.sums.b)
    | Empty -> 0


(* make a set *)
let make l k r = 
  Set {lset = l; intv = k; rset = r; h = (max (height l) (height r) + 1); 
       sums = {a = (num_in_set l); b = (num_in_set r)}}


(* balance the set *)
let bal l k r =
  let hl = height l in
  let hr = height r in
  if hl > hr + 2 then
    match l with
    | Set(lSet) ->
        if (height lSet.lset) >= (height lSet.rset) then
          make (lSet.lset) (lSet.intv) (make lSet.rset k r)
        else
          (match lSet.rset with
          | Set (lrSet) ->
              make 
              (make lSet.lset lSet.intv lrSet.lset) 
              (lrSet.intv) 
              (make lrSet.rset k r)
          | Empty -> assert false)
    | Empty -> assert false
  else if hr > hl + 2 then
    match r with
    | Set (rSet) ->
        if (height rSet.rset) >= (height rSet.lset) then 
          make (make l k rSet.lset) rSet.intv rSet.rset
        else
          (match rSet.lset with
          | Set (rlSet)->
              make 
              (make l k rlSet.lset) 
              (rlSet.intv) 
              (make rlSet.rset rSet.intv rSet.rset)
          | Empty -> assert false)
    | Empty -> assert false
  else 
    Set {lset = l; intv = k; rset = r; h = max hl hr + 1; 
         sums = {a = (num_in_set l); b = (num_in_set r)}}


(** [mem x s] returns [true] if [s] contains [x], and [false] otherwise. *)
let rec mem x s =
  match s with
    | Empty -> false
    | Set (set) ->
      if x < set.intv.a then
        mem x set.lset
      else if x > set.intv.b then
        mem x set.rset
      else
        true


(* return set with values smaller than x *)
let rec split_lesser x s =
  match s with
  | Empty -> Empty
  | Set (set) ->
  if x < set.intv.a then
    split_lesser x set.lset
  else if x = set.intv.a then
    set.lset
  else if x > set.intv.b then
    bal set.lset {a = set.intv.a; b = set.intv.b} (split_lesser x set.rset)
  else if x = set.intv.b then
    bal set.lset {a = set.intv.a; b = (plus set.intv.b (-1))} Empty
  else
    bal set.lset {a = set.intv.a; b = (max (plus x (-1)) set.intv.a)} Empty


(* return set with values greater than x *)
let rec split_greater x s =
  match s with
  | Empty -> Empty
  | Set(set) ->
  if x > set.intv.b then
    split_greater x set.rset
  else if x = set.intv.b then
    set.rset
  else if x < set.intv.a then
    bal (split_greater x set.lset) {a = set.intv.a; b = set.intv.b} set.rset 
  else if x = set.intv.a then
    bal Empty {a = (plus set.intv.a 1); b = set.intv.b} set.rset
  else
    bal Empty {a = (min (plus x 1) set.intv.b); b = set.intv.b} set.rset


(** [split x s] returns a triple [(l, present, r)], where
    [l] is the set of elements of [s] that are strictly lesser than [x];
    [r] is the set of elements of [s] that are strictly greater than [x];
    [present] is [false] if [s] contains no element equal to [x],
    or [true] if [s] contains an element equal to [x]. *)
let split x s =
  let l = split_lesser x s
  and r = split_greater x s
  in (l, mem x s, r)


(* merge 'numb', if possible, with interval in set, return merged interval *)
(* and set without this interval *)
(* assuming that all intervals have values lesser than numb *)
let rec merge_left numb s =
  match s with
    | Empty -> (Empty, numb)
    | Set(set) ->
      if set.rset = Empty then
        if (diff_abs numb set.intv.b) = 1 then
          (set.lset, set.intv.a)
        else
          (bal set.lset {a = set.intv.a; b = set.intv.b} set.rset, numb)
      else
        let (resulset_set, n) = merge_left numb set.rset 
        in ((bal set.lset {a = set.intv.a; b = set.intv.b} resulset_set), n)


(* merge 'numb', if possible, with interval in set, return merged interval *)
(* and set without this interval *)
(* assuming that all intervals have values greater than numb *)
let rec merge_right numb s =
  match s with
    | Empty -> (Empty, numb)
    | Set (set)->
      if set.lset = Empty then  
        if (diff_abs set.intv.a numb) = 1 then
          (set.rset, set.intv.b)
        else
          (bal set.lset {a = set.intv.a; b = set.intv.b} set.rset, numb)
      else
        let (resulset_set, n) = merge_right numb set.lset in
        ((bal resulset_set {a = set.intv.a; b = set.intv.b} set.rset), n)


(** [add (x, y) s] returns a set containing the same elements as [s],
    plus all elements of the interval [[x,y]] including [x] and [y].
    Assumes [x <= y]. *)
let add (a, b) s =
  let less = split_lesser a s
  and great = split_greater b s in
  let (lset, ln) = merge_left a less
  and (rset, rn) = merge_right b great
  in
    bal lset {a = ln; b = rn} rset


(* return smallest interval in set and set without this interval *)
let rec smallest_inv s =
  match s with
    | Set(set) ->
      if set.lset = Empty then
        ({a = set.intv.a; b = set.intv.b}, set.rset)
      else
        let (sm_intv, sm_set) = smallest_inv set.lset in
          (sm_intv, (bal sm_set {a = set.intv.a; b = set.intv.b} set.rset))
    | Empty -> assert false


(** [remove (x, y) s] returns a set containing the same elements as [s],
    except for all those which are included between [x] and [y].
    Assumes [x <= y]. *)
let remove (x, y) s =
  let (less_set, great_set) = (split_lesser x s, split_greater y s) in
  match (less_set, great_set) with
    | (Empty, Empty) -> Empty
    | (Empty, Set (set)) ->
      bal set.lset {a = set.intv.a; b = set.intv.b} set.rset
    | (Set (set), Empty) ->
      bal set.lset {a = set.intv.a; b = set.intv.b} set.rset
    | (Set (set1), Set (set2)) ->
      let (interv, rseset) = smallest_inv great_set in
        bal less_set interv rseset


(** Return the list of all continuous intervals of the given set.
    The returned list is sorted in increasing order. *)
let elements s = 
  let rec loop acc s = 
    match s with
    | Empty -> acc
    | Set (set) -> loop ((set.intv.a, set.intv.b) :: loop acc set.rset) set.lset in
  loop [] s


(** [below n s] returns the number of elements of [s] that are lesser
    or equal to [n]. If there are more than max_int such elements, 
    the result should be max_int. *)
let below numb s = 
  let rec _below acc numb s = 
    match s with 
      | Empty -> acc
      | Set (set) ->
      if numb < set.intv.a then
        _below acc numb set.lset
      else if numb > set.intv.b then
        let hsum = plus (plus (diff_abs set.intv.b set.intv.a) 1) set.sums.a in
        _below (plus acc hsum) numb set.rset 
      else
          (plus acc (plus (plus (diff_abs numb set.intv.a) 1) set.sums.a) )
  in _below 0 numb s


(** [iter f s] applies [f] to all continuous intervals in the set [s].
    The intervals are passed to [f] in increasing order. *)
let iter f s=
  let rec loop s = 
    match s with
    | Empty -> ()
    | Set (set) -> loop set.lset; f (set.intv.a, set.intv.b); loop set.rset in
  loop s


(** [fold f s a] computes [(f xN ... (f x2 (f x1 a))...)], where x1
    ... xN are all continuous intervals of s, in increasing order. *)
let fold f s acc =
  let rec loop acc s = 
    match s with
    | Empty -> acc
    | Set (set) ->
          loop (f (set.intv.a, set.intv.b) (loop acc set.lset)) set.rset in
  loop acc s


(** The empty set *)
let empty = Empty


(** returns true if the set is empty. *)
let is_empty s =
  s = Empty



(* tests *)

(* 

let info = false;;

let simple l =
  let (e, res) =
    List.fold_left (fun ((px, py), la) (x, y) ->
      if py + 1 >= x then ((px, max py y), la)
      else ((x, y), (px, py)::la)) ((List.hd l), []) (List.tl l)
  in
  List.rev (e::res);;

let long l =
  let rec add_inter acc (x, y) =
    if x == y then x::acc
    else add_inter (x::acc) (x + 1, y)
  in
  List.rev (List.fold_left (fun acc inter -> (add_inter [] inter) @ acc) [] l);;
  
let add_list =
  List.fold_left (fun s x -> add x s);;

let mem_all a l1 =
  List.filter (fun x -> not (mem x a)) l1 = []

let mem_none a l1 =
  List.filter (fun x -> mem x a) l1 = []

(* Small correctness tests *)

let l1 = [(-10, -8); (-7, -7); (-4, -1); (1, 1); (3, 7); (10, 15); (100, 1000)];;
let a = add_list empty l1;;

assert(elements a = simple l1);;
assert(mem_all a (long l1));;
assert(below 1000 a = 921);;

let (a1, b, a2) = split 4 a;;
assert(b);;
assert(simple (elements a1 @ [(4, 4)] @ elements a2) = simple l1);;
assert(List.filter (fun (x, y) -> y >= 4) (elements a1) = []);;
assert(List.filter (fun (x, y) -> x <= 4) (elements a2) = []);;

let (a1, b, a2) = split 3 a;;
assert(b);;
assert(simple (elements a1 @ [(3, 3)] @ elements a2) = simple l1);;
assert(List.filter (fun (x, y) -> y >= 3) (elements a1) = []);;
assert(List.filter (fun (x, y) -> x <= 3) (elements a2) = []);;

let (a1, b, a2) = split 2 a;;
assert(not b);;
assert(simple(elements a1 @ elements a2) = simple l1);;
assert(List.filter (fun (x, y) -> y >= 2) (elements a1) = []);;
assert(List.filter (fun (x, y) -> x <= 2) (elements a2) = []);;

let b = add (1, 10) a;;
let l2 = List.sort (fun (x1, _) (x2, _) -> compare x1 x2) ((1, 10)::l1);;

assert(elements b = simple l2);;

let c = remove (1, 10) a;;
let d = remove (1, 10) b;;

assert(elements c = elements d);;

let e = add (min_int, max_int) a;;
assert(elements e = [(min_int, max_int)]);;
assert(below 1 e = max_int);;

let f = remove (min_int, max_int) a;;
assert(elements f = []);;

let l3 = [(16, 99); (2, 2); (8, 9); (-6, -5)];;
let g = add_list a l3;;
assert(elements g = [(-10, -1); (1, 1000)]);;
assert(not (mem 0 g));;
let h = remove (420, 690) g;;
assert(not (mem 500 h));;
assert(elements h = [(-10, -1); (1, 419); (691, 1000)]);;
let i = add (0, 0) g;;
assert(elements i = [(-10, 1000)]);;
let j = remove (-9, -1) i;;
assert(elements j = [(-10, -10); (0, 1000)]);;
let k = remove (500, 999) j;;
assert(elements k = [(-10, -10); (0, 499); (1000, 1000)]);; *)



