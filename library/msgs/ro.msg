# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset ro DAYS_OF_WEEK_ABBREV [list \
        "D"\
        "L"\
        "Ma"\
        "Mi"\
        "J"\
        "V"\
        "S"]
    ::msgcat::mcset ro DAYS_OF_WEEK_FULL [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "duminic\u0103"\
=======
        "duminică"\
>>>>>>> upstream/master
        "luni"\
        "marţi"\
        "miercuri"\
        "joi"\
        "vineri"\
<<<<<<< HEAD
        "s\u00eemb\u0103t\u0103"]
=======
        "duminică"\
        "luni"\
        "marţi"\
        "miercuri"\
        "joi"\
        "vineri"\
        "sîmbătă"]
>>>>>>> upstream/master
=======
        "sîmbătă"]
>>>>>>> upstream/master
    ::msgcat::mcset ro MONTHS_ABBREV [list \
        "Ian"\
        "Feb"\
        "Mar"\
        "Apr"\
        "Mai"\
        "Iun"\
        "Iul"\
        "Aug"\
        "Sep"\
        "Oct"\
        "Nov"\
        "Dec"\
        ""]
    ::msgcat::mcset ro MONTHS_FULL [list \
        "ianuarie"\
        "februarie"\
        "martie"\
        "aprilie"\
        "mai"\
        "iunie"\
        "iulie"\
        "august"\
        "septembrie"\
        "octombrie"\
        "noiembrie"\
        "decembrie"\
        ""]
    ::msgcat::mcset ro BCE "d.C."
<<<<<<< HEAD
<<<<<<< HEAD
    ::msgcat::mcset ro CE "\u00ee.d.C."
=======
    ::msgcat::mcset ro CE "î.d.C."
>>>>>>> upstream/master
=======
    ::msgcat::mcset ro CE "î.d.C."
>>>>>>> upstream/master
    ::msgcat::mcset ro DATE_FORMAT "%d.%m.%Y"
    ::msgcat::mcset ro TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset ro DATE_TIME_FORMAT "%d.%m.%Y %H:%M:%S %z"
}
