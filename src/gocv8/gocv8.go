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

func ProcessEvent(ctx *GoCV8, event string, js string){
	event_cstr := C.CString(event)
	js_cstr := C.CString(js)
	C.gocv8_process_event(ctx.cptr, event_cstr, C.size_t(len(event)), js_cstr, C.size_t(len(js)), nil)
}
