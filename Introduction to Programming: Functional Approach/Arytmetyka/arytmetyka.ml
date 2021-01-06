
(* Arytmetyka *)
(* Author: Michał Wiśniewski *)
(* Code Reviewer:  Kacper Szczepański gr. 6 *)


(* ----------------------------------------------------------- *)
(* All intervals can be presented as a closed interval: [l;r], *)
(* or a sum of two intervals: (-infinity; l] U [r; infinity)   *)
(* ----------------------------------------------------------- *)


(* interval consists of:  *)
(* l:float - left end of interval *)
(* r:float - right end of interval *)
(* closed:bool - whether the interval is a closed interval *)
(* or sum of intervals. *)
type interval = {l : float; r : float; closed : bool}



(* ----------------------- *)
(* ---- Helper values ---- *)
(* ----------------------- *)


(* [0.0; 0.0] interval *)
let zero_interval = {l = 0.0; r = 0.0; closed = true}


(* unspecified interval *)
let nan_interval = {l = nan; r = nan; closed = true}



(* -------------------------- *)
(* ---- Helper functions ---- *)
(* -------------------------- *)


(* Function that multiplies two float numbers *)
(* Used where multiplying 0.0 and infinity might occur, *)
(* then the result will be 0.0 *)
let mp (a : float) (b : float) = 
    if (a = 0.0) && (b = infinity) then 0.0
    else if (a = 0.0) && (b = neg_infinity) then 0.0
    else if (a = infinity) && (b = 0.0) then 0.0
    else if (a = neg_infinity) && (b = 0.0) then 0.0
    else a *. b


(* Returns true if 'a' is not a number, else false *)
let is_nan (a : float) =
    if (classify_float a) == FP_nan then true else false


(* Returns true if 'w' is an unspecified interval, else false *)
let is_nan_interval (w : interval) =
    if (is_nan w.l) || (is_nan w.r) then true else false


(* Returns true if the interval has positive and negative values, else false *)
let neg_and_pos (w : interval) = 
    if w.closed then
        if (w.l < 0.0) && (w.r > 0.0) then true else false
    else 
        true


(* Returns true if the interval has only negative values (with 0.0), else false *)
let only_neg (w : interval) = 
    if w.closed then
        if (w.l < 0.0) && (w.r <= 0.0) then true else false
    else
        false


(* Returns the smallest of 4 numbers *)
let min4 (a : float) (b : float) (c : float) (d : float) = 
    min a (min b (min c d))


(* Returns the largest of 4 numbers *)
let max4 (a : float) (b : float) (c : float) (d : float) = 
    max a (max b (max c d))


(* Returns reciprocal of number *)
(* Assuming, that 'a' is not 0.0 *)
let rcp (a : float) = 
    (1.0 /. a)


(* Returns a closed interval [a;b] or [b;a] if b > a *)
let closed_interval (a : float) (b : float) = 
    if a > b then
        {l = b; r = a; closed = true}
    else
        {l = a; r = b; closed = true}


(* Returns a sum of intervals: (-infinity;a] U [b;infinity)*)
(* or (-infinity;b] U [a;infinity) if b > a *)
(* or (-infinity;infinity) if a = b *)
let interval_sum (a : float) (b : float) =
    if a > b then
        {l = b; r = a; closed = false}
    else if a = b then
        {l = neg_infinity; r = infinity; closed = true}
    else
        {l = a; r = b; closed = false}



(* ------------------------ *)
(* ---- Core functions ---- *)
(* ------------------------ *)


(* Returns a closed interval [x - p%;x + p%] *)
let value_with_error (x : float) (p : float) = 
    closed_interval (x -. ((p /. 100.0) *. x)) (x +. ((p /. 100.0) *. x))


(* Returns true if value 'x' is in interval 'w', else false *)
let value_in_interval (w : interval) (x : float) = 
    if is_nan_interval w then 
        false 
    else
        if w.closed then
            if (x >= w.l) && (x <= w.r) then true else false
        else
            if (x <= w.l) || (x >= w.r) then true else false


