package docker

import (
	"fmt"
	"os"
	"os/exec"
)

// Backbone struct (does not use any other structs)
type Filesystem struct {
	RootFS string
	RWPath string

	// Q: What is expected string content of "Layers"?
	Layers []string
}

func (fs *Filesystem) createMountPoints() error {
	// Q: Why 0700 here?
	if err := os.Mkdir(fs.RootFS, 0700); err != nil && !os.IsExist(err) {
		return err
	}
	if err := os.Mkdir(fs.RWPath, 0700); err != nil && !os.IsExist(err) {
		return err
	}
	return nil
}

func (fs *Filesystem) Mount() error {
	// Q: Does this create a file directory onto which to point the mount syscall?
	if err := fs.createMountPoints(); err != nil {
		return err
	}

	// rwBranch = read-write branch
	rwBranch := fmt.Sprintf("%v=rw", fs.RWPath)

	// roBranches = read-only branches
	roBranches := ""
	for _, layer := range fs.Layers {
		roBranches += fmt.Sprintf("%v=ro:", layer)
	}
	branches := fmt.Sprintf("br:%v:%v", rwBranch, roBranches)

	// Q: Mounting a filesystem here... what are the default files in the mounted filesystem?
	// Q: Are "inodes" created?
	cmd := exec.Command("mount", "-t", "aufs", "-o", branches, "none", fs.RootFS)
	if err := cmd.Run(); err != nil {
		return err
	}
	return nil
}

func (fs *Filesystem) Umount() error {
	return exec.Command("umount", fs.RootFS).Run()
}

func newFilesystem(rootfs string, rwpath string, layers []string) *Filesystem {
	return &Filesystem{
		RootFS: rootfs,
		RWPath: rwpath,
		Layers: layers,
	}
}
