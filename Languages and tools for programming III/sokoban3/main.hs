{-# LANGUAGE OverloadedStrings #-}

import CodeWorld

main :: Program
main = program

type Program = IO ()

program :: Program
program = runActivity fullGameActivity
fullGameActivity = (withStartScreen . resettable) initialActivity

-- Current level.
initialPlayerCoord :: Coord
initialPlayerCoord = C (-3) (-3)

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

maze :: MazeMap
maze (C x y)
  | abs x > 4 || abs y > 4 = Space
  | abs x == 4 || abs y == 4 = Wall
  | x == 2 && y <= 0 = Wall
  | x == 3 && y <= 0 = Storage
  | x >= -2 && y == 0 = Box
  | otherwise = Ground

initialState :: State
initialState =
  Ms
    { playerPos = initialPlayerCoord,
      playerDir = initialPlayerDirection,
      boardSize = initialBoardSize,
      boxesCoords = initialBoxesCoords maze initialBoardSize,
      mazeMap = maze
    }
  where
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

data ScreenState state = StartScreen | Running state

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

-- Types
data State = Ms
  { playerPos :: Coord,
    playerDir :: Direction,
    boardSize :: BoardSize,
    boxesCoords :: [Coord],
    mazeMap :: MazeMap
  }

type MazeMap = Coord -> Tile

data Direction = R | U | L | D

data Coord = C Integer Integer deriving (Eq)

data BoardSize = Bs
  { minX :: Integer,
    maxX :: Integer,
    minY :: Integer,
    maxY :: Integer
  }

-- Game state
isWinning :: State -> Bool
isWinning (Ms _ _ _ boxesCoords mazeMap) =
  all (\coord -> mazeMap coord == Storage) boxesCoords

-- Event handling
handleEvent :: Event -> State -> State
handleEvent (KeyPress key) ms
  | isWinning ms = ms
  | key == "Right" = go R
  | key == "Up" = go U
  | key == "Left" = go L
  | key == "Down" = go D
  where go direction = movePlayer direction ms
handleEvent _ ms = ms

-- Maze drawing.
draw :: State -> Picture
draw (Ms playerPos playerDir boardSize boxesCoords mazeMap) =
  if isWinning (Ms playerPos playerDir boardSize boxesCoords mazeMap) then winScreen
  else
  drawAtCoord playerPos (player playerDir) & drawMaze mazeMap boardSize boxesCoords

drawMaze :: MazeMap -> BoardSize -> [Coord] -> Picture
drawMaze mazeMap (Bs minX maxX minY maxY) boxesCoords = 
  pictures [drawAtCoord (C x y) (drawTile (mazeWithUpdatedBoxes (C x y))) | x <- [minX .. maxX], y <- [minY .. maxY]]
  where
    mazeWithUpdatedBoxes = withUpdatedBoxes mazeMap boxesCoords

drawAtCoord :: Coord -> Picture -> Picture
drawAtCoord (C x y) = translated (fromIntegral x) (fromIntegral y)

-- Start screen
startScreen :: Picture
startScreen = scaled 3 3 (lettering "Sokoban!")

-- Win screen
winScreen :: Picture
winScreen = scaled 3 3 (lettering "Wygrana!")

-- Boxes
initialBoxesCoords :: MazeMap -> BoardSize -> [Coord]
initialBoxesCoords mazeMap (Bs minX maxX minY maxY) =
  [C x y | x <- [minX..maxX], y <- [minY..maxY], mazeMap (C x y) == Box]

removeBoxes :: MazeMap -> MazeMap
removeBoxes mazeMap = transformBoxToGround . mazeMap
  where transformBoxToGround tile = if tile == Box then Ground else tile

addBoxes :: [Coord] -> MazeMap -> MazeMap
addBoxes boxCoords mazeMap coord = if coord `elem` boxCoords then Box else mazeMap coord

withUpdatedBoxes :: MazeMap -> [Coord] -> MazeMap
withUpdatedBoxes mazeMap boxesCoords = addBoxes boxesCoords (removeBoxes mazeMap)

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

-- Moving
adjacentCoord :: Direction -> Coord -> Coord
adjacentCoord U (C x y) = C x (y + 1)
adjacentCoord D (C x y) = C x (y - 1)
adjacentCoord R (C x y) = C (x + 1) y
adjacentCoord L (C x y) = C (x - 1) y

movePlayer :: Direction -> State -> State
movePlayer direction ms
  | adjCoord `elem` boxesCoords ms = movePlayerAndBox adjCoord direction ms
  | isSteppable adjCoord mazeWithUpdatedBoxes = msWithUpdatedPlayerDir {playerPos = adjCoord}
  | otherwise = msWithUpdatedPlayerDir
  where
      mazeWithUpdatedBoxes = withUpdatedBoxes (mazeMap ms) (boxesCoords ms)
      msWithUpdatedPlayerDir = ms {playerDir = direction}
      adjCoord = adjacentCoord direction (playerPos ms)

movePlayerAndBox :: Coord -> Direction -> State -> State
movePlayerAndBox boxCoord direction ms =
  if isSteppable boxAdjCoord mazeWithUpdatedBoxes
  then msWithUpdatedPlayerDir {
          playerPos = boxCoord,
          boxesCoords = updatedBoxesCoords
          }
  else msWithUpdatedPlayerDir
  where
    msWithUpdatedPlayerDir = ms {playerDir = direction}
    boxAdjCoord = adjacentCoord direction boxCoord
    updatedBoxesCoords = updateBoxesCoords (boxesCoords ms) boxCoord boxAdjCoord
    mazeWithUpdatedBoxes = withUpdatedBoxes (mazeMap ms) (boxesCoords ms)

updateBoxesCoords :: [Coord] -> Coord -> Coord -> [Coord]
updateBoxesCoords boxesCoords oldCoord newCoord =
  map (\c -> if c == oldCoord then newCoord else c) boxesCoords

isSteppable :: Coord -> MazeMap -> Bool
isSteppable coord mazeMap = (mazeMap coord == Ground) || (mazeMap coord == Storage)

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
