{-# LANGUAGE OverloadedStrings #-}

import CodeWorld
import Data.String

main :: Program
main = etap5

type Program = IO ()

etap5 :: Program
etap5 = runActivity fullGameActivity
fullGameActivity = (withUndo . withStartScreen . resettable) initialActivity

-- Current level.
mazesQueue :: [Maze]
mazesQueue = tail mazes

initialMaze :: Maze
initialMaze = head mazes

initialPlayerCoord :: Coord
initialPlayerCoord = mazeCoord initialMaze

initialPlayerDirection :: Direction
initialPlayerDirection = U

mazeMinX :: Integer
mazeMinX = -10

mazeMaxX :: Integer
mazeMaxX = 10

mazeMinY :: Integer
mazeMinY = -10

mazeMaxY :: Integer
mazeMaxY = 10

initialState :: State
initialState =
  Ms
    { playerPos = initialPlayerCoord,
      playerDir = initialPlayerDirection,
      boardSize = initialBoardSize,
      boxesCoords = getBoxesCoords firstMazeMap initialPlayerCoord,
      numberOfMoves = 0,
      mazeMap = firstMazeMap,
      mazeQueue = mazesQueue
    }
  where
    firstMazeMap = rootMazeMap initialMaze
    initialBoardSize = Bs mazeMinX mazeMaxX mazeMinY mazeMaxY

initialActivity :: Activity State
initialActivity = Activity {
  activityState = initialState,
  activityEventHandler = handleEvent,
  activityDrawer = draw
}

-- Activity
data Activity state = Activity {
  activityState :: state,
  activityEventHandler :: Event -> state -> state,
  activityDrawer :: state -> Picture
}

data ScreenState state = StartScreen | Running state | EndScreen state deriving Eq
data WithUndo a = WithUndo a [a]

runActivity :: Activity state -> IO ()
runActivity (Activity state eventHandler drawer) = activityOf state eventHandler drawer

resettable :: Activity state -> Activity state
resettable (Activity initialState eventHandler drawer)
  = Activity initialState eventHandler' drawer
  where
    eventHandler' (KeyPress "Esc") _ = initialState
    eventHandler' event state = eventHandler event state

withStartScreen :: Activity state -> Activity (ScreenState state)
withStartScreen (Activity initialState eventHandler drawer)
  = Activity initialState' eventHandler' drawer'
  where
    initialState' = StartScreen
    eventHandler' (KeyPress " ") StartScreen = Running initialState
    eventHandler' _ StartScreen = StartScreen
    eventHandler' event (Running state) = Running (eventHandler event state)
    drawer' StartScreen = startScreen
    drawer' (Running state) = drawer state

withUndo :: Eq a => Activity a -> Activity (WithUndo a)
withUndo (Activity initialState eventHandler drawer) = Activity initialState' eventHandler' drawer'
  where
    initialState' = WithUndo initialState []
    eventHandler' (KeyPress "U") (WithUndo s stack) =
      case stack of 
        s':stack' -> WithUndo s' stack'
        []        -> WithUndo s []
    eventHandler' e (WithUndo s stack)
      | s' == s = WithUndo s stack
      | otherwise = WithUndo (eventHandler e s) (s:stack)
      where s' = eventHandler e s
    drawer' (WithUndo s _) = drawer s

-- Types
data State = Ms
  { playerPos :: Coord,
    playerDir :: Direction,
    boardSize :: BoardSize,
    boxesCoords :: [Coord],
    numberOfMoves :: Integer,
    mazeMap :: MazeMap,
    mazeQueue :: [Maze]
  }

instance Eq State where
  (Ms pp1 pd1 bs1 bc1 nm1 _ _) == (Ms pp2 pd2 bs2 bc2 nm2 _ _) = 
    andList [pp1 == pp2, pd1 == pd2, bs1 == bs2, bc1 == bc2, nm1 == nm2]

type MazeMap = Coord -> Tile

data Maze = Maze Coord MazeMap

mazeCoord :: Maze -> Coord
mazeCoord (Maze c _) = c

rootMazeMap :: Maze -> MazeMap
rootMazeMap (Maze _ mm) = mm

data Direction = R | U | L | D deriving Eq

data Coord = C Integer Integer deriving (Eq)

data BoardSize = Bs
  { minX :: Integer,
    maxX :: Integer,
    minY :: Integer,
    maxY :: Integer
  } deriving Eq

-- Game state
isWinning :: State -> Bool
isWinning ms =
  all (\coord -> (mazeMap ms) coord == Storage) allReachableBoxesCoords
  where
    allReachableBoxesCoords = getBoxesCoords mazeMapWithUpdatedBoxes (playerPos ms)
    mazeMapWithUpdatedBoxes = withUpdatedBoxes (mazeMap ms) (boxesCoords ms)

isClosed :: Maze -> Bool
isClosed (Maze initial mazeMap) =
  elemList (mazeMap initial) [Storage, Ground] &&
  isGraphClosed initial steppableNeighboursFn (\c -> mazeMap c /= Space) 
  where
  steppableNeighboursFn (C x y) =
    if mazeMap (C x y) == Space then []
    else
      filterList isSteppableAndSpace [C (x+1) y, C (x-1) y, C x (y+1), C x (y-1)]
      where
        isSteppableAndSpace coord = elemList (mazeMap coord) [Storage, Ground, Space]

isSane :: Maze -> Bool
isSane (Maze initial mazeMap) = storagesCount >= boxesCount
  where
    boxesCount = predicateCount (\c -> mazeMap c == Box)
    storagesCount = predicateCount (\c -> mazeMap c == Storage)
    predicateCount = graphTraverseWithCount initial steppableNeighboursFn
    steppableNeighboursFn (C x y) =
      if mazeMap (C x y) == Box then []
      else
        filterList isSteppableAndBox [C (x+1) y, C (x-1) y, C x (y+1), C x (y-1)]
        where
          isSteppableAndBox coord = elemList (mazeMap coord) [Storage, Ground, Box]

-- Event handling
handleEvent :: Event -> State -> State
handleEvent (KeyPress key) ms
  | key == "N" = if null (mazeQueue ms) then ms else switchToNextMaze ms
  | winning = ms
  | key == "Right" = go R
  | key == "Up" = go U
  | key == "Left" = go L
  | key == "Down" = go D
  where
    go direction = movePlayer direction ms
    winning = isWinning ms
handleEvent _ ms = ms

switchToNextMaze :: State -> State
switchToNextMaze ms =
  Ms initialCoord U (boardSize ms) initialBoxes 0 newMazeMap newQueue
  where
    newMaze = head (mazeQueue ms)
    newQueue = tail (mazeQueue ms)
    initialBoxes = getBoxesCoords newMazeMap initialCoord
    initialCoord = mazeCoord newMaze
    newMazeMap = rootMazeMap newMaze

-- Maze drawing.
draw :: State -> Picture
draw ms =
  if isWinning ms then mazeEndScreen (numberOfMoves ms)
  else
  drawAtCoord (playerPos ms) (player (playerDir ms)) & drawMaze (mazeMap ms) (boardSize ms) (boxesCoords ms)

drawMaze :: MazeMap -> BoardSize -> [Coord] -> Picture
drawMaze mazeMap (Bs minX maxX minY maxY) boxesCoords =
  pictures [drawAtCoord (C x y) (drawTile (mazeMapWithUpdatedBoxes (C x y))) | x <- [minX .. maxX], y <- [minY .. maxY]]
  where
    mazeMapWithUpdatedBoxes = withUpdatedBoxes mazeMap boxesCoords

drawAtCoord :: Coord -> Picture -> Picture
drawAtCoord (C x y) = translated (fromIntegral x) (fromIntegral y)

-- Boxes
getBoxesCoords :: MazeMap -> Coord -> [Coord]
getBoxesCoords mazeMap initialCoord =
  graphTraverseGeneric initialCoord steppableNeighboursFn (\c -> mazeMap c == Box) [] updateFn
  where
    updateFn boxCoord currentBoxesCoords = boxCoord:currentBoxesCoords
    steppableNeighboursFn (C x y) =
        if mazeMap (C x y) == Box then []
        else
          filterList isSteppableAndBox [C (x+1) y, C (x-1) y, C x (y+1), C x (y-1)]
          where
            isSteppableAndBox coord = elemList (mazeMap coord) [Storage, Ground, Box]

removeBoxes :: MazeMap -> MazeMap
removeBoxes mazeMap = transformBoxToGround . mazeMap
  where transformBoxToGround tile = if tile == Box then Ground else tile

addBoxes :: [Coord] -> MazeMap -> MazeMap
addBoxes boxCoords mazeMap coord = if coord `elem` boxCoords then Box else mazeMap coord

withUpdatedBoxes :: MazeMap -> [Coord] -> MazeMap
withUpdatedBoxes mazeMap boxesCoords = addBoxes boxesCoords (removeBoxes mazeMap)

-- Moving
adjacentCoord :: Direction -> Coord -> Coord
adjacentCoord U (C x y) = C x (y + 1)
adjacentCoord D (C x y) = C x (y - 1)
adjacentCoord R (C x y) = C (x + 1) y
adjacentCoord L (C x y) = C (x - 1) y

movePlayer :: Direction -> State -> State
movePlayer direction ms
  | adjCoord `elem` boxesCoords ms = movePlayerAndBox adjCoord direction ms
  | isSteppable adjCoord mazeMapWithUpdatedBoxes = msWithUpdatedPlayerDir {playerPos = adjCoord, numberOfMoves = numberOfMoves ms + 1}
  | otherwise = msWithUpdatedPlayerDir
  where
      mazeMapWithUpdatedBoxes = withUpdatedBoxes (mazeMap ms) (boxesCoords ms)
      msWithUpdatedPlayerDir = ms {playerDir = direction}
      adjCoord = adjacentCoord direction (playerPos ms)

movePlayerAndBox :: Coord -> Direction -> State -> State
movePlayerAndBox boxCoord direction ms =
  if isSteppable boxAdjCoord mazeMapWithUpdatedBoxes
  then msWithUpdatedPlayerDir {
          playerPos = boxCoord,
          numberOfMoves = numberOfMoves ms + 1,
          boxesCoords = updatedBoxesCoords
          }
  else msWithUpdatedPlayerDir
  where
    msWithUpdatedPlayerDir = ms {playerDir = direction}
    boxAdjCoord = adjacentCoord direction boxCoord
    updatedBoxesCoords = updateBoxesCoords (boxesCoords ms) boxCoord boxAdjCoord
    mazeMapWithUpdatedBoxes = withUpdatedBoxes (mazeMap ms) (boxesCoords ms)

updateBoxesCoords :: [Coord] -> Coord -> Coord -> [Coord]
updateBoxesCoords boxesCoords oldCoord newCoord =
  map (\c -> if c == oldCoord then newCoord else c) boxesCoords

isSteppable :: Coord -> MazeMap -> Bool
isSteppable coord mazeMap = (mazeMap coord == Ground) || (mazeMap coord == Storage)

-- Game screens
startScreen :: Picture
startScreen = etap4

winScreen :: Picture
winScreen = scaled 3 3 (lettering "Wygrana!")

mazeEndScreen :: Integer -> Picture
mazeEndScreen moves = toTextPicture ("Poziom ukończony, liczba ruchów: " ++ show moves)

toTextPicture :: String -> Picture
toTextPicture = lettering . fromString

-- Player
player :: Direction -> Picture
player direction = rotated (directionToAngle direction) (colored red arrow)

directionToAngle :: Direction -> Double
directionToAngle R = 3 * pi / 2
directionToAngle U = 0
directionToAngle L = pi / 2
directionToAngle D = pi

arrow :: Picture
arrow = polyline [((-0.3), 0.3), (0, 0.4), (0, (-0.4)), (0, 0.4), (0.3, 0.3)]

-- Tiles
data Tile = Wall | Ground | Storage | Box | Space deriving (Eq)

drawTile :: Tile -> Picture
drawTile Wall = wall
drawTile Ground = ground
drawTile Storage = storage
drawTile Box = box
drawTile Space = space

wall :: Picture
wall = colored black emptyTile

ground :: Picture
ground = colored gray emptyTile

storage :: Picture
storage = colored green emptyTile

box :: Picture
box = colored brown emptyTile

space :: Picture
space = colored white emptyTile

emptyTile :: Picture
emptyTile = solidRectangle 1 1

-- Levels
mazes :: [Maze]
mazes = [mazeG1, mazeG2, mazeG3, mazeG4]

badMazes :: [Maze]
badMazes = [mazeB1, mazeB2, mazeB3, mazeB4]

goodMaze1 :: Coord -> Tile
goodMaze1 (C x y)
  | abs x > 4  || abs y > 4  = Space
  | abs x == 4 || abs y == 4 = Wall
  | x ==  2 && y <= 0        = Wall
  | x ==  3 && y <= 0        = Storage
  | x >= -2 && y == 0        = Box
  | otherwise                = Ground

spawnPointG1:: Coord
spawnPointG1 = C 0 2

goodMaze2 :: Coord -> Tile
goodMaze2 (C x y)
  | abs x > 4 || abs y > 2          = Space
  | abs x == 4 || abs y == 2        = Wall
  | x == 3 && y == -1               = Wall
  | x == -3 || (x == -2 && y == 1)  = Storage
  | x == 0 && y /= -1               = Box
  | x == 1 && y /= 1                = Box
  | otherwise                       = Ground

spawnPointG2 :: Coord
spawnPointG2 = C 3 1

goodMaze3 :: Coord -> Tile
goodMaze3 (C x y)
  | abs x > 7 || abs y > 7        = Space
  | abs x == 7 || abs y == 7      = Wall
  | x == 0 && y == 0              = Storage
  | x == 0 && y == -1             = Box
  | x == -2 && y == 0             = Storage
  | x == -4 && y == 5             = Box
  | y == 6 && -6 <= x && x <= -4  = Ground
  | y == 6 && 4 <= x && x <= 5    = Ground
  | y == 5 && -6 <= x && x <= 5   = Ground
  | x == 5 && -5 <= y && y <= 6   = Ground
  | x == 6 && -5 <= y && y <= -4  = Ground
  | y == -5 && -5 <= x && x <= 6  = Ground
  | y == -6 && -5 <= x && x <= -4 = Ground
  | x == -5 && -6 <= y && y <= 2  = Ground
  | x == -6 && 1 <= y && y <= 2   = Ground
  | y == 2 && -6 <= x && x <= 2   = Ground
  | y == 3 && 1 <= x && x <= 2    = Ground
  | x == 2 && -2 <= y && y <= 3   = Ground
  | x == 3 && -2 <= y && y <= -1  = Ground
  | y == -2 && -2 <= x && x <= 3  = Ground
  | y == -3 && -2 <= x && x <= -1 = Ground
  | x == -2 && y == -1            = Ground
  | otherwise                     = Wall

spawnPointG3 :: Coord
spawnPointG3 = C (-2) (-1)

goodMaze4 :: Coord -> Tile
goodMaze4 (C x y)
  | x == 1 && y == 1 = Ground
  | x == 1 && y == 2 = Box
  | x == 1 && y == 3 = Storage
  | x == 1 && (y == 0 || y == 4) = Wall
  | (x == 0 || x == 2) && y >= 0 && y < 5 = Wall
  | otherwise = Space

spawnPointG4 :: Coord
spawnPointG4 = C 1 1

badMaze1 :: Coord -> Tile
badMaze1 (C x y)
  | abs x > 2 || abs y > 1   = Space
  | abs x == 2 || abs y == 1 = Wall
  | x == -1                  = Storage
  | x == 1                   = Box
  | otherwise                = Ground

spawnPointB1:: Coord
spawnPointB1 = C 0 0

badMaze2 :: Coord -> Tile
badMaze2 (C x y)
  | abs x > 4 || y > 2 || y < -3              = Space
  | abs x == 4 || y == 2 || y == -3 || x == 0 = Wall
  | x == -3 && y == 1                         = Storage
  | x == 1 && y == 0                          = Box
  | otherwise                                 = Ground

spawnPointB2:: Coord
spawnPointB2 = C 3 (-2)

badMaze3 :: Coord -> Tile
badMaze3 (C x y)
  | x > 4 || x < -5 || y > 3 || y < -2     = Space
  | x == 4 && y == 0                       = Space
  | x == 4 || x == -5 || y == 3 || y == -2 = Wall
  | x == 3 && y == 0                       = Storage
  | x == -3 && y == 0                      = Box
  | y == 0                                 = Ground
  | y == 1 && x < -2                       = Ground
  | otherwise                              = Wall

spawnPointB3 :: Coord
spawnPointB3 = C 0 0

badMaze4 :: Coord -> Tile
badMaze4 (C x y)
  | x == 0 && y == 0 = Storage
  | x == 1 && y == 0 = Box
  | otherwise = Wall

spawnPointB4 :: Coord
spawnPointB4 = C 0 0

mazeG1:: Maze
mazeG1 = Maze spawnPointG1 goodMaze1
mazeG2:: Maze
mazeG2 = Maze spawnPointG2 goodMaze2
mazeG3 :: Maze
mazeG3 = Maze spawnPointG3 goodMaze3
mazeG4 :: Maze
mazeG4 = Maze spawnPointG4 goodMaze4
mazeB1:: Maze
mazeB1 = Maze spawnPointB1 badMaze1
mazeB2:: Maze
mazeB2 = Maze spawnPointB2 badMaze2
mazeB3 :: Maze
mazeB3 = Maze spawnPointB3 badMaze3
mazeB4 :: Maze
mazeB4 = Maze spawnPointB4 badMaze4

pictureOfBools :: [Bool] -> Picture
pictureOfBools xs = translated (-fromIntegral k / 2) (fromIntegral k) (go 0 xs)
  where n = length xs
        k = findK 0 -- k is the integer square of n
        findK i | i * i >= n = i
                | otherwise  = findK (i+1)
        go _ [] = blank
        go i (b:bs) =
          translated (fromIntegral (i `mod` k))
                     (-fromIntegral (i `div` k))
                     (pictureOfBool b)
          & go (i+1) bs

        pictureOfBool True =  colored green (solidCircle 0.4)
        pictureOfBool False = colored red   (solidCircle 0.4)

etap4 :: Picture
etap4 = pictureOfBools (mapList (\l -> isClosed l && isSane l) (mazes++badMazes))

-- Graph operations
isGraphClosed :: Eq a => a -> (a -> [a]) -> (a -> Bool) -> Bool
isGraphClosed initial neighbours isOk = graphTraverseWithCount initial neighbours (not . isOk) == 0

reachable :: Eq a => a -> a -> (a -> [a]) -> Bool
reachable v initial neighbours = graphTraverseWithCount initial neighbours (== v) > 0

allReachable :: Eq a => [a] -> a -> (a -> [a]) -> Bool
allReachable vs initial neighbours = allList reachable' vs
  where
    reachable' v = reachable v initial neighbours

graphTraverseWithCount :: Eq a => a -> (a -> [a]) -> (a -> Bool) -> Integer
graphTraverseWithCount initial neighboursFn predicate =
  graphTraverseGeneric initial neighboursFn predicate 0 updateFn
  where
    updateFn _ v = v + 1

graphTraverseGeneric :: Eq a => a -> (a -> [a]) -> (a -> Bool) -> b -> (a -> b -> b) -> b
graphTraverseGeneric initial neighboursFn predicate initialValue updateFn = graphTraverseGeneric' [initial] [] initialValue
  where
  graphTraverseGeneric' queue visited currentValue
    | null queue = currentValue
    | otherwise = graphTraverseGeneric' updatedQueue newVisited updatedValue
    where
      updatedQueue = appendList (tail queue) newNeighbours
      newNeighbours = filterList (\n -> not (elemList n (appendList newVisited queue))) (neighboursFn current)
      newVisited = current:visited
      updatedValue = if predicate current then updateFn current currentValue else currentValue
      current = head queue

-- Utils
elemList :: Eq a => a -> [a] -> Bool
elemList elem = foldList fn False
  where fn currElem isElem = isElem || (currElem == elem)

appendList :: [a] -> [a] -> [a]
appendList l1 l2 = foldList fn l2 l1
    where fn elem l2 = elem:l2

listLength :: [a] -> Integer
listLength = foldList fn 0
  where fn _ s = s + 1

filterList :: (a -> Bool) -> [a] -> [a]
filterList predicate = foldList fn []
  where fn e l = if predicate e then e:l else l

nth :: [a] -> Integer -> a
nth (h:t) n = if n == 0 then h else nth t (n-1)

mapList :: (a -> b) -> [a] -> [b]
mapList mapping = foldList fn []
  where
      fn elem l2 = (mapping elem):l2

andList :: [Bool] -> Bool
andList = allList id

allList :: (a-> Bool) -> [a] -> Bool
allList predicate = foldList fn True
  where
    fn a b = predicate a && b

foldList :: (a -> b -> b) -> b -> [a] -> b
foldList _ current [] = current
foldList fn current (h:t) = fn h (foldList fn current t)
