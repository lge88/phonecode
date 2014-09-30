# Problem
Given the list of english words (`words.txt`) and the layout of phone
panel (`phone.txt`). Write a program that translates the digits user
input to all possible sentences that consist of given set of words.

# Interface
- Each implementation is in a seperate folder.
- Each implementation folder should contains a executable `translate`.
- `translate` reads from stdin for digits user input, and output
  translated all possible sentences to stdout. `input**.txt` and
  `output**.txt` are sample inputs and outputs.
- `translate` should support 3 command line arguments:
  -w|--words-file, -p|--phone-file, -r|--reverse
- Example usage: suppose there is a python implementation in
  `./python` folder, the program should be run by:
  `cat input01.txt | ./python/translate -w words.txt -p phone.txt`
