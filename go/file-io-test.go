package main

import (
	"fmt"
	"os"
	"bufio"
	"strings"
)

func main() {
	args := os.Args[1:]
	fname := args[0]

	file, _ := os.Open(fname)

	scanner := bufio.NewScanner(file)

	mySplit := func(data []byte, atEOF bool) (advance int, token []byte, err error) {
		// if atEOF { err = "EOF"; return }
		advance, token, err = 1, data[0:1], nil
		fmt.Printf("advance: %+v\n", advance) // output for debug
		fmt.Printf("token: %+v\n", token) // output for debug
		fmt.Printf("err: %+v\n", err) // output for debug

		return
	}

	scanner.Split(mySplit)

	for scanner.Scan() {
		line := scanner.Text()
		fmt.Println(strings.ToUpper(line))
	}

}
