# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset lv DAYS_OF_WEEK_ABBREV [list \
        "Sv"\
        "P"\
        "O"\
        "T"\
        "C"\
        "Pk"\
        "S"]
    ::msgcat::mcset lv DAYS_OF_WEEK_FULL [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "sv\u0113tdiena"\
        "pirmdiena"\
        "otrdiena"\
        "tre\u0161diena"\
=======
        "svētdiena"\
        "pirmdiena"\
        "otrdiena"\
        "trešdiena"\
>>>>>>> upstream/master
=======
        "svētdiena"\
        "pirmdiena"\
        "otrdiena"\
        "trešdiena"\
>>>>>>> upstream/master
        "ceturdien"\
        "piektdiena"\
        "sestdiena"]
    ::msgcat::mcset lv MONTHS_ABBREV [list \
        "Jan"\
        "Feb"\
        "Mar"\
        "Apr"\
        "Maijs"\
<<<<<<< HEAD
<<<<<<< HEAD
        "J\u016bn"\
        "J\u016bl"\
=======
        "Jūn"\
        "Jūl"\
>>>>>>> upstream/master
=======
        "Jūn"\
        "Jūl"\
>>>>>>> upstream/master
        "Aug"\
        "Sep"\
        "Okt"\
        "Nov"\
        "Dec"\
        ""]
    ::msgcat::mcset lv MONTHS_FULL [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "janv\u0101ris"\
        "febru\u0101ris"\
=======
        "janvāris"\
        "februāris"\
>>>>>>> upstream/master
        "marts"\
        "aprīlis"\
        "maijs"\
<<<<<<< HEAD
        "j\u016bnijs"\
        "j\u016blijs"\
=======
        "janvāris"\
        "februāris"\
        "marts"\
        "aprīlis"\
        "maijs"\
        "jūnijs"\
        "jūlijs"\
>>>>>>> upstream/master
=======
        "jūnijs"\
        "jūlijs"\
>>>>>>> upstream/master
        "augusts"\
        "septembris"\
        "oktobris"\
        "novembris"\
        "decembris"\
        ""]
<<<<<<< HEAD
<<<<<<< HEAD
    ::msgcat::mcset lv BCE "pm\u0113"
    ::msgcat::mcset lv CE "m\u0113"
=======
    ::msgcat::mcset lv BCE "pmē"
    ::msgcat::mcset lv CE "mē"
>>>>>>> upstream/master
=======
    ::msgcat::mcset lv BCE "pmē"
    ::msgcat::mcset lv CE "mē"
>>>>>>> upstream/master
    ::msgcat::mcset lv DATE_FORMAT "%Y.%e.%m"
    ::msgcat::mcset lv TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset lv DATE_TIME_FORMAT "%Y.%e.%m %H:%M:%S %z"
}
