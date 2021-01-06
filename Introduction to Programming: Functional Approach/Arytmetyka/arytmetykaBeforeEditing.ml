(* Arytmetyka *)
(* Author: Michał Wiśniewski *)




type value = { lewy : float ; prawy : float ; laczny : bool};;

let mp a b = 
    if (a = 0.)&&(b = infinity) then 0.
    else if (a = 0.)&&(b = neg_infinity) then 0.
    else if (a = infinity)&&(b = 0.) then 0.
    else if (a = neg_infinity)&&(b = 0.) then 0.
    else a *. b;;

let zero = {lewy = 0. ; prawy = 0. ; laczny = true};;

let nan_interval = { lewy = nan; prawy = nan; laczny = true};;

let is_nan w =
    let (a, b) = (w.lewy, w.prawy)
    in
        if ((classify_float a) == FP_nan) || ((classify_float b) == FP_nan) then true else false;;

let neg_and_pos w = 
    if w.laczny then
        if (w.lewy < 0.) && (w.prawy > 0.) then true else false
    else true;;

let only_neg w = 
    if w.laczny then
        if (w.lewy < 0.) && (w.prawy <= 0.) then true else false
    else
        false;;

let min4 a b c d = 
    min a (min b (min c d));;

let max4 a b c d = 
    max a (max b (max c d));;

let przedzial_laczny a b = 
    if a > b then
        {lewy = b ; prawy = a ; laczny = true}
    else
        {lewy = a ; prawy = b ; laczny = true};;

let przedzial_rozlaczny a b =
    if a > b then
        {lewy = b ; prawy = a ; laczny = false}
    else if a = b then
        {lewy = neg_infinity ; prawy = infinity; laczny = true}
    else
        {lewy = a ; prawy = b ; laczny = false};;

let przedzial_przeciwny w = 
    if w.laczny then
        przedzial_laczny (-.(w.prawy)) (-.(w.lewy))
    else
        przedzial_rozlaczny (-.(w.prawy)) (-.(w.lewy));;

let value_dokladnosc x p = 
    przedzial_laczny ((x-.((p/.100.0)*.x))) (x+.((p/.100.0)*.x));;

let value_od_do x y = 
    przedzial_laczny x y;;

let value_dokladna x =
    przedzial_laczny x x;;

(* in_value w x = x \in w *)
let in_value w x = 
    if is_nan w then false else
        let (a, b) = (w.lewy, w.prawy)
        in
            if w.laczny then
                if (x >= a) && (x <= b) then true else false
            else
                if (x <= a) || (x >= b) then true else false;;

let min_value w = 
    if is_nan w then nan else
        if w.laczny then w.lewy
        else neg_infinity;;

let max_value w = 
    if is_nan w then nan else
        if w.laczny then w.prawy
        else infinity;;

let sr_value w = 
    if (is_nan w) || (not w.laczny) then nan 
    else 
        ((min_value w)+.(max_value w))/.2.;;

let przedzial_odwrotny w =
    let (a, b) = (w.lewy, w.prawy)
        in
        if (in_value w 0.) then
            if w.laczny then
                if a = neg_infinity then
                    if b = 0. then
                        przedzial_laczny neg_infinity 0.
                    else
                        przedzial_rozlaczny 0. (1. /. b)
                else if b = infinity then
                    if a = 0. then
                        przedzial_laczny 0. infinity
                    else
                        przedzial_rozlaczny (1. /. a) 0.
                else if a = 0. then
                    przedzial_laczny (1. /. b) infinity
                else if b = 0. then
                    przedzial_laczny neg_infinity (1. /. a)
                else
                    przedzial_rozlaczny (1. /. a) (1. /. b)
            else
                if a = 0. then
                    przedzial_laczny neg_infinity (1. /. b)
                else if b = 0. then
                    przedzial_laczny (1. /. a) infinity
                else
                    przedzial_rozlaczny (1. /. b) (1. /. a)
    else
        let (l, p) = ((min (1. /. a) (1. /. b)), (max (1. /. a) (1. /. b)))
        in
            if l = 0. then
                przedzial_laczny 0. p
            else if p = 0. then
                przedzial_laczny l 0.
            else
                przedzial_laczny (min (1. /. a) (1. /. b)) (max (1. /. a) (1. /. b));;

let plus w1 w2 = 
    if (is_nan w1) || (is_nan w2) then nan_interval 
    else
        if w1.laczny && w2.laczny then
            przedzial_laczny (w1.lewy +. w2.lewy) (w1.prawy +. w2.prawy)
        else if (not w1.laczny) && w2.laczny then
            let lewy_koniec = (w2.prawy +. w1.lewy)
            and prawy_koniec = (w2.lewy +. w1.prawy)
            in
                if lewy_koniec >= prawy_koniec then
                    przedzial_laczny neg_infinity infinity
                else
                    przedzial_rozlaczny lewy_koniec prawy_koniec 
        else if w1.laczny && (not w2.laczny) then
            let lewy_koniec = (w1.prawy +. w2.lewy)
            and prawy_koniec = (w1.lewy +. w2.prawy)
            in
                if lewy_koniec >= prawy_koniec then
                    przedzial_laczny neg_infinity infinity
                else
                    przedzial_rozlaczny lewy_koniec prawy_koniec 
        else
            przedzial_laczny neg_infinity infinity;;
                

let minus w1 w2 =
    if (is_nan w1) || (is_nan w2) then nan_interval else
        plus w1 (przedzial_przeciwny w2);;
            
let razy w1 w2 = 
    if (is_nan w1) || (is_nan w2) then nan_interval else
        let (a1, b1, a2, b2) = (w1.lewy, w1.prawy, w2.lewy, w2.prawy)
        in
            (* mnożenie przez dokładnie 0 *)
            if (w1 = zero)||(w2 = zero) then
                value_dokladnosc 0. 0.
            (* mnożenie dwóch łącznych przedziałów *)
            else if (w1.laczny) && (w2.laczny) then
                przedzial_laczny (min4 (mp a1 a2) (mp a1 b2) (mp b1 a2) (mp b1 b2)) (max4 (mp a1 a2) (mp a1 b2) (mp b1 a2) (mp b1 b2))
            (* mnożenie dwóch rozłącznych przedziałów *)
            else if (not (w1.laczny)) && (not (w2.laczny)) then
                if (in_value w1 0.) || (in_value w2 0.) then
                    przedzial_laczny neg_infinity infinity
                else
                    przedzial_rozlaczny (max (a1 *. b2) (b1 *. a2)) (min (a1 *. a2) (b1 *. b2))   
            (* mnożenie przedziału łącznego i rozłącznego *)
            else
                let (p1, p2) = 
                    if w1.laczny then (w1, w2)
                    else (w2, w1)
                in 
                    let (a1, b1, a2, b2) = (p1.lewy, p1.prawy, p2.lewy, p2.prawy)
                    in
                        if neg_and_pos p1 then
                            przedzial_laczny neg_infinity infinity
                        else if only_neg p1 then
                            przedzial_rozlaczny (max (a1*.b2) (b1*.b2)) (min (a1*.a2) (b1*.a2))
                        else
                            przedzial_rozlaczny (max (a1*.a2) (b1*.a2)) (min (a1*.b2) (b1*.b2));;

        
let podzielic w1 w2 = 
    if (is_nan w1) || (is_nan w2) then nan_interval else
        if w2 = zero then 
            przedzial_laczny nan nan
        else
            let odw_w2 = przedzial_odwrotny w2
            in
                razy w1 odw_w2;;

