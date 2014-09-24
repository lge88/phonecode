// Playground - noun: a place where people can play

import UIKit
import Swift

let words_file = "/Users/lige/Desktop/MyPlayground.playground/Resources/words.txt"

let content = String.stringWithContentsOfFile(words_file, encoding: NSUTF8StringEncoding, error: nil)

var word_list = content?.componentsSeparatedByString("\n")

func head<T>(list:[T], n:Int) -> [T]{
    var out = [T](), i:Int = 0
    while i < n {
        out.append(list[i])
        i += 1
    }
    return out
}

//word_list = head(word_list!, 50)

word_list = ["I", "love", "emacs", "is", "the", "best", "swift", "ema", "csist", "he"]

let phone_digit_to_letters:[Character: String] = [
    "2": "ABC",
    "3": "DEF",
    "4": "GHI",
    "5": "JKL",
    "6": "MNO",
    "7": "PQRS",
    "8": "TUV",
    "9": "WXYZ"
]

func build_letter_to_digit_map(digit_to_letters: [Character: String]) -> [Character: Character] {
    var out = [Character: Character]()
    for (digit,letters) in digit_to_letters {
        for letter in letters.lowercaseString {
            out[letter] = digit
        }
    }
    return out
}

let letter_to_digit = build_letter_to_digit_map(phone_digit_to_letters)

func letter_to_digit(letter:Character) -> String {
    let c:Character? = letter_to_digit[letter]
    return (c != nil) ? String(c!) : ""
}

func word_to_digits(word:String) -> String {
    let letters = [Character](word.lowercaseString)
    return "".join(letters.map(letter_to_digit))
}

func build_digits_to_word_map(word_list:[String]) -> [String:String] {
    var out = [String:String]()
    for word in word_list {
        let digits = word_to_digits(word)
        out[digits] = word
    }
    return out
}


let digits_to_word_map = build_digits_to_word_map(word_list!)

word_to_digits("I love swift")
word_to_digits("emacs is the best")

// "abcd" -> ("", "abcd"), ("a","bcd"), ("ab", "cd"), ("abc", "d"), ("abcd", "")
struct StringSplitEnumator: SequenceType {
    let len:Int
    let s:String

    init(string:String) {
        self.s = string
        self.len = countElements(s)
    }

    func generate() -> GeneratorOf<(String,String)> {
        var i = 0
        let len = self.len
        let s = self.s

        return GeneratorOf<(String,String)> {
            if i > len { return nil }

            let idx = advance(s.startIndex, i)
            let nextItem = (s.substringToIndex(idx), s.substringFromIndex(idx))
            i += 1
            return nextItem
        }
    }
}

func _digits_to_possible_sentences(digits:String, sofar:String, inout sentences:[String]) {
    for parts in StringSplitEnumator(string: digits) {
        let (left, right) = parts
        if countElements(left) > 0 {
            let word = digits_to_word_map[left]
            if word != nil {
                let sentence = sofar + " " + word!
                if countElements(right) == 0 {
                    sentences.append(sentence)
                } else {
                    _digits_to_possible_sentences(right, sentence, &sentences)
                }
            }
        }
    }
}

func translate(digits:String) -> [String] {
    var sofar = "", out = [String]()
    _digits_to_possible_sentences(digits, sofar, &out)
    return out
}

translate("36227478432378")
translate("4568379438")
