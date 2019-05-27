module Main where

-- needed for printing the number of steps
import Debug.Trace
-- needed for shuffle algorithm
import qualified Data.Map as Map
-- needed for StdGen / random stuff
import System.Random

-- needed for gloss functionality
import qualified Graphics.Gloss as GLO
import qualified Graphics.Gloss.Interface.Pure.Simulate as Sim

-- import our own module
import Schelling

-- data-structure defining the World
data World = World 
  { cells :: ![Cell] -- | The cells of the world
  , rng   :: !StdGen -- | The random number generator
  , steps :: !Int    -- | The number steps the world has advanced
  }

-- Size in pixels of gloss rendering window 
winSize :: (Int, Int)
winSize = (500, 500)

--Tthe ratio of similar neighbourhood the agents need to be happy
satisfactionRatio :: Double
satisfactionRatio = 0.3

-- The size of the world in terms of cells in x and y dimension
worldSize :: (Int, Int)
worldSize = (20, 20)

-- The number of agents placed in the world. Must be strictly less than
-- uncurry (*) worldSize!
agentCount :: Int
agentCount = 250

-- The random-number seed, change to start with a different random world
-- and see different dynamics
rngSeed :: Int
rngSeed = 42

-- Steps per second computed 
stepsPerSec :: Int
stepsPerSec = 3

-- The main function of the program
main :: IO ()
main = GLO.simulate -- use gloss' simulate function as rendering loop
    window          -- the window configuration
    GLO.white       -- background color
    stepsPerSec     -- Number of simulation steps to take for each second of real time.
    initWorld       -- The initial world.
    worldToPicture  -- function to convert the world a picture.
    stepWorld       -- function to step the world one iteration.         

-- defines the window properties (title, size, position)
window :: GLO.Display
window = GLO.InWindow "Schelling Segregation" winSize (0, 0)

