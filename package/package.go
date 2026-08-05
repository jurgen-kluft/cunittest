package cunittest

import (
	csdk "github.com/jurgen-kluft/csdk/package"
	"github.com/jurgen-kluft/go-ide/denv"
)

const (
	repo_path = "github.com\\jurgen-kluft\\"
	repo_name = "cunittest"
)

// GetPackage returns the package object of 'cunittest'
func GetPackage() *denv.Package {
	// Dependencies
	sdk_pkg := csdk.GetPackage()

	// The main (cunittest) package
	main_pkg := denv.NewPackage(repo_path, repo_name)
	main_pkg.AddPackage(sdk_pkg)

	// 'cunittest' library
	mainlib := denv.SetupCppLibProject(main_pkg, repo_name)
	mainlib.AddDependencies(sdk_pkg.GetMainLib())

	// 'cunittest' library for unittest
	testlib := denv.SetupCppTestLibProject(main_pkg, repo_name)

	// 'cunittest' unittest project
	maintest := denv.SetupCppTestProject(main_pkg, repo_name)
	maintest.AddDependency(testlib)

	main_pkg.AddMainLib(mainlib)
	main_pkg.AddTestLib(testlib)
	main_pkg.AddUnittest(maintest)
	return main_pkg
}
