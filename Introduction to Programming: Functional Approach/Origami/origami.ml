
(* Autor: Michał Wiśniewski *)
(* Recenzent: Tomek Nitsch grupa 7 *)


(** Punkt na płaszczyźnie *)
type point = float * float

(** Poskładana kartka: ile razy kartkę przebije szpilka wbita w danym punkcie *)
type kartka = point -> int

(** epsilon do używania kiedy pojawiają się niedokładne wartości *)
let epsilon = epsilon_float *. 1000000.


(** odległość dwóch punktów (ax, ay) i (bx, by) od siebie *)
let odleglosc (ax, ay) (bx, by) = 
    sqrt (((ax -. bx) *. (ax -. bx)) +. ((ay -. by) *. (ay -. by)))


(** [prostokat p1 p2] zwraca kartkę, reprezentującą domknięty prostokąt
    o bokach równoległych do osi układu współrzędnych i lewym dolnym rogu [p1]
    a prawym górnym [p2]. Punkt [p1] musi więc być nieostro na lewo i w dół
    od punktu [p2]. Gdy w kartkę tę wbije się szpilkę wewnątrz
    (lub na krawędziach) prostokąta, kartka zostanie przebita 1 raz,
    w pozostałych przypadkach 0 razy *)
let prostokat (ax, ay) (bx, by) = 
    function (px, py) -> 
        if (px >= ax) && (py >= ay) && (px <= bx) && (py <= by) then 1 else 0


(** [kolko p r] zwraca kartkę, reprezentującą kółko domknięte o środku
    w punkcie [p] i promieniu [r] *)
let kolko (srodek: point) (r: float) = 
    function p -> 
        if (odleglosc srodek p) <= r then 1 else 0


(** [symetria a b p] zwraca punkt symetryczny do punktu p 
    względem prostej przechodzącej przez punkty a i b *)
let symetria (ax, ay) (bx, by) (px, py) =
    if ax = bx then             (* pionowa linia *)
        ((2.0 *. bx -. px), py)
    else if ay = by then        (* pozioma linia *)  
        (px, (2.0 *. by -. py))
    else
        let a1 = (by -. ay) /. (bx -. ax) in
        let b1 = by -. (a1 *. bx) in
        let a2 = (~-.1.0) /. a1 in
        let b2 = py -. (a2 *. px) in
        let x = (b2 -. b1) /. (a1 -. a2) in
        let y = (a2 *. x) +. b2 in
        ((2.0 *. x -. px), (2.0 *. y -. py))


(** [pozycja a b p] to funkcja pomocnicza do funkcji [zloz p1 p2 k]
    Wynik funkcji jest dodatni, jeśli p leży na odbiciu kartki,
    jest równy 0 jeśli p leży na zgięciu kartki,
    jest ujemny jeśli p leży po przeciwnej stronie od strony odbicia kartki. *)  
let pozycja (ax, ay) (bx, by) (px, py) = 
    ((bx -. ax) *. (py -. ay) -. (by -. ay) *. (px -. ax))


(** [zloz p1 p2 k] składa kartkę [k] wzdłuż prostej przechodzącej przez
    punkty [p1] i [p2] (muszą to być różne punkty). Papier jest składany
    w ten sposób, że z prawej strony prostej (patrząc w kierunku od [p1] do [p2])
    jest przekładany na lewą. Wynikiem funkcji jest złożona kartka. Jej
    przebicie po prawej stronie prostej powinno więc zwrócić 0.
    Przebicie dokładnie na prostej powinno zwrócić tyle samo,
    co przebicie kartki przed złożeniem. Po stronie lewej -
    tyle co przed złożeniem plus przebicie rozłożonej kartki w punkcie,
    który nałożył się na punkt przebicia. *)
let zloz (a: point) (b: point) (k: kartka) =
    function p ->
        let poz = pozycja a b p in
        if (abs_float poz) <= epsilon then
            k p
        else if poz < 0.0 then
            0
        else
            (k p) + (k (symetria a b p))
            

(** [skladaj [(p1_1,p2_1);...;(p1_n,p2_n)] k = zloz p1_n p2_n (zloz ... (zloz p1_1 p2_1 k)...)]
    czyli wynikiem jest złożenie kartki [k] kolejno wzdłuż wszystkich prostych
    z listy *)
let skladaj lista k =
    let zloz2 acc (a, b) = 
        zloz a b acc
    in
        List.fold_left zloz2 k lista



(* Testy *)
(* let centr = (0., 0.);;

let a = prostokat centr (10., 10.);;

assert(a centr = 1);;
assert(a (5., 5.) = 1);;
assert(a (10., 10.) = 1);;
assert(a (10., 0.) = 1);;
assert(a (0., 10.) = 1);;
assert(a (10.1, 0.) = 0);;
assert(a (0., 10.1) = 0);;
assert(a (10.1, 10.1) = 0);;

let a = zloz (5., 0.) (5., 377.) a;;

assert(a centr = 2);;
assert(a (-377., 0.) = 0);;
assert(a (5., 2.5) = 1);;
assert(a (2.5, 3.5) = 2);;
assert(a (5., 5.) = 1);;
assert(a (5.1, 5.) = 0);;
assert(a (5.1, 5.1) = 0);;

let a = zloz (5., 0.) (5., 1.) a;;

assert(a centr = 2);;
assert(a (-377., 0.) = 0);;
assert(a (5., 2.5) = 1);;
assert(a (2.5, 3.5) = 2);;
assert(a (5., 5.) = 1);;
assert(a (5.1, 5.) = 0);;
assert(a (5.1, 5.1) = 0);;

let b = zloz (-7., -7.) (300., 300.) a;;

assert(b centr = 2);;
assert(b (0., 5.) = 3);;
assert(b (2.5, 2.5) = 2);;
assert(b (1., 2.) = 4);;
assert(b (2.5, 5.) = 3);;
assert(b (2.5, 6.) = 2);;
assert(b (2.5, 2.) = 0);;
assert(b (5., 5.) = 1);;
assert(b (5., 0.) = 0);;
assert(b (4., 2.) = 0);;
assert(b (7., 9.) = 0);;
assert(b (7., 2.) = 0);;
assert(b (5., 2.5) = 0);;
assert(b (10., 0.) = 0);;
assert(b (10., 10.) = 0);;
assert(b (10., 2.5) = 0);; *)


