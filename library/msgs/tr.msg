# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset tr DAYS_OF_WEEK_ABBREV [list \
        "Paz"\
        "Pzt"\
        "Sal"\
<<<<<<< HEAD
        "\u00c7ar"\
=======
        "Çar"\
>>>>>>> upstream/master
        "Per"\
        "Cum"\
        "Cmt"]
    ::msgcat::mcset tr DAYS_OF_WEEK_FULL [list \
        "Pazar"\
        "Pazartesi"\
<<<<<<< HEAD
        "Sal\u0131"\
        "\u00c7ar\u015famba"\
        "Per\u015fembe"\
=======
        "Salı"\
        "Çarşamba"\
        "Perşembe"\
>>>>>>> upstream/master
        "Cuma"\
        "Cumartesi"]
    ::msgcat::mcset tr MONTHS_ABBREV [list \
        "Oca"\
<<<<<<< HEAD
        "\u015eub"\
=======
        "Şub"\
>>>>>>> upstream/master
        "Mar"\
        "Nis"\
        "May"\
        "Haz"\
        "Tem"\
<<<<<<< HEAD
        "A\u011fu"\
=======
        "Ağu"\
>>>>>>> upstream/master
        "Eyl"\
        "Eki"\
        "Kas"\
        "Ara"\
        ""]
    ::msgcat::mcset tr MONTHS_FULL [list \
        "Ocak"\
<<<<<<< HEAD
        "\u015eubat"\
        "Mart"\
        "Nisan"\
        "May\u0131s"\
        "Haziran"\
        "Temmuz"\
        "A\u011fustos"\
        "Eyl\u00fcl"\
        "Ekim"\
        "Kas\u0131m"\
        "Aral\u0131k"\
=======
        "Şubat"\
        "Mart"\
        "Nisan"\
        "Mayıs"\
        "Haziran"\
        "Temmuz"\
        "Ağustos"\
        "Eylül"\
        "Ekim"\
        "Kasım"\
        "Aralık"\
>>>>>>> upstream/master
        ""]
    ::msgcat::mcset tr DATE_FORMAT "%d.%m.%Y"
    ::msgcat::mcset tr TIME_FORMAT "%H:%M:%S"
    ::msgcat::mcset tr DATE_TIME_FORMAT "%d.%m.%Y %H:%M:%S %z"
}
