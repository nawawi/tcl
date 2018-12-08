# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset fr DAYS_OF_WEEK_ABBREV [list \
        "dim."\
        "lun."\
        "mar."\
        "mer."\
        "jeu."\
        "ven."\
        "sam."]
    ::msgcat::mcset fr DAYS_OF_WEEK_FULL [list \
        "dimanche"\
        "lundi"\
        "mardi"\
        "mercredi"\
        "jeudi"\
        "vendredi"\
        "samedi"]
    ::msgcat::mcset fr MONTHS_ABBREV [list \
        "janv."\
<<<<<<< HEAD
<<<<<<< HEAD
        "f\u00e9vr."\
=======
        "févr."\
>>>>>>> upstream/master
=======
        "févr."\
>>>>>>> upstream/master
        "mars"\
        "avr."\
        "mai"\
        "juin"\
        "juil."\
<<<<<<< HEAD
<<<<<<< HEAD
        "ao\u00fbt"\
=======
        "août"\
>>>>>>> upstream/master
        "sept."\
        "oct."\
        "nov."\
        "déc."\
        ""]
    ::msgcat::mcset fr MONTHS_FULL [list \
        "janvier"\
<<<<<<< HEAD
        "f\u00e9vrier"\
=======
        "août"\
        "sept."\
        "oct."\
        "nov."\
        "déc."\
        ""]
    ::msgcat::mcset fr MONTHS_FULL [list \
        "janvier"\
        "février"\
>>>>>>> upstream/master
=======
        "février"\
>>>>>>> upstream/master
        "mars"\
        "avril"\
        "mai"\
        "juin"\
        "juillet"\
<<<<<<< HEAD
<<<<<<< HEAD
        "ao\u00fbt"\
        "septembre"\
        "octobre"\
        "novembre"\
        "d\u00e9cembre"\
=======
        "août"\
        "septembre"\
        "octobre"\
        "novembre"\
        "décembre"\
>>>>>>> upstream/master
=======
        "août"\
        "septembre"\
        "octobre"\
        "novembre"\
        "décembre"\
>>>>>>> upstream/master
        ""]
    ::msgcat::mcset fr BCE "av. J.-C."
    ::msgcat::mcset fr CE "ap. J.-C."
    ::msgcat::mcset fr DATE_FORMAT "%e %B %Y"
    ::msgcat::mcset fr TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset fr DATE_TIME_FORMAT "%e %B %Y %H:%M:%S %z"
}
