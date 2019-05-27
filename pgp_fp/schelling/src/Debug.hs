
-- getColor :: Cell
--          -> [Char]
-- getColor cell = color
--   where
--     color  = case (snd cell) of
--       Just Red   -> ("Red")
--       Just Green -> ("Green")
--       Just Blue  -> ("Blue")
--       Nothing    -> ("Empty")

-- debugOutput :: [Cell]
--             -> [(Coord, [Char])]
-- debugOutput cells = cellOutput
--   where
--     color = map getColor cells
--     pos = map fst cells
--     cellOutput = zip pos color          