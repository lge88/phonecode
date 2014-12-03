package main


import (
	"fmt"
)

func main()  {
	arr := make([]string, 1)

	arr = append(arr, "12")
	arr = append(arr, "12")
	arr = append(arr, "12")

	fmt.Printf("%+v\n", arr) // output for debug

}
