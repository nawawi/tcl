if {![package vsatisfies [package provide Tcl] 8.6-]} {return}
<<<<<<< HEAD
package ifneeded http 2.8.10 [list tclPkgSetup $dir http 2.8.10 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
=======
package ifneeded http 2.8.11 [list tclPkgSetup $dir http 2.8.11 {{http.tcl source {::http::config ::http::formatQuery ::http::geturl ::http::reset ::http::wait ::http::register ::http::unregister ::http::mapReply}}}]
>>>>>>> upstream/master
