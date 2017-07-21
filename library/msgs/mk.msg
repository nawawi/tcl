# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset mk DAYS_OF_WEEK_ABBREV [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "\u043d\u0435\u0434."\
        "\u043f\u043e\u043d."\
        "\u0432\u0442."\
        "\u0441\u0440\u0435."\
        "\u0447\u0435\u0442."\
        "\u043f\u0435\u0442."\
        "\u0441\u0430\u0431."]
=======
        "нед."\
        "пон."\
        "вт."\
        "сре."\
        "чет."\
        "пет."\
        "саб."]
>>>>>>> upstream/master
    ::msgcat::mcset mk DAYS_OF_WEEK_FULL [list \
        "недела"\
        "понеделник"\
        "вторник"\
        "среда"\
        "четврток"\
        "петок"\
        "сабота"]
    ::msgcat::mcset mk MONTHS_ABBREV [list \
        "јан."\
        "фев."\
        "мар."\
        "апр."\
        "мај."\
        "јун."\
        "јул."\
        "авг."\
        "септ."\
        "окт."\
        "ноем."\
        "декем."\
        ""]
    ::msgcat::mcset mk MONTHS_FULL [list \
        "јануари"\
        "февруари"\
        "март"\
        "април"\
        "мај"\
        "јуни"\
        "јули"\
        "август"\
        "септември"\
        "октомври"\
        "ноември"\
        "декември"\
        ""]
<<<<<<< HEAD
    ::msgcat::mcset mk BCE "\u043f\u0440.\u043d.\u0435."
    ::msgcat::mcset mk CE "\u0430\u0435."
=======
        "нед."\
        "пон."\
        "вт."\
        "сре."\
        "чет."\
        "пет."\
        "саб."]
    ::msgcat::mcset mk DAYS_OF_WEEK_FULL [list \
        "недела"\
        "понеделник"\
        "вторник"\
        "среда"\
        "четврток"\
        "петок"\
        "сабота"]
    ::msgcat::mcset mk MONTHS_ABBREV [list \
        "јан."\
        "фев."\
        "мар."\
        "апр."\
        "мај."\
        "јун."\
        "јул."\
        "авг."\
        "септ."\
        "окт."\
        "ноем."\
        "декем."\
        ""]
    ::msgcat::mcset mk MONTHS_FULL [list \
        "јануари"\
        "февруари"\
        "март"\
        "април"\
        "мај"\
        "јуни"\
        "јули"\
        "август"\
        "септември"\
        "октомври"\
        "ноември"\
        "декември"\
        ""]
    ::msgcat::mcset mk BCE "пр.н.е."
    ::msgcat::mcset mk CE "ае."
>>>>>>> upstream/master
=======
    ::msgcat::mcset mk BCE "пр.н.е."
    ::msgcat::mcset mk CE "ае."
>>>>>>> upstream/master
    ::msgcat::mcset mk DATE_FORMAT "%e.%m.%Y"
    ::msgcat::mcset mk TIME_FORMAT "%H:%M:%S %z"
    ::msgcat::mcset mk DATE_TIME_FORMAT "%e.%m.%Y %H:%M:%S %z %z"
}
