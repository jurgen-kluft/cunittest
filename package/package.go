package cunittest

import (
	"github.com/jurgen-kluft/ccode/denv"
)

// GetPackage returns the package object of 'cbase'
func GetPackage() *denv.Package {

    mainpkg := denv.NewPackage("github.com\\jurgen-kluft", "cunittest")

	// 'cunittest' library
	mainlib := denv.SetupCppTestLibProject(mainpkg,"cunittest")

	// 'cunittest' test project
	maintest := denv.SetupCppTestProject(mainpkg, "cunittest_test")
	maintest.AddDependency(mainlib)

	mainpkg.AddMainLib(mainlib)
	mainpkg.AddUnittest(maintest)
	return mainpkg
}
