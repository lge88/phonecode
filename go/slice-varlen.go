package main

import "fmt"

func main() {
	a := []string { "abc", "def", "ghi" }

	fmt.Println("a:", a, "len of a:", len(a), "cap of a:", cap(a))

	b := a[:]
	fmt.Println("b := a[:]")
	fmt.Println("b:", b, "len of b:", len(b), "cap of b:", cap(b))

	fmt.Println("a[1] = \"xxx\"")
	a[1] = "xxx"

	fmt.Println("a:", a, "len of a:", len(a), "cap of a:", cap(a))
	fmt.Println("b:", b, "len of b:", len(b), "cap of b:", cap(b))

	fmt.Println("b := append(b, \"jkl\")")
	b = append(b, "jkl")

	fmt.Println("a[1] = \"yyy\"")
	a[1] = "yyy"

	fmt.Println("a:", a, "len of a:", len(a), "cap of a:", cap(a))
	fmt.Println("b:", b, "len of b:", len(b), "cap of b:", cap(b))

}
