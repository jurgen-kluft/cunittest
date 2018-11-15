package xunittest

import (
	"github.com/jurgen-kluft/xcode/denv"
)

// GetPackage returns the package object of 'xbase'
func GetPackage() *denv.Package {

	// 'xunittest' library
	mainlib := denv.SetupDefaultCppLibProject("xunittest", "github.com\\jurgen-kluft\\xunittest")

	// 'xunittest' test project
	maintest := denv.SetupDefaultCppTestProject("xunittest_test", "github.com\\jurgen-kluft\\xunittest")
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg := denv.NewPackage("xunittest")
	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
