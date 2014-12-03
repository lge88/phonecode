// TODO: figure out the difference between interface and
// pointer to interface
// Does go has a concept of reference?
package main

import (
	"os"
	"line_writer"
	// "io"
  // "bufio"
  "fmt"
  "syscall"
)

func doit(lineWriter line_writer.LineWriter) {
  for i := 0; i < 10; i += 1 {
		lineWriter.WriteLine(fmt.Sprintf("%d", i))
	}
	lineWriter.Flush()
}

func main() {
  liner := NewLiner(os.Stdout)
	doit(liner)
}
