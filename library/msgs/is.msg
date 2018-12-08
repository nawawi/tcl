# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset is DAYS_OF_WEEK_ABBREV [list \
        "sun."\
<<<<<<< HEAD
<<<<<<< HEAD
        "m\u00e1n."\
        "\u00feri."\
        "mi\u00f0."\
=======
        "mán."\
        "þri."\
        "mið."\
>>>>>>> upstream/master
        "fim."\
        "fös."\
        "lau."]
    ::msgcat::mcset is DAYS_OF_WEEK_FULL [list \
        "sunnudagur"\
        "mánudagur"\
        "þriðjudagur"\
        "miðvikudagur"\
        "fimmtudagur"\
<<<<<<< HEAD
        "f\u00f6studagur"\
=======
        "mán."\
        "þri."\
        "mið."\
        "fim."\
        "fös."\
        "lau."]
    ::msgcat::mcset is DAYS_OF_WEEK_FULL [list \
        "sunnudagur"\
        "mánudagur"\
        "þriðjudagur"\
        "miðvikudagur"\
        "fimmtudagur"\
        "föstudagur"\
>>>>>>> upstream/master
=======
        "föstudagur"\
>>>>>>> upstream/master
        "laugardagur"]
    ::msgcat::mcset is MONTHS_ABBREV [list \
        "jan."\
        "feb."\
        "mar."\
        "apr."\
<<<<<<< HEAD
<<<<<<< HEAD
        "ma\u00ed"\
        "j\u00fan."\
        "j\u00fal."\
        "\u00e1g\u00fa."\
=======
        "maí"\
        "jún."\
        "júl."\
        "ágú."\
>>>>>>> upstream/master
        "sep."\
        "okt."\
        "nóv."\
        "des."\
        ""]
    ::msgcat::mcset is MONTHS_FULL [list \
        "janúar"\
        "febrúar"\
        "mars"\
        "apríl"\
        "maí"\
        "júní"\
        "júlí"\
        "ágúst"\
        "september"\
<<<<<<< HEAD
        "okt\u00f3ber"\
        "n\u00f3vember"\
=======
        "maí"\
        "jún."\
        "júl."\
        "ágú."\
        "sep."\
        "okt."\
        "nóv."\
        "des."\
        ""]
    ::msgcat::mcset is MONTHS_FULL [list \
        "janúar"\
        "febrúar"\
        "mars"\
        "apríl"\
        "maí"\
        "júní"\
        "júlí"\
        "ágúst"\
        "september"\
        "október"\
        "nóvember"\
>>>>>>> upstream/master
=======
        "október"\
        "nóvember"\
>>>>>>> upstream/master
        "desember"\
        ""]
    ::msgcat::mcset is DATE_FORMAT "%e.%m.%Y"
    ::msgcat::mcset is TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset is DATE_TIME_FORMAT "%e.%m.%Y %H:%M:%S %z"
}
