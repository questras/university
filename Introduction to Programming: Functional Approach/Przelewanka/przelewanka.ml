(* Przelewanka *)
(* Autor: Michał Wiśniewski *)
(* Code Review: Aleksandra Martyniuk gr 7 *)


(** Rozdziel tablice [arr] typu [(int * int) array] na 
    dwie tablice typu [int array] *)
let rozdziel arr = 
  let n = Array.length arr in
  let arr1 = Array.init n (fun i -> fst arr.(i)) in
  let arr2 = Array.init n (fun i -> snd arr.(i)) in
  (arr1, arr2)


(** Najwiekszy wspolny dzielnik liczb a i b *)
let rec nwd a b =
  if (a = 0) || (b = 0) then (max a b)
  else
    let (n1, n2) = (max a b, min a b) in
    if n1 mod n2 = 0 then n2
    else 
      nwd n2 (n1 mod n2)


(** Sprawdz czy w tablicy [arr1] i [arr2] sa same zera. *)
let czy_same_zera arr1 arr2 =
  let sum1 = Array.fold_left (fun acc a -> acc + a) 0 arr1 in
  let sum2 = Array.fold_left (fun acc a -> acc + a) 0 arr2 in
  (sum1 = 0) && (sum2 = 0)


(** Sprawdz czy nwd [pojemnosci] podzieli kazda z liczb w [cel],
    zakladamy, ze [pojemnosci] i [cel] nie sa samymi zerami. *)
let sprawdz_nwd pojemnosci cel = 
  let n = Array.length pojemnosci in
  let nwd_pojemnosci = Array.fold_left nwd pojemnosci.(0) pojemnosci in
  if nwd_pojemnosci = 0 then 
    false
  else
    begin 
      let flaga = ref true in
      for i = 0 to n - 1 do
        if cel.(i) mod nwd_pojemnosci <> 0 then
          flaga := false;
      done;
      !flaga
    end


(** Sprawdz czy w [cel] jest co najmniej jedna szklanka
    pusta lub pelna. *)
let czy_pelna_lub_pusta pojemnosci cel = 
  let n = Array.length pojemnosci in
  let flaga = ref false in
  begin 
    for i = 0 to n-1 do
    if (cel.(i) = pojemnosci.(i)) || (cel.(i) = 0) then
      flaga := true;
    done;
    !flaga
  end


(** Sprawdz czy warunki poczatkowe zadania sa spelnione *)
let warunki_spelnione pojemnosci cel =
  (Array.length pojemnosci = 0) || 
  (czy_same_zera pojemnosci cel) ||
  ((czy_pelna_lub_pusta pojemnosci cel) && 
  (sprawdz_nwd pojemnosci cel))


(** Nalej wody do pelna do i-tej szklanki i zwroc stan. *)
let nalej pojemnosci stan i =
  let kopia = Array.copy stan in
  kopia.(i) <- pojemnosci.(i);
  kopia


(** Wylej wode z i-tej szklanki i zwroc stan. *)
let wylej stan i =
  let kopia = Array.copy stan in
  kopia.(i) <- 0;
  kopia


(** Przelej wode z i-tej szklanki do k-tej szklanki i zwroc stan. *)
let przelej pojemnosci stan i k =
  let kopia = Array.copy stan in
  if (stan.(i) + stan.(k)) >= pojemnosci.(k) then
    begin
      kopia.(i) <- stan.(i) + stan.(k) - pojemnosci.(k);
      kopia.(k) <- pojemnosci.(k);
      kopia
    end
  else
    begin
      kopia.(i) <- 0;
      kopia.(k) <- stan.(i) + stan.(k);
      kopia
    end


(** Wyjatek podnoszony kiedy zostal znaleziony prawidlowy stan *)
exception Znalezione of int


(** Dodaj [stan] do [kolejka] jesli nie wystepuje on juz w [mapa_stanow],
    wywolaj [Znalezione] jesli dany [stan] jest docelowym stanem. *)
let dodaj_stan stan cel numer kolejka mapa_stanow = 
  if stan = cel then 
    raise (Znalezione numer)
  else
    if not (Hashtbl.mem mapa_stanow stan) then
      begin
        Hashtbl.add mapa_stanow stan numer;
        Queue.push stan kolejka;
      end


