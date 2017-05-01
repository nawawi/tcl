# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset pl DAYS_OF_WEEK_ABBREV [list \
        "N"\
        "Pn"\
        "Wt"\
<<<<<<< HEAD
        "\u015ar"\
=======
        "Śr"\
>>>>>>> upstream/master
        "Cz"\
        "Pt"\
        "So"]
    ::msgcat::mcset pl DAYS_OF_WEEK_FULL [list \
        "niedziela"\
<<<<<<< HEAD
        "poniedzia\u0142ek"\
        "wtorek"\
        "\u015broda"\
        "czwartek"\
        "pi\u0105tek"\
=======
        "poniedziałek"\
        "wtorek"\
        "środa"\
        "czwartek"\
        "piątek"\
>>>>>>> upstream/master
        "sobota"]
    ::msgcat::mcset pl MONTHS_ABBREV [list \
        "sty"\
        "lut"\
        "mar"\
        "kwi"\
        "maj"\
        "cze"\
        "lip"\
        "sie"\
        "wrz"\
<<<<<<< HEAD
        "pa\u017a"\
=======
        "paź"\
>>>>>>> upstream/master
        "lis"\
        "gru"\
        ""]
    ::msgcat::mcset pl MONTHS_FULL [list \
<<<<<<< HEAD
        "stycze\u0144"\
        "luty"\
        "marzec"\
        "kwiecie\u0144"\
        "maj"\
        "czerwiec"\
        "lipiec"\
        "sierpie\u0144"\
        "wrzesie\u0144"\
        "pa\u017adziernik"\
        "listopad"\
        "grudzie\u0144"\
=======
        "styczeń"\
        "luty"\
        "marzec"\
        "kwiecień"\
        "maj"\
        "czerwiec"\
        "lipiec"\
        "sierpień"\
        "wrzesień"\
        "październik"\
        "listopad"\
        "grudzień"\
>>>>>>> upstream/master
        ""]
    ::msgcat::mcset pl BCE "p.n.e."
    ::msgcat::mcset pl CE "n.e."
    ::msgcat::mcset pl DATE_FORMAT "%Y-%m-%d"
    ::msgcat::mcset pl TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset pl DATE_TIME_FORMAT "%Y-%m-%d %H:%M:%S %z"
}
