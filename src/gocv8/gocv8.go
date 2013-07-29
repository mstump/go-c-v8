package gocv8
// #cgo LDFLAGS: -lgocv8 -lv8 -pthread
// #include <stdlib.h>
// #include <gocv8/gocv8.h>
import "C"
import "unsafe"

type GoCV8 struct {
	cptr unsafe.Pointer
}

func NewContext()(ctx *GoCV8) {
	c := new(GoCV8)
	c.cptr = C.gocv8_context_new()
	return c
}

func FreeContext(ctx *GoCV8) {
	C.gocv8_context_free(ctx.cptr)
	ctx.cptr = nil
}