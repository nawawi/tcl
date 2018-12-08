# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset sl DAYS_OF_WEEK_ABBREV [list \
        "Ned"\
        "Pon"\
        "Tor"\
        "Sre"\
<<<<<<< HEAD
<<<<<<< HEAD
        "\u010cet"\
=======
        "Čet"\
>>>>>>> upstream/master
=======
        "Čet"\
>>>>>>> upstream/master
        "Pet"\
        "Sob"]
    ::msgcat::mcset sl DAYS_OF_WEEK_FULL [list \
        "Nedelja"\
        "Ponedeljek"\
        "Torek"\
        "Sreda"\
<<<<<<< HEAD
<<<<<<< HEAD
        "\u010cetrtek"\
=======
        "Četrtek"\
>>>>>>> upstream/master
=======
        "Četrtek"\
>>>>>>> upstream/master
        "Petek"\
        "Sobota"]
    ::msgcat::mcset sl MONTHS_ABBREV [list \
        "jan"\
        "feb"\
        "mar"\
        "apr"\
        "maj"\
        "jun"\
        "jul"\
        "avg"\
        "sep"\
        "okt"\
        "nov"\
        "dec"\
        ""]
    ::msgcat::mcset sl MONTHS_FULL [list \
        "januar"\
        "februar"\
        "marec"\
        "april"\
        "maj"\
        "junij"\
        "julij"\
        "avgust"\
        "september"\
        "oktober"\
        "november"\
        "december"\
        ""]
<<<<<<< HEAD
<<<<<<< HEAD
    ::msgcat::mcset sl BCE "pr.n.\u0161."
=======
    ::msgcat::mcset sl BCE "pr.n.š."
>>>>>>> upstream/master
=======
    ::msgcat::mcset sl BCE "pr.n.š."
>>>>>>> upstream/master
    ::msgcat::mcset sl CE "po Kr."
    ::msgcat::mcset sl DATE_FORMAT "%Y.%m.%e"
    ::msgcat::mcset sl TIME_FORMAT "%k:%M:%S"
    ::msgcat::mcset sl DATE_TIME_FORMAT "%Y.%m.%e %k:%M:%S %z"
}