-- initialises a random world based on the rngSeed
initWorld :: World
initWorld = World allCells g' 0
  where
    (wx, wy) = worldSize
    cs       = [(x,y) | y <- [0..wy - 1], x <- [0..wx - 1]]

    -- create random-generator
    g = mkStdGen rngSeed
    -- shuffle the coordinates because we want to place agents randomly
    (cs', g') = fisherYatesShuffle g cs

    -- take (random) coordinates for the agents
    rco = take agentCount cs'
    -- generate random color for each agent
    rcs = zip rco (map Just $ randomAgentTypes g')

    -- take (random) coordinates for the emtpy cells and generate them
    eco = drop agentCount cs'
    ecs = zip eco (repeat Nothing)

    allCells = ecs ++ rcs

-- generates an infinite list (stream) of AgentType (colors) with equal 
-- distribution between Red, Green and Blue (Empty is omitted)
randomAgentTypes :: RandomGen g 
                 => g          -- ^ The random-number generator
                 -> [AgentType] -- ^ Result is an infinite list (stream) of AgentType
randomAgentTypes g 
    | r == 1    = Red   : randomAgentTypes g'
    | r == 2    = Green : randomAgentTypes g'
    | otherwise = Blue  : randomAgentTypes g'
  where
    -- draw a random number between 1 and 3 (inclusive)
    (r, g') = randomR ((1, 3) :: (Int, Int)) g

-- Compute the next step of the world. This function checks for invariants
-- which checks that the Schelling module keeps the number of 
-- empty/red/green/blue cells constant
stepWorld :: Sim.ViewPort -- ^ The viewport
          -> Float    -- ^ The time since the last update in seconds 
          -> World    -- ^ The world of the simulation
          -> World    -- ^ Result is the world of the simulation 
stepWorld _ _ w
    | checkInvariant cs cs' = w
    | otherwise = trace ("Steps = " ++ show s) w { cells = cs', rng = g', steps = s }
  where
    cs = cells w
    g  = rng w
    s  = steps w + 1
    -- calling step from Schelling Module
    (cs', g') = step cs g satisfactionRatio

-- Checks for invariants of two cell lists: must have same number of Empty,
-- Red, Green and Blue cells in both, otherwise will throw an error exeption.
checkInvariant :: [Cell] -- ^ The first list of cells
               -> [Cell] -- ^ The second list of cells
               -> Bool   -- ^ Result is True in case of invariants are ok but will throw an error in case not
checkInvariant cs cs' 
    | invalid = error ("Invalid state detected: \n" ++
                         "\n  Empty cells before / after step = " ++ show ec ++ " / " ++ show ec' ++
                         "\n  Red cells before / after step = " ++ show rc ++ " / " ++ show rc' ++
                         "\n  Green cells before / after step = " ++ show gc ++ " / " ++ show gc' ++
                         "\n  Blue cells before / after step = " ++ show bc ++ " / " ++ show bc')
    | otherwise = False
  where
    ec = length $ filter isEmpty cs
    rc = length $ filter isRed cs
    gc = length $ filter isGreen cs
    bc = length $ filter isBlue cs

    ec' = length $ filter isEmpty cs'
    rc' = length $ filter isRed cs'
    gc' = length $ filter isGreen cs'
    bc' = length $ filter isBlue cs'

    invalid = ec /= ec' ||
              rc /= rc' ||
              gc /= gc' ||
              bc /= bc' 

    isEmpty (_, Nothing) = True
    isEmpty _ = False

    isRed (_, Just Red) = True
    isRed _ = False

    isGreen (_, Just Green) = True
    isGreen _ = False

    isBlue (_, Just Blue) = True
    isBlue _ = False

-- computes a gloss picture description of the world 
worldToPicture :: World        -- ^ The world of the simulation
               -> GLO.Picture  -- ^ Result is a gloss picture description of the world
worldToPicture w = GLO.Pictures cps
  where
    cps      = map renderCell (cells w)
    (dx, dy) = worldSize
    (wx, wy) = winSize
    cw       = (fromIntegral wx / fromIntegral dx) :: Double
    ch       = (fromIntegral wy / fromIntegral dy) :: Double

    -- renders a cell, empty cells are rendered as hollow circles, others are
    -- rendered as filled circles
    renderCell :: Cell -> GLO.Picture
    renderCell (coord, ct) 
        | isEmpty ct = GLO.color color $ GLO.translate x y $ GLO.Circle (realToFrac cw / 2)
        | otherwise  = GLO.color color $ GLO.translate x y $ GLO.ThickCircle 0 (realToFrac cw)
      where
        (x, y) = transformToWindow cw ch coord
        color  = case ct of
                  Just Red   -> GLO.makeColor 0.7 0.0 0.0 1.0
                  Just Green -> GLO.makeColor 0.0 0.7 0.0 1.0
                  Just Blue  -> GLO.makeColor 0.0 0.0 0.7 1.0
                  Nothing    -> GLO.makeColor 0.0 0.0 0.0 1.0

        isEmpty Nothing = True
        isEmpty _       = False

    -- transforms a window-coord into gloss picture coordinate system
    transformToWindow :: Double -> Double -> Coord -> (Float, Float)
    transformToWindow rw rh (x, y) = (x', y')
      where
        halfXSize = fromRational (toRational wx / 2.0)
        halfYSize = fromRational (toRational wy / 2.0)

        x' = fromRational (toRational (fromIntegral x * rw)) - halfXSize
        y' = -(fromRational (toRational (fromIntegral y * rh)) - halfYSize)

-- Taken from https://wiki.haskell.org/Random_shuffle
-- | Randomly shuffle a list without the IO Monad
--   /O(N log N)/
fisherYatesShuffle :: RandomGen g => g -> [a] -> ([a], g)
fisherYatesShuffle gen0 [] = ([], gen0)
fisherYatesShuffle gen0 l = toElems $ foldl fisherYatesStep (initial (head l) gen0) (numerate (tail l))
  where
    toElems (x, y) = (Map.elems x, y)
    numerate = zip [1..]
    initial x gen' = (Map.singleton 0 x, gen')

    fisherYatesStep :: RandomGen g => (Map.Map Int a, g) -> (Int, a) -> (Map.Map Int a, g)
    fisherYatesStep (m, gen) (i, x) = ((Map.insert j x . Map.insert i (m Map.! j)) m, gen')
      where
        (j, gen') = randomR (0, i) gen