let przelewanka arr =
  let (pojemnosci, cel) = rozdziel arr in
  let n = Array.length pojemnosci in

  if warunki_spelnione pojemnosci cel then
    (* Mapa postaci: stan -> liczba krokow wykonanych do jego osiagniecia *)
    let mapa_stanow = Hashtbl.create 100 in
    (* Kolejka stanow do rozpatrzenia. *)
    let kolejka = Queue.create () in
    try
      begin
        (* Dodanie pierwszego stanu *)
        let stan = ref (Array.make n 0) in
        let numer = ref 0 in
        dodaj_stan !stan cel !numer kolejka mapa_stanow;

        (* Sprawdzanie i dodawanie nowych stanow do kolejki tak dlugo 
          jak inne stany sa w kolejce *)
        while not (Queue.is_empty kolejka) do
          let stan_z_kolejki = Queue.pop kolejka in
          for i = 0 to n-1 do
            numer := Hashtbl.find mapa_stanow stan_z_kolejki;
            
            stan := nalej pojemnosci stan_z_kolejki i;
            dodaj_stan !stan cel (!numer + 1) kolejka mapa_stanow;

            stan := wylej stan_z_kolejki i;
            dodaj_stan !stan cel (!numer + 1) kolejka mapa_stanow;

            for k = 0 to n - 1 do
              if k <> i then
                stan := przelej pojemnosci stan_z_kolejki i k;
                dodaj_stan !stan cel (!numer + 1) kolejka mapa_stanow;
            done;
          done;
        done;
        -1
      end
    with 
      Znalezione wynik -> wynik
  else 
    -1 (* Kiedy [cel] jest niemozliwy do osiagniecia *)


(* Testy *)
(* (* Autor: mdrozdowski1996
 * Source: https://github.com/mdrozdowski1996/przelewanka
 * Licence: unlicensed *)


(*Nie ma rozwiązania*)
let c = [|(10,2);(20,20);(10,0);(1000,1000)|];;
assert ( przelewanka c = -1 );;
let c = [|(3,2);(5,4);(5,2);(6,1)|];;
assert (przelewanka c = -1);;
let c = [|(40,1);(10,4);(23,2);(40,1)|];;
assert (przelewanka c = -1);;
let c = [|(12,2);(6,3);(4,4);(10,2)|];;
assert (przelewanka c = -1);;
let c = [|(14,3);(3,1)|];;
assert (przelewanka c = -1);;

(*Testy różne*)
let c = [|(3,2);(3,3);(1,0);(12,1)|];;
assert ( przelewanka c = 4 );;
let c = [|(1,1);(100,99)|];;
assert ( przelewanka c = 2 );;
let c = [|(3,3);(5,4);(5,2);(6,1)|];;
assert (przelewanka c = 6);;
let c = [|(100,3);(2,1);(1,0);(6,1)|];;
assert (przelewanka c = 7);;
let c = [|(3,3);(5,5);(5,5);(6,6)|];;
assert (przelewanka c = 4);;
let c = [|(40,20);(20,10);(10,5);(5,0)|];;
przelewanka c ;;
let c = [|(19,3);(1,1);(2,2)|];;
assert (przelewanka c = 6);;
let c = [|(14,3);(3,1);(3,0)|];;
assert (przelewanka c = 13);;
let c = [|(3,3);(4,0);(1,1);(6,6)|];;
assert (przelewanka c = 3);;
let c = [|(46,20);(23,10);(13,5);(5,0)|];;
assert (przelewanka c = 10);;
let c = [|(18,3);(3,1);(2,2)|];;
assert (przelewanka c = 4);;
let c = [|(14,3);(5,1)|];;
assert (przelewanka c = -1);;
let c = [|(14,3);(5,1);(5,0)|];;
assert (przelewanka c = 16);;

(* Przelewanie ciągle z jednego do drugiego*)
let c = [|(10000,5000);(1,0)|];;
assert (przelewanka c = 10000);;
let c = [|(50000,450);(3,1);(3,0)|];;
assert (przelewanka c = 33635);;
let c = [|(100000,25252);(2,2)|];;
assert (przelewanka c = 25253);; *)