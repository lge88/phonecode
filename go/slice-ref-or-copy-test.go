package main

import "fmt"

func main() {

	a := []string { "abc", "def", "ghi" }
	b := a[1:2]

	fmt.Println("a:", a)
	fmt.Println("b:", b)

	b[0] = "xxx"
	fmt.Println("b[0] = \"xxx\"")

	fmt.Println("a:", a)
	fmt.Println("b:", b)
}
