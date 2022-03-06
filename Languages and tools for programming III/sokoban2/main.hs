{-# LANGUAGE OverloadedStrings #-}

import CodeWorld

main :: Program
main = program

type Program = IO ()

program :: Program
program = walk3

walk1 :: IO ()
walk1 = activityOf initialMazeState handleEvent drawMazeState

walk2 :: IO ()
walk2 = activityOf initialMazeState handleEvent drawMazeStateWithTurning

walk3 :: IO ()
walk3 = resettableActivityOf initialMazeState handleEvent drawMazeStateWithTurning

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

initialMazeState :: MazeState
initialMazeState =
  Ms
    { playerPos = initialPlayerCoord,
      playerDir = initialPlayerDirection,
      boardSize = Bs mazeMinX mazeMaxX mazeMinY mazeMaxY,
      mazeMap = maze
    }

-- Activity wrapper.
resettableActivityOf ::
  world ->
  (Event -> world -> world) ->
  (world -> Picture) ->
  IO ()
resettableActivityOf initState eventHandler = activityOf initState (resettingEventHandler eventHandler initState)

{-
Zdecydowałem, że po puszczeniu klawisza ESC (Key Release) nie powinno się nic dziać.
Po wciśnięciu przycisku ESC i trzymaniu go, wysyłane jest tylko jedno zdarzenie, 
więc nie ma efektu ciągle resetującej się gry, więc nie widzę powodu, aby dodawać
specjalne zachowanie na puszczeniu ESC, a możliwość grania z wciśniętym klawiszem ESC jest
moim zdaniem poprawnym zachowaniem. 
-}
resettingEventHandler :: (Event -> world -> world) -> world -> Event -> world -> world
resettingEventHandler eventHandler initState (KeyPress "Esc") currentState = initState
resettingEventHandler eventHandler _ event currentState = eventHandler event currentState

-- Types
data MazeState = Ms
  { playerPos :: Coord,
    playerDir :: Direction,
    boardSize :: BoardSize,
    mazeMap :: MazeMap
  }

type MazeMap = Coord -> Tile

data Direction = R | U | L | D

data Coord = C Integer Integer

data BoardSize = Bs
  { minX :: Integer,
    maxX :: Integer,
    minY :: Integer,
    maxY :: Integer
  }

-- Event handling
handleEvent :: Event -> MazeState -> MazeState
handleEvent (KeyPress key) ms
  | key == "Right" = ms {playerPos = movePlayer R (playerPos ms) (mazeMap ms), playerDir = R}
  | key == "Up" = ms {playerPos = movePlayer U (playerPos ms) (mazeMap ms), playerDir = U}
  | key == "Left" = ms {playerPos = movePlayer L (playerPos ms) (mazeMap ms), playerDir = L}
  | key == "Down" = ms {playerPos = movePlayer D (playerPos ms) (mazeMap ms), playerDir = D}
handleEvent _ ms = ms

-- Maze drawing.
drawMazeState :: MazeState -> Picture
drawMazeState (Ms playerPos playerDir boardSize mazeMap) =
  drawAtCoord playerPos player1 & drawMaze mazeMap boardSize

drawMazeStateWithTurning :: MazeState -> Picture
drawMazeStateWithTurning (Ms playerPos playerDir boardSize mazeMap) =
  drawAtCoord playerPos (player2 playerDir) & drawMaze mazeMap boardSize

drawMaze :: MazeMap -> BoardSize -> Picture
drawMaze mazeMap (Bs minX maxX minY maxY) = pictures [drawAtCoord (C x y) (drawTile (mazeMap (C x y))) | x <- [minX .. maxX], y <- [minY .. maxY]]

drawAtCoord :: Coord -> Picture -> Picture
drawAtCoord (C x y) = translated (fromIntegral x) (fromIntegral y)

-- Player
player2 :: Direction -> Picture
player2 direction = rotated (directionToAngle direction) player1

player1 :: Picture
player1 = colored red arrow

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

movePlayer :: Direction -> Coord -> MazeMap -> Coord
movePlayer direction coord mazeMap =
  if isSteppable adjCoord mazeMap then adjCoord else coord
  where
    adjCoord = adjacentCoord direction coord

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
