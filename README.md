# Problem

Given a set of English words (`words.txt`) and the layout of phone
panel (`phone.txt`). Write a program that translates the digits user
input to all possible sentences that consist of given set of
words. Reference:
http://page.mi.fu-berlin.de/prechelt/Biblio/jccpprtTR.pdf .

# Interface

- Each implementation is in a seperate folder, each folder should
  contains a executable `translate`.

- `translate` reads data from stdin and output translated results to
  stdout. `input**.txt` and `output**.txt` are corresponding sample
  inputs and expected outputs.

- `translate` should support 3 command line options:
  `-w|--words-file`, `-p|--phone-file`, `-r|--reverse`

  - `words-file`: The location of the file that defines a set of valid
    words.
  - `phone-file`: The location of the file that defines the layout of
    the phone panel.
  - `reverse`: A flag indicates the direction of the translation. By
    default, it is not enabled, the program should translate digits
    ([0-9]+) to sentences (might be many possible solutions). When the
    -r flag is enabled, the program should translate the sentences to
    digits (unique solution).

- Examples: suppose current directory is the project root, there is a
  python implementation in `./python` folder, the program should have
  following behaviors:

  - `echo 'hello world'  | ./python/translate -r -w words.txt -p phone.txt`

     outputs:
     ```
     hello world: 4355696753
     ```

  - `echo '4355696753'  | ./python/translate -w words.txt -p phone.txt`

     outputs:
    ```
     4355696753: gel Jo world
     4355696753: hello world
    ```

# Test Correctness

Use the script `verify`, for example, to check the correctness of `js`
implementation, run `./verify js`
