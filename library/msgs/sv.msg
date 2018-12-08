# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset sv DAYS_OF_WEEK_ABBREV [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "s\u00f6"\
        "m\u00e5"\
=======
        "sö"\
        "må"\
>>>>>>> upstream/master
=======
        "sö"\
        "må"\
>>>>>>> upstream/master
        "ti"\
        "on"\
        "to"\
        "fr"\
<<<<<<< HEAD
<<<<<<< HEAD
        "l\u00f6"]
    ::msgcat::mcset sv DAYS_OF_WEEK_FULL [list \
        "s\u00f6ndag"\
        "m\u00e5ndag"\
=======
        "lö"]
    ::msgcat::mcset sv DAYS_OF_WEEK_FULL [list \
        "söndag"\
        "måndag"\
>>>>>>> upstream/master
=======
        "lö"]
    ::msgcat::mcset sv DAYS_OF_WEEK_FULL [list \
        "söndag"\
        "måndag"\
>>>>>>> upstream/master
        "tisdag"\
        "onsdag"\
        "torsdag"\
        "fredag"\
<<<<<<< HEAD
<<<<<<< HEAD
        "l\u00f6rdag"]
=======
        "lördag"]
>>>>>>> upstream/master
=======
        "lördag"]
>>>>>>> upstream/master
    ::msgcat::mcset sv MONTHS_ABBREV [list \
        "jan"\
        "feb"\
        "mar"\
        "apr"\
        "maj"\
        "jun"\
        "jul"\
        "aug"\
        "sep"\
        "okt"\
        "nov"\
        "dec"\
        ""]
    ::msgcat::mcset sv MONTHS_FULL [list \
        "januari"\
        "februari"\
        "mars"\
        "april"\
        "maj"\
        "juni"\
        "juli"\
        "augusti"\
        "september"\
        "oktober"\
        "november"\
        "december"\
        ""]
    ::msgcat::mcset sv BCE "f.Kr."
    ::msgcat::mcset sv CE "e.Kr."
    ::msgcat::mcset sv DATE_FORMAT "%Y-%m-%d"
    ::msgcat::mcset sv TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset sv DATE_TIME_FORMAT "%Y-%m-%d %H:%M:%S %z"
}
