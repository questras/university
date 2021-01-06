(* Leftist Trees *)
(* Author: Michał Wiśniewski *)
(* Code Reviewer:   *)



(* Type representing a leftist tree, *)
(* the tree is represented by Node or Leaf, *)
(* Node consists of: *)
(* - left subtree *)
(* - value in the node *)
(* - length of it's right path *)
(* - right subtree *)
(* Leaf is an empty tree *)
type 'a queue = 
    | Node of  'a queue * 'a * int * 'a queue
    | Leaf



(* -------------------- *)
(* ---- Exceptions ---- *)
(* -------------------- *)


(* Exception which is raised when you try *)
(* to access minimum element of an empty leftist tree *)
exception Empty 



(* ----------------------- *)
(* ---- Helper values ---- *)
(* ----------------------- *)


(* An empty leftist tree *)
let empty = Leaf



(* -------------------------- *)
(* ---- Helper functions ---- *)
(* -------------------------- *)


(* Returns pair of trees where tree with smaller *)
(* value in node is to the left *)
(* Leaf has no value so it goes to the right*)
let left_smaller (tree1 : 'a queue)  (tree2 : 'a queue) = 
    match(tree1, tree2) with
        | (Leaf, Leaf) -> (Leaf, Leaf)
        | (Node(_, _, _, _), Leaf) -> (tree1, Leaf)
        | (Leaf, Node(_, _, _, _)) -> (tree2, Leaf)
        | (Node(_, x, _, _), Node(_, y, _, _)) ->
            if x < y then
                (tree1, tree2)
            else
                (tree2, tree1)        



(* ------------------------ *)
(* ---- Core functions ---- *)
(* ------------------------ *)


(* Returns a leftist tree which is the result of *)
(* merging two leftist trees into one *)
 let rec join (tree1 : 'a queue)  (tree2 : 'a queue) =
    (* result is empty if both trees are empty *)
    if (tree1 = empty) && (tree2 = empty) then
        empty
    (* result is tree2 if tree1 is empty *)
    else if tree1 = empty then
        tree2
    (* result is tree1 if tree2 is empty *)
    else if tree2 = empty then
        tree1
    (* when both trees are not empty *)
    else
        (* make sure that 'tree1' has smaller value in node *)
        let (tree1, tree2) = left_smaller tree1 tree2 in
        match tree1 with
            (* error when getting leaf, what is not possible *)
            (* because above conditions prevent it *)
            | Leaf -> failwith "not possible"
            | Node(sub_left, n, rpath, sub_right) ->
                (* merge right subtree of 'tree1' with 'tree2' *)
                let merged = join sub_right tree2 in
                (* put tree with smaller right path to the right *)
                match (sub_left, merged) with
                    (* Leaf goes to the right, because it has no right path *)
                    | (Leaf, Leaf) -> Node(sub_left, n, 0, merged)
                    (* Leaf goes to the right, because it has no right path *)
                    | (Leaf, Node(_, _, _, _)) -> Node(merged, n, 0, sub_left)
                    (* Leaf goes to the right, because it has no right path *)
                    | (Node(_, _, _, _), Leaf) -> Node(sub_left, n, 0, merged)
                    (* Tree with smaller right path goes to the right *)
                    | (Node(_, _, rpath1, _), Node(_, _, rpath2, _)) ->
                        if rpath1 < rpath2 then
                            Node(merged, n, rpath1+1, sub_left)
                        else
                            Node(sub_left, n, rpath2+1, merged)


(* Returns tree with added 'element' *)
let add (element : 'a) (tree : 'a queue) =
    (* Joining 'tree' and tree with one element: 'element' *)
    join tree (Node(Leaf, element, 0, Leaf))


(* Returns the smallest element of tree and *)
(* tree without this element *)
let delete_min (tree : 'a queue) = 
    match tree with
        (* Raise exception if function is called on *)
        (* an empty tree *)
        Leaf -> raise Empty  |
        Node(l, x, _, p) ->
            (x, (join l p))
        

(* Returns true if the tree is empty, false otherwise *)
let is_empty (tree : 'a queue) = 
    if tree = empty then true else false;;


    
(* ------*)
(* Tests *)
(* ----- *)


(* let a = empty;;
let b = add 1 empty;;

assert (is_empty a = true);;
assert (try let _=delete_min a in false with Empty -> true);;
assert (is_empty b <> true);;

let b = join a b ;;
assert (is_empty b <> true);;

let (x,y) = delete_min b;;

assert (x = 1);;
assert (is_empty y = true);;
assert (try let _=delete_min y in false with Empty -> true);;

(* delete_min integer tests *)
let b = add 1 empty;;
let b = add 3 b;;
let b = add (-1) b;;
let b = add 2 b;;
let b = add 1 b;;

let (a,b) = delete_min b;;
assert (a = -1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 2);;

let (a,b) = delete_min b;;
assert (a = 3);;

assert(is_empty b = true);;

(* delete_min string tests *)
let b = add "a" empty;;
let b = add "aca" b;;
let b = add "nzbzad" b;;
let b = add "nzbza" b;;
let b = add "bxbxc" b;;

let (a,b) = delete_min b;;
assert (a = "a");;

let (a,b) = delete_min b;;
assert (a = "aca");;

let (a,b) = delete_min b;;
assert (a = "bxbxc");;

let (a,b) = delete_min b;;
assert (a = "nzbza");;

let (a,b) = delete_min b;;
assert (a = "nzbzad");;

assert(is_empty b = true);;
assert (try let _=delete_min b in false with Empty -> true);;

(* join tests *)

let b = add 1 empty;;
let b = add 3 b;;
let b = add (-1) b;;
let b = add 2 b;;
let b = add 1 b;;

let c = add 10 empty;;
let c = add (-5) c;;
let c = add 1 c;;
let c = add 4 c;;
let c = add 0 c;;

let b = join b c;;

let (a,b) = delete_min b;;
assert (a = (-5));;

let (a,b) = delete_min b;;
assert (a = (-1));;

let (a,b) = delete_min b;;
assert (a = 0);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 2);;

let (a,b) = delete_min b;;
assert (a = 3);;

let (a,b) = delete_min b;;
assert (a = 4);;

let (a,b) = delete_min b;;
assert (a = 10);;

assert (try let _=delete_min b in false with Empty -> true);;

let b = add 1 empty;;
let b = add 3 b;;
let b = add (-1) b;;
let b = add 2 b;;
let b = add 1 b;;

let c = add 10 empty;;
let c = add (-5) c;;
let c = add 1 c;;
let c = add 4 c;;
let c = add 0 c;;

let b = join c b;;

let (a,b) = delete_min b;;
assert (a = (-5));;

let (a,b) = delete_min b;;
assert (a = (-1));;

let (a,b) = delete_min b;;
assert (a = 0);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 1);;

let (a,b) = delete_min b;;
assert (a = 2);;

let (a,b) = delete_min b;;
assert (a = 3);;

let (a,b) = delete_min b;;
assert (a = 4);;

let (a,b) = delete_min b;;
assert (a = 10);;

assert (try let _=delete_min b in false with Empty -> true);; *)
