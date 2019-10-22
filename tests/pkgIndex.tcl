#! /usr/bin/env tclsh

<<<<<<< HEAD
<<<<<<< HEAD
package ifneeded tcltests 0.1 {
	source [file dirname [file dirname [file normalize [info script]/...]]]/tcltests.tcl
	package provide tcltests 0.1
}
=======
package ifneeded tcltests 0.1 "
    source [list $dir/tcltests.tcl]
    package provide tcltests 0.1
"
>>>>>>> upstream/master
=======
package ifneeded tcltests 0.1 [list source $dir/tcltests.tcl]
>>>>>>> upstream/master
