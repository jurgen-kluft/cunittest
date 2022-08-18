package cunittest

import (
	"github.com/jurgen-kluft/ccode/denv"
)

// GetPackage returns the package object of 'cbase'
func GetPackage() *denv.Package {

	// 'cunittest' library
	mainlib := denv.SetupDefaultCppLibProject("cunittest", "github.com\\jurgen-kluft\\cunittest")

	// 'cunittest' test project
	maintest := denv.SetupDefaultCppTestProject("cunittest_test", "github.com\\jurgen-kluft\\cunittest")
	maintest.Dependencies = append(maintest.Dependencies, mainlib)

	mainpkg := denv.NewPackage("cunittest")
	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
