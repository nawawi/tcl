<<<<<<< HEAD
if {([info commands ::tcl::pkgconfig] eq "")
	|| ([info sharedlibextension] ne ".dll")} return
=======
if {![package vsatisfies [package provide Tcl] 8.5-]} return
if {[info sharedlibextension] != ".dll"} return
<<<<<<< HEAD
>>>>>>> upstream/master
if {[::tcl::pkgconfig get debug]} {
    package ifneeded registry 1.3.5 \
            [list load [file join $dir tclreg13g.dll] registry]
} else {
    package ifneeded registry 1.3.5 \
            [list load [file join $dir tclreg13.dll] registry]
}
=======
package ifneeded registry 1.3.5 \
        [list load [file join $dir tclreg13.dll] registry]
>>>>>>> upstream/master
