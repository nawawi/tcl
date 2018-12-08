if {![package vsatisfies [package provide Tcl] 8.6-]} {return}
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
package ifneeded http 2.8.10 [list tclPkgSetup $dir http 2.8.10 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
=======
package ifneeded http 2.8.11 [list tclPkgSetup $dir http 2.8.11 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
>>>>>>> upstream/master
=======
package ifneeded http 2.8.12 [list tclPkgSetup $dir http 2.8.12 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
>>>>>>> upstream/master
=======
package ifneeded http 2.8.13 [list tclPkgSetup $dir http 2.8.13 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
>>>>>>> upstream/master
=======
package ifneeded http 2.9.0 [list tclPkgSetup $dir http 2.9.0 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
package ifneeded cookiejar 0.1 [list source [file join $dir cookiejar.tcl]]
package ifneeded tcl::idna 1.0 [list source [file join $dir idna.tcl]]
>>>>>>> upstream/master
