-- STUDENTID USERNAME FULLNAME
module Schelling
  ( Coord
  , AgentType (..)
  , Cell

  , step
  ) where

import System.Random

-- type-definition of a coordinate in our world
type Coord = (Int, Int)

-- data definition for the agent types
data AgentType 
  = Red       -- ^ Red agent
  | Green     -- ^ Green agent
  | Blue      -- ^ Blue agent
  deriving Eq -- Needed to compare for equality, otherwise would need to implement by ourself

-- Type-definition of a Cell: it is just a coordinate and an optional AgentType.
-- Note: the agent type is optional and is Nothing in case the cell is empty.
type Cell = (Coord, Maybe AgentType)

-- Computes one step of the Schelling Segregation model. The algorithm works as:
--  1. mark all unhappy agents
--  2. move all unhappy agents to any random Emtpy cell
--  3. all happy agents stay at the same position
step :: [Cell]           -- ^ All cells of the world
     -> StdGen           -- ^ The random-number generator
     -> Double           -- ^ The ratio of equal neighbours an agent requires to be happy
     -> ([Cell], StdGen) -- ^ Result is the new list of cells and the updated random-number generator
step cs g _ratio = (cs',g')
  where 
    -- cellOutput1 = debugOutput [cs !! emptyIndex]
    -- emptyIndex = findEmpty g cs 0
    -- happyFlags = map (\x -> isHappy x cs _ratio) cs
    (cs', g') = processCell g _ratio cs 0 []

-- Process every agent recursively
-- 1. check the type of agent (Color, Happy)
-- 2. update the model and mark processed agent
-- 3. return new model when all agents are visited
processCell :: RandomGen g => g -> Double -> [Cell] -> Int -> [Int] -> ([Cell], g)
processCell g _ratio cs index visited
    -- all agents are visited
    | index == length cs = (cs, g)
    -- skip visited agent
    | index `elem` visited = processCell g' _ratio cs next_index visited
    -- skip empty agent
    | (snd $ cs !! index) == Nothing = processCell g' _ratio cs next_index (visited ++ [index])
    -- skip happy agnet
    | happyFlag = processCell g' _ratio cs next_index (visited ++ [index])
    -- update model for unhappy agent
    | otherwise =  processCell g' _ratio cs' next_index (visited ++ [index, new_index])
  where
    happyFlag = isHappy (cs !! index) cs _ratio
    next_index = index + 1

    -- find empty cell for unhappy agent and swap them
    new_index = findEmpty g' cs x
    cs' = swapElem cs index new_index
    -- ramdon start for search an empty cell
    (x, g') = randomR (0,length cs - 1) g

-- Swap between empty agent and unhappy agent
-- modified from https://stackoverflow.com/questions/30551033/swap-two-elements-in-a-list-by-its-indices
swapElem :: [Cell] -> Int -> Int -> [Cell]
swapElem cs aIndex bIndex
    | (aIndex > bIndex) = swapElem cs bIndex aIndex
    | otherwise = cs'
  where
    aElem = cs !! aIndex
    bElem = cs !! bIndex
    -- swap position but not swap color
    aElemNew = (fst bElem, snd aElem)
    bElemNew = (fst aElem, snd bElem)
    leftElems = take aIndex cs
    midElems = take (bIndex - aIndex - 1) (drop (aIndex + 1) cs)
    rightElems = drop (bIndex + 1) cs
    cs' = leftElems ++ [bElemNew] ++ midElems ++ [aElemNew] ++ rightElems

-- Find a empty agent and return its index in model
findEmpty :: RandomGen g => g -> [Cell] -> Int -> Int
findEmpty g cs index =  
  -- return present index if the agent is an empty one
  if (snd $ cs !! index) == Nothing then index
  -- find next one if not empty
  else findEmpty g' cs x
  where
    (x, g') = randomR (0,length cs - 1) g

-- Check whether the agent is happy
-- 1. find out all neighbours
-- 2. find out neighbours with same color
-- 3. get the ratio and decide its happy or not
isHappy :: Cell -> [Cell] -> Double -> Bool
isHappy cell cs _ratio
    | this_ratio >= _ratio = True
    | this_ratio < _ratio = False
    | otherwise = False
  where
    this_ratio = if (length all_neighbours) == 0 then 0.0 else (fromIntegral (length true_neighbours))  / (fromIntegral (length all_neighbours))
    true_neighbours = filter (\x -> (snd cell) == (snd x)) all_neighbours
    all_neighbours = filter (\x -> (getNeighbour cell x)) cs
    getNeighbour :: Cell -> Cell -> Bool
    getNeighbour self other
        | delta_x > 1 || delta_y > 1 = False
        | delta_x == 0 && delta_y == 0 = False
        | otherwise = True
      where
        delta_x = abs ((fst (fst self)) - (fst (fst other)))
        delta_y = abs ((snd (fst self)) - (snd (fst other)))