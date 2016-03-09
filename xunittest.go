package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xunittest/package"
)

func main() {
	xcode.Generate(xunittest.GetProject())
}
