# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset sr DAYS_OF_WEEK_ABBREV [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "\u041d\u0435\u0434"\
        "\u041f\u043e\u043d"\
        "\u0423\u0442\u043e"\
        "\u0421\u0440\u0435"\
        "\u0427\u0435\u0442"\
        "\u041f\u0435\u0442"\
        "\u0421\u0443\u0431"]
=======
        "Нед"\
        "Пон"\
        "Уто"\
        "Сре"\
        "Чет"\
        "Пет"\
        "Суб"]
>>>>>>> upstream/master
    ::msgcat::mcset sr DAYS_OF_WEEK_FULL [list \
        "Недеља"\
        "Понедељак"\
        "Уторак"\
        "Среда"\
        "Четвртак"\
        "Петак"\
        "Субота"]
    ::msgcat::mcset sr MONTHS_ABBREV [list \
        "Јан"\
        "Феб"\
        "Мар"\
        "Апр"\
        "Мај"\
        "Јун"\
        "Јул"\
        "Авг"\
        "Сеп"\
        "Окт"\
        "Нов"\
        "Дец"\
        ""]
    ::msgcat::mcset sr MONTHS_FULL [list \
        "Јануар"\
        "Фебруар"\
        "Март"\
        "Април"\
        "Мај"\
        "Јуни"\
        "Јули"\
        "Август"\
        "Септембар"\
        "Октобар"\
        "Новембар"\
        "Децембар"\
        ""]
<<<<<<< HEAD
    ::msgcat::mcset sr BCE "\u043f. \u043d. \u0435."
    ::msgcat::mcset sr CE "\u043d. \u0435"
=======
        "Нед"\
        "Пон"\
        "Уто"\
        "Сре"\
        "Чет"\
        "Пет"\
        "Суб"]
    ::msgcat::mcset sr DAYS_OF_WEEK_FULL [list \
        "Недеља"\
        "Понедељак"\
        "Уторак"\
        "Среда"\
        "Четвртак"\
        "Петак"\
        "Субота"]
    ::msgcat::mcset sr MONTHS_ABBREV [list \
        "Јан"\
        "Феб"\
        "Мар"\
        "Апр"\
        "Мај"\
        "Јун"\
        "Јул"\
        "Авг"\
        "Сеп"\
        "Окт"\
        "Нов"\
        "Дец"\
        ""]
    ::msgcat::mcset sr MONTHS_FULL [list \
        "Јануар"\
        "Фебруар"\
        "Март"\
        "Април"\
        "Мај"\
        "Јуни"\
        "Јули"\
        "Август"\
        "Септембар"\
        "Октобар"\
        "Новембар"\
        "Децембар"\
        ""]
    ::msgcat::mcset sr BCE "п. н. е."
    ::msgcat::mcset sr CE "н. е"
>>>>>>> upstream/master
=======
    ::msgcat::mcset sr BCE "п. н. е."
    ::msgcat::mcset sr CE "н. е"
>>>>>>> upstream/master
    ::msgcat::mcset sr DATE_FORMAT "%Y.%m.%e"
    ::msgcat::mcset sr TIME_FORMAT "%k.%M.%S"
    ::msgcat::mcset sr DATE_TIME_FORMAT "%Y.%m.%e %k.%M.%S %z"
}
