import Data.Char
import Data.List
import Data.Maybe
import qualified Data.Map.Lazy as Map
import System.Environment
import System.Exit

data Options = Options {
  wordsFile :: String,
  phoneFile :: String,
  reversed :: Bool
  } deriving (Show)

parseOptions :: [String] -> Options
parseOptions ("-r":xs) = (parseOptions xs) { reversed = True }
parseOptions ("-w":x:xs) = (parseOptions xs) { wordsFile = x }
parseOptions ("-p":x:xs) = (parseOptions xs) { phoneFile = x }
parseOptions [] = Options {
  wordsFile = "words.txt",
  phoneFile = "phone.txt",
  reversed = False
  }

splitByComma :: String -> [String]
splitByComma ("") = []
splitByComma str =
  let (pre, post) = span (/=',') str in
   if null post then [pre] else [pre] ++ splitByComma(tail post)

buildWordList :: String -> IO [String]
buildWordList (wordsFile) = fmap lines $ readFile wordsFile

buildLetterToDigitMap_ :: [String] -> (Map.Map Char Char)
buildLetterToDigitMap_ strs =
  Map.fromList $ concatMap (letterToDigitPairs .
             (\xs -> (xs!!1, xs!!0)) .
             splitByComma) strs
  where
    letterToDigitPairs (letters, digit) =
      map (\letter -> (letter, head digit)) letters

buildLetterToDigitMap :: String -> IO (Map.Map Char Char)
buildLetterToDigitMap (phoneFile) = do
  pairs <- fmap lines $ readFile phoneFile
  return $ buildLetterToDigitMap_ pairs

encodeWord:: String -> (Map.Map Char Char) -> String
encodeWord str dict = concatMap (encodeChar . toUpper) str where
  encodeChar c =
    let d = Map.lookup c dict in
    if isJust d then [fromJust d] else []

buildDigitsToWordsMap :: [String] -> (Map.Map Char Char)
                    -> (Map.Map String [String])

buildDigitsToWordsMap words l2d =
  foldr addOneWord emptyMap words
  where
    emptyMap = Map.fromList []
    addOneWord word sofar =
      let digits = encodeWord word l2d in
       Map.insertWith (++) digits [word] sofar

decodeDigits:: String -> (Map.Map String [String])
            -> [[String]]

-- d2w: the map from digits to words
-- "43556" -> "hello"
decodeDigits "" _ = [[]]
decodeDigits digits d2w =
  let partitions = map (\i -> splitAt i digits) [1..(length digits)]
  in
   foldr processEachPartition [] partitions
   where
     processEachPartition partition sofar =
       let (prefix, rest) = partition
           firstWords = Map.lookup prefix d2w
       in
        if isJust firstWords then
          let partitialSolutions = decodeDigits rest d2w
              words = fromJust firstWords
              solutionForThisPartition = [
                [firstWord] ++ partitialSolution
                | firstWord <- words,
                  partitialSolution <- partitialSolutions
                ]
          in
           solutionForThisPartition ++ sofar
        else
          sofar

lineByLine :: (String -> String) -> IO String
lineByLine f = do
  line <- getLine
  putStrLn(f line)
  lineByLine f

main = do
  args <- getArgs
  let options = parseOptions args

  wl <- buildWordList $ wordsFile options
  l2d <- buildLetterToDigitMap $ phoneFile options

  let translate =
        if (reversed options) then
          let encode sentence =
                sentence ++ ": " ++ (encodeWord sentence l2d)
          in
           encode
        else
          let d2w = (buildDigitsToWordsMap wl l2d)
              decode digits = intercalate "\n" (map (\ss -> digits ++ ": " ++ (intercalate " " ss)) (decodeDigits digits d2w))
          in
           decode

  lineByLine translate
