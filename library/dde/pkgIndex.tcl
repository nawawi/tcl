<<<<<<< HEAD
if {([info commands ::tcl::pkgconfig] eq "")
	|| ([info sharedlibextension] ne ".dll")} return
=======
if {![package vsatisfies [package provide Tcl] 8.5-]} return
if {[info sharedlibextension] != ".dll"} return
<<<<<<< HEAD
>>>>>>> upstream/master
if {[::tcl::pkgconfig get debug]} {
    package ifneeded dde 1.4.3 [list load [file join $dir tcldde14g.dll] dde]
} else {
    package ifneeded dde 1.4.3 [list load [file join $dir tcldde14.dll] dde]
}
=======
package ifneeded dde 1.4.3 [list load [file join $dir tcldde14.dll] dde]
>>>>>>> upstream/master
