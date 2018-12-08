# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset pt DAYS_OF_WEEK_ABBREV [list \
        "Dom"\
        "Seg"\
        "Ter"\
        "Qua"\
        "Qui"\
        "Sex"\
<<<<<<< HEAD
<<<<<<< HEAD
        "S\u00e1b"]
=======
        "Sáb"]
>>>>>>> upstream/master
    ::msgcat::mcset pt DAYS_OF_WEEK_FULL [list \
        "Domingo"\
        "Segunda-feira"\
        "Terça-feira"\
        "Quarta-feira"\
        "Quinta-feira"\
        "Sexta-feira"\
<<<<<<< HEAD
        "S\u00e1bado"]
=======
        "Sáb"]
    ::msgcat::mcset pt DAYS_OF_WEEK_FULL [list \
        "Domingo"\
        "Segunda-feira"\
        "Terça-feira"\
        "Quarta-feira"\
        "Quinta-feira"\
        "Sexta-feira"\
        "Sábado"]
>>>>>>> upstream/master
=======
        "Sábado"]
>>>>>>> upstream/master
    ::msgcat::mcset pt MONTHS_ABBREV [list \
        "Jan"\
        "Fev"\
        "Mar"\
        "Abr"\
        "Mai"\
        "Jun"\
        "Jul"\
        "Ago"\
        "Set"\
        "Out"\
        "Nov"\
        "Dez"\
        ""]
    ::msgcat::mcset pt MONTHS_FULL [list \
        "Janeiro"\
        "Fevereiro"\
<<<<<<< HEAD
<<<<<<< HEAD
        "Mar\u00e7o"\
=======
        "Março"\
>>>>>>> upstream/master
=======
        "Março"\
>>>>>>> upstream/master
        "Abril"\
        "Maio"\
        "Junho"\
        "Julho"\
        "Agosto"\
        "Setembro"\
        "Outubro"\
        "Novembro"\
        "Dezembro"\
        ""]
    ::msgcat::mcset pt DATE_FORMAT "%d-%m-%Y"
    ::msgcat::mcset pt TIME_FORMAT "%k:%M:%S"
    ::msgcat::mcset pt DATE_TIME_FORMAT "%d-%m-%Y %k:%M:%S %z"
}