(* Returns an interval with all numbers multiplied by -1 *)
let minus_interval (w : interval) = 
    if w.closed then
        closed_interval (-.(w.r)) (-.(w.l))
    else
        interval_sum (-.(w.r)) (-.(w.l))


(* Returns the smallest value of the interval, *)
(* or -infinity if there isn't the smallest value *)
(* or nan if interval is not specified *)
let min_value (w : interval) = 
    if is_nan_interval w then 
        nan 
    else
        if w.closed then w.l else neg_infinity


(* Returns the largest value of the interval, *)
(* or infinity if there isn't the largest value *)
(* or nan if interval is not specified *)
let max_value (w : interval) = 
    if is_nan_interval w then 
        nan 
    else
        if w.closed then w.r else infinity


(* Returns the middle value of the interval *)
(* or 'nan' if the interval is a sum of intervals or unspecified *)
let middle_value (w : interval) = 
    if (is_nan_interval w) || (not w.closed) then 
        nan 
    else 
        ((min_value w) +. (max_value w)) /. 2.0


(* Returns an interval where all elements are reciprocals *)
(* of numbers in 'w' interval. Assuming 'w' is not [0.0;0.0] *)
(* and reciprocal of 0.0 is infinity *)
let reciprocal_interval (w : interval) =
    let (a, b) = (w.l, w.r) in
        (* when 0.0 is in the interval *)
        if (value_in_interval w 0.0) then
            if w.closed then
                if a = neg_infinity then
                    if b = 0.0 then
                        closed_interval neg_infinity 0.0
                    else
                        interval_sum 0.0 (rcp b)

                else if b = infinity then
                    if a = 0.0 then
                        closed_interval 0.0 infinity
                    else
                        interval_sum (rcp a) 0.0

                else if a = 0.0 then
                    closed_interval (rcp b) infinity

                else if b = 0.0 then
                    closed_interval neg_infinity (rcp a)

                else
                    interval_sum (rcp a) (rcp b)
            else
                if a = 0.0 then
                    closed_interval neg_infinity (rcp b)

                else if b = 0.0 then
                    closed_interval (rcp a) infinity

                else
                    interval_sum (rcp b) (rcp a)
        (* when 0.0 is not in the interval *)
        else
            closed_interval (min (rcp a) (rcp b)) (max (rcp a) (rcp b))


(* Returns an interval, where all elements *)
(* are x + y, where x is in 'w1' and y is in 'w2' *)
let plus (w1 : interval) (w2 : interval) =
    (* when one interval is unspecified, the sum is always unspecified *)
    if (is_nan_interval w1) || (is_nan_interval w2) then 
        nan_interval 
    else
        if w1.closed && w2.closed then
            closed_interval (w1.l +. w2.l) (w1.r +. w2.r)

        else if (not w1.closed) && w2.closed then
            if (w2.r +. w1.l) >= (w2.l +. w1.r) then
                closed_interval neg_infinity infinity
            else
                interval_sum (w2.r +. w1.l) (w2.l +. w1.r) 

        else if w1.closed && (not w2.closed) then
            if (w1.r +. w2.l) >= (w1.l +. w2.r) then
                closed_interval neg_infinity infinity
            else
                interval_sum (w1.r +. w2.l) (w1.l +. w2.r) 

        else
            closed_interval neg_infinity infinity


(* Returns an interval, where all elements *)
(* are x-y, where x is in 'w1' and y is in 'w2' *)
let minus (w1 : interval) (w2 : interval) =
    (* when one interval is unspecified, the difference is always unspecified *)
    if (is_nan_interval w1) || (is_nan_interval w2) then 
        nan_interval
    else
        (* w1 - w2 is equivalent to w1 + (-w2) *)
        plus w1 (minus_interval w2)


(* Returns an interval, where all elements *)
(* are x * y, where x is in 'w1' and y is in 'w2' *)
let multiply (w1 : interval) (w2 : interval) = 
    (* when one interval is unspecified, the product is always unspecified *)
    if (is_nan_interval w1) || (is_nan_interval w2) then 
        nan_interval
    else
        let (a1, b1, a2, b2) = (w1.l, w1.r, w2.l, w2.r) in
            (* product, when one of intervals is [0.0;0.0] *)
            if (w1 = zero_interval) || (w2 = zero_interval) then
                closed_interval 0.0 0.0

            (* product when both intervals are closed *)
            else if (w1.closed) && (w2.closed) then
                closed_interval 
                    (min4 (mp a1 a2) (mp a1 b2) (mp b1 a2) (mp b1 b2)) 
                    (max4 (mp a1 a2) (mp a1 b2) (mp b1 a2) (mp b1 b2))

            (* product, when both intervals are sums of intervals *)
            else if (not (w1.closed)) && (not (w2.closed)) then
                if (value_in_interval w1 0.0) || (value_in_interval w2 0.0) then
                    closed_interval neg_infinity infinity
                else
                    interval_sum 
                        (max (a1 *. b2) (b1 *. a2)) 
                        (min (a1 *. a2) (b1 *. b2))   

            (* product when one interval is closed *)
            (* and the other one is sum of two intervals *)
            else
                (* changing order, so clsd is always closed and sm is always *)
                (* sum of two intervals *)
                let (clsd, sm) = 
                    if w1.closed then (w1, w2) else (w2, w1)
                in 
                    (* when values in clsd can be negative or positive *)
                    if neg_and_pos clsd then
                        closed_interval neg_infinity infinity

                    (* when values in clsd are only negative *)
                    else if only_neg clsd then
                        interval_sum 
                            (max (clsd.l *. sm.r) (clsd.r *. sm.r)) 
                            (min (clsd.l *. sm.l) (clsd.r *. sm.l))

                    (* when values in clsd are only positive *)
                    else
                        interval_sum 
                            (max (clsd.l *. sm.l) (clsd.r *. sm.l)) 
                            (min (clsd.l *. sm.r) (clsd.r *. sm.r))


(* Returns an interval, where all elements *)
(* are x / y, where x is in 'w1' and y is in 'w2' *)
let divide (w1 : interval) (w2 : interval) = 
    (* when one interval is unspecified, the quotient is always unspecified *)
    if (is_nan_interval w1) || (is_nan_interval w2) then 
        nan_interval
    else
        (* There are no elements, when all numbers are x / 0 *)
        if w2 = zero_interval then 
            closed_interval nan nan
        else
            (* w1 / w2 is equivalent to w1 * (1 / w2) *)
            multiply w1 (reciprocal_interval w2)




(* ----------------------------------- *)
(* Translating to task's specification *)
(* ----------------------------------- *)

type wartosc = interval

let wartosc_dokladnosc x p = value_with_error x p
let wartosc_od_do a b = closed_interval a b
let wartosc_dokladna x = closed_interval x x
    
let in_wartosc w x = value_in_interval w x
let min_wartosc w = min_value w
let max_wartosc w = max_value w
let sr_wartosc w = middle_value w
    
let plus w1 w2 = plus w1 w2
let minus w1 w2 = minus w1 w2
let razy w1 w2 = multiply w1 w2
let podzielic w1 w2 = divide w1 w2



(* ------*)
(* Tests *)
(* ----- *)

(* let a = wartosc_od_do (-1.0) 1.0           (* <-1, 1> *)
let b = wartosc_dokladna (-1.0)            (* <-1, -1> *)
let c = podzielic b a                     (* (-inf -1> U <1 inf) *)
let d = plus c a                          (* (-inf, inf) *)
let e = wartosc_dokladna 0.0               (* <0, 0> *)
let f = razy c e                          (* <0, 0> *)
let g = razy d e                          (* <0, 0> *)
let h = wartosc_dokladnosc (-10.0) 50.0     (* <-15, -5> *)
let i = podzielic h e                     (* nan, przedzial pusty*)
let j = wartosc_od_do (-6.0) 5.0            (* <-6, 5> *)
let k = razy j j                          (* <-30, 36> *)
let l = plus a b                          (* <-2, 0> *)
let m = razy b l                          (* <0, 2> *)
let n = podzielic l l                     (* <0, inf) *)
let o = podzielic l m                     (* (-inf, 0) *)
let p = razy o a                          (* (-inf, inf) *)
let q = plus n o                          (* (-inf, inf) *)
let r = minus n n                         (* (-inf, inf) *)
let s = wartosc_dokladnosc (-0.0001) 100.0 (* <-0.0002, 0> *)
let t = razy n s                        (* (-inf, 0) *)
;;
assert ((min_wartosc c, max_wartosc c) = (neg_infinity, infinity));
assert (is_nan (sr_wartosc c) );
assert (not (in_wartosc c 0.0));
assert ((in_wartosc c (-1.0)) && (in_wartosc c (-100000.0)) && (in_wartosc c 1.0) && (in_wartosc c 100000.0));
assert ((in_wartosc d 0.0) && (in_wartosc d (-1.0)) && (in_wartosc d (-100000.0)) && (in_wartosc d 1.0) && (in_wartosc d 100000.0));
assert ((min_wartosc f, max_wartosc f, sr_wartosc f) = (0.0, 0.0, 0.0));
assert ((min_wartosc g, max_wartosc g, sr_wartosc g) = (0.0, 0.0, 0.0));
assert ((min_wartosc h, max_wartosc h, sr_wartosc h) = (-15.0, -5.0, -10.0));
assert (is_nan (min_wartosc i) && is_nan (sr_wartosc i) && is_nan (max_wartosc i));
assert ((min_wartosc k, max_wartosc k, sr_wartosc k) = (-30.0, 36.0, 3.0));
assert ((min_wartosc n, max_wartosc n, sr_wartosc n) = (0.0, infinity, infinity));
assert ((min_wartosc o, max_wartosc o, sr_wartosc o) = (neg_infinity, 0.0, neg_infinity));
assert ((min_wartosc p, max_wartosc p, is_nan (sr_wartosc p)) = (neg_infinity, infinity, true));
assert ((min_wartosc q, max_wartosc q, is_nan (sr_wartosc q)) = (neg_infinity, infinity, true));
assert ((min_wartosc r, max_wartosc r, is_nan (sr_wartosc r)) = (neg_infinity, infinity, true));
assert ((min_wartosc t, max_wartosc t, sr_wartosc t) = (neg_infinity, 0.0, neg_infinity))

let a = wartosc_od_do neg_infinity infinity
let c = plus a a
let d = razy a a
let e = podzielic a a
let f = minus a a
;;
assert ((min_wartosc c, max_wartosc c, is_nan (sr_wartosc c)) = (neg_infinity, infinity, true));
assert ((min_wartosc d, max_wartosc d, is_nan (sr_wartosc d)) = (neg_infinity, infinity, true));
assert ((min_wartosc e, max_wartosc e, is_nan (sr_wartosc e)) = (neg_infinity, infinity, true));
assert ((min_wartosc d, max_wartosc d, is_nan (sr_wartosc d)) = (neg_infinity, infinity, true))

let a = wartosc_od_do 0.0 infinity
let b = wartosc_dokladna 0.0
let c = podzielic a b
let d = podzielic b b
;;
assert ((is_nan(min_wartosc c), is_nan(max_wartosc c), is_nan (sr_wartosc c)) = (true, true, true));
assert ((is_nan(min_wartosc d), is_nan(max_wartosc d), is_nan (sr_wartosc d)) = (true, true, true))

let a = wartosc_od_do (-10.0) 10.0
let b = wartosc_od_do (-1.0) 1000.0
let c = podzielic a b
;;
assert ((min_wartosc c, max_wartosc c, is_nan (sr_wartosc c)) = (neg_infinity, infinity, true)) *)