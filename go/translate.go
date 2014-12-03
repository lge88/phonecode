
package main

import "os"
import "fmt"
import "bufio"
import "strings"

type Options struct {
	wordsFile, phoneFile string
	reverse bool
}

func (options *Options) String() string {
	return fmt.Sprintf(
		"wordsFile: %v\n" +
			"phoneFile: %v\n" +
			"reverse: %v\n",
		options.wordsFile,
		options.phoneFile,
		options.reverse)
}

func parseOptions(args []string) Options {
	options := Options { "../words.txt", "../phone.txt", false }
	parseOptionsRecursive(args, &options)
	return options;
}

func parseOptionsRecursive(args []string, options *Options) {
	if len(args) <= 0 { return }

	flag := args[0]
	switch flag {
	case "-w", "--words-file":
		options.wordsFile = args[1]
		args = args[2:]
	case "-p", "--phone-file":
		options.phoneFile = args[1]
		args = args[2:]
	case "-r", "--reverse":
		options.reverse = true
		args = args[1:]
	default:
		fmt.Println("Unknown flag %s, abort!", flag)
	}

	parseOptionsRecursive(args, options)
}

type LineWriter interface {
	WriteLine(line string)
	Flush()
}

type FileLineWriter struct {
	file *os.File
}

func NewFileLineWriter(file *os.File) FileLineWriter {
	var liner FileLineWriter
	liner.file = file
	return liner
}

func (liner FileLineWriter) WriteLine(line string) {
	liner.file.WriteString(line + "\n")
}

func (liner FileLineWriter) Flush() {
	liner.file.Sync()
}

type LineReader interface {
	ReadLine() bool
	Text() string
}

type FileLineReader struct {
	scanner *bufio.Scanner
}

func NewFileLineReader(file *os.File) FileLineReader {
	var reader FileLineReader
	scanner := bufio.NewScanner(file)
	reader.scanner = scanner
	return reader
}

func (reader FileLineReader) ReadLine() bool {
	return reader.scanner.Scan()
}

func (reader FileLineReader) Text() string {
	return reader.scanner.Text()
}

type Translator struct {
	l2dMap map[rune]rune
	d2wMap map[string][]string
	reverse bool
}

func buildLetterToDigitMap(phoneFileName string) map[rune]rune {
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

func (t *Translator) init(phoneFileName string, wordsFileName string, reverse bool) {
	wordsFile, _ := os.Open(wordsFileName)
	scanner := bufio.NewScanner(wordsFile)
	d2wMap := make(map[string][]string)

	l2dMap := buildLetterToDigitMap(phoneFileName)

	for scanner.Scan() {
		word := scanner.Text()
		digits := t.wordToDigits(word)

		if d2wMap[digits] == nil {
			d2wMap[digits] = make([]string, 1)
		}

		d2wMap[digits] = append(d2wMap[digits], word)
	}

	t.l2dMap = l2dMap
	t.d2wMap = d2wMap
	t.reverse = reverse
}

func (t *Translator) letterToDigit(letter rune) rune {
	l2d := t.l2dMap
	digit := l2d[rune(strings.ToUpper(string(letter))[0])]
	if digit == 0 {
		return '-'
	} else {
		return digit
	}
}

func (t *Translator) wordToDigits(word string) string {
  res := strings.Map(func (x rune) rune {
		return t.letterToDigit(x)
	}, word)
	r := strings.NewReplacer("-", "")
	return fmt.Sprintf("%s", r.Replace(res))
}

func (t *Translator) translate(digits string, writer LineWriter) {
	writer.WriteLine(digits + ":")
}

func (t *Translator) reverseTranslate(word string, writer LineWriter) {
	writer.WriteLine(word + ": " + t.wordToDigits(word))
}


func (t *Translator) Run(reader LineReader, writer LineWriter) {
	reverse := t.reverse
	var transform func (string, LineWriter)
	if reverse {
		transform = t.reverseTranslate
	} else {
		transform = t.translate
	}

	for reader.ReadLine() {
		transform(reader.Text(), writer)
	}
}

func NewTranslator (phoneFileName string, wordsFileName string, reverse bool) Translator {
	var t Translator
	t.init(phoneFileName, wordsFileName, reverse)
	return t
}


func main() {
	args := os.Args[1:]
	options := parseOptions(args)

  phoneFileName := options.phoneFile
	wordsFileName := options.wordsFile
	reverse := options.reverse

	reader := NewFileLineReader(os.Stdin)
	writer := NewFileLineWriter(os.Stdout)

	translator := NewTranslator(phoneFileName, wordsFileName, reverse)
	translator.Run(reader, writer)
}
