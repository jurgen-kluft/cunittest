package main

import (
	ccode "github.com/jurgen-kluft/ccode"
	cpkg "github.com/jurgen-kluft/cunittest/package"
)

func main() {
	ccode.Generate(cpkg.GetPackage())
}
