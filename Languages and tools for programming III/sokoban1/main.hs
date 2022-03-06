import CodeWorld

main :: Program
main = program

type Program = IO ()

program :: Program
program = drawingOf pictureOfMaze

pictureOfMaze :: Picture
pictureOfMaze = pictures [translated (fromIntegral x) (fromIntegral y) (drawTile (maze x y)) | x <- [-10 .. 10], y <- [-10 .. 10]]

maze :: Integer -> Integer -> Integer
maze x y
  | abs x > 4 || abs y > 4 = 0
  | abs x == 4 || abs y == 4 = 1
  | x == 2 && y <= 0 = 1
  | x == 3 && y <= 0 = 3
  | x >= -2 && y == 0 = 4
  | otherwise = 2

drawTile :: Integer -> Picture
drawTile n
  | n == 1 = wall
  | n == 2 = ground
  | n == 3 = storage
  | n == 4 = box
  | otherwise = space

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
