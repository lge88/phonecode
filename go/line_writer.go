// TODO: figure out the difference between interface and
// pointer to interface
// Does go has a concept of reference?
package line_writer

import "os"
import "syscall"

type LineWriter interface {
	WriteLine(line string)
	Flush()
}

type Liner struct {
	file *os.File
}

func NewLiner(file *os.File) Liner {
	var liner Liner
	liner.file = file
	return liner
}

func (liner Liner) WriteLine(line string) {
	bs, _ := syscall.ByteSliceFromString(line + "\n")
	liner.file.Write(bs)
}

func (liner Liner) Flush() {
	liner.file.Sync()
}
