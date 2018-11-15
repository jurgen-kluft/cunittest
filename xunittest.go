package main

import (
	"github.com/jurgen-kluft/xcode"
	"github.com/jurgen-kluft/xunittest/package"
)

func main() {
	xcode.Init()
	xcode.Generate(xunittest.GetPackage())
}
