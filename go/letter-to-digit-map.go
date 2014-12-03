// TODO: how to declare a map/slice on stack in go?
package main

import (
  "fmt"
	"strings"
  "os"
	"bufio"
)

func BuildLetterToDigitMap(phoneFileName string) map[rune]rune {
	file, _ := os.Open(phoneFileName)
	scanner := bufio.NewScanner(file)

	dict := make(map[rune]rune)
	for scanner.Scan() {
		line := scanner.Text()
		items := strings.SplitN(line, ",", -1)
		digit, letters := items[0][0], items[1]
		for _, letter := range letters {
			dict[letter] = rune(digit)
		}
	}
	return dict
}

func LetterToDigit(letter rune, l2d map[rune]rune) rune {
	digit := l2d[rune(strings.ToUpper(string(letter))[0])]
	if digit == 0 {
		return '-'
	} else {
		return digit
	}
}

func TranslateWordToDigits(word string, l2d map[rune]rune) string {
  return strings.Map(func (x rune) rune {
		return LetterToDigit(x, l2d)
	}, word)
}

func BuildDigitsToWordsMap(phoneFileName string, wordsFileName string) map[string][]string {
	wordsFile, _ := os.Open(wordsFileName)
	scanner := bufio.NewScanner(wordsFile)
	dict := make(map[string][]string)

	l2dMap := BuildLetterToDigitMap(phoneFileName)

	for scanner.Scan() {
		word := scanner.Text()
		digits := TranslateWordToDigits(word, l2dMap)

		if dict[digits] == nil {
			dict[digits] = make([]string, 1)
		}

		dict[digits] = append(dict[digits], word)
	}
	return dict
}

func main() {
  phoneFileName := "../phone.txt"
	wordsFileName := "../words.txt"
	// l2dMap := BuildLetterToDigitMap(phoneFileName)
	d2wMap := BuildDigitsToWordsMap(phoneFileName, wordsFileName)

	// fmt.Println("l2dMap:", l2dMap)
	// for key, val := range l2dMap {
	// 	fmt.Printf("%c -> %c\n", key, val)
	// }
	// fmt.Println("toupper: a -> ", strings.ToUpper("a"))
	// fmt.Println("toupper: A -> ", strings.ToUpper("A"))

	// for _, letter := range "abc" {
	// 	fmt.Printf("%s -> %s\n", string(letter), LetterToDigit(string(letter), l2dMap))
	// }

	fmt.Println("d2wMap:", d2wMap)

}
