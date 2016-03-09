package xunittest

import (
	"github.com/jurgen-kluft/xcode/denv"
)

// GetProject returns the Project information of 'xunittest'
func GetProject() *denv.Project {
	project := denv.SetupDefaultCppProject("xunittest", "github.com\\jurgen-kluft")
	return project
}
