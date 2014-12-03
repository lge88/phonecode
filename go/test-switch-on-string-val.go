package main

import (
	"fmt"
	"os"
)

func main() {
	flag := os.Args[1]
	fmt.Printf("%+v\n", flag) // output for debug

	switch flag {
	case "a":
		fmt.Println("a")
	case "b":
		fmt.Println("b")
	case "c":
		fmt.Println("c")
	default:
		fmt.Println("default")
	}
}
