# created by tools/loadICU.tcl -- do not edit
namespace eval ::tcl::clock {
    ::msgcat::mcset ja DAYS_OF_WEEK_ABBREV [list \
<<<<<<< HEAD
<<<<<<< HEAD
        "\u65e5"\
        "\u6708"\
        "\u706b"\
        "\u6c34"\
        "\u6728"\
        "\u91d1"\
        "\u571f"]
=======
        "日"\
        "月"\
        "火"\
        "水"\
        "木"\
        "金"\
        "土"]
>>>>>>> upstream/master
    ::msgcat::mcset ja DAYS_OF_WEEK_FULL [list \
        "日曜日"\
        "月曜日"\
        "火曜日"\
        "水曜日"\
        "木曜日"\
        "金曜日"\
        "土曜日"]
    ::msgcat::mcset ja MONTHS_FULL [list \
<<<<<<< HEAD
        "1\u6708"\
        "2\u6708"\
        "3\u6708"\
        "4\u6708"\
        "5\u6708"\
        "6\u6708"\
        "7\u6708"\
        "8\u6708"\
        "9\u6708"\
        "10\u6708"\
        "11\u6708"\
        "12\u6708"]
    ::msgcat::mcset ja BCE "\u7d00\u5143\u524d"
    ::msgcat::mcset ja CE "\u897f\u66a6"
    ::msgcat::mcset ja AM "\u5348\u524d"
    ::msgcat::mcset ja PM "\u5348\u5f8c"
=======
        "日"\
        "月"\
        "火"\
        "水"\
        "木"\
        "金"\
        "土"]
    ::msgcat::mcset ja DAYS_OF_WEEK_FULL [list \
        "日曜日"\
        "月曜日"\
        "火曜日"\
        "水曜日"\
        "木曜日"\
        "金曜日"\
        "土曜日"]
    ::msgcat::mcset ja MONTHS_FULL [list \
=======
>>>>>>> upstream/master
        "1月"\
        "2月"\
        "3月"\
        "4月"\
        "5月"\
        "6月"\
        "7月"\
        "8月"\
        "9月"\
        "10月"\
        "11月"\
        "12月"]
    ::msgcat::mcset ja BCE "紀元前"
    ::msgcat::mcset ja CE "西暦"
    ::msgcat::mcset ja AM "午前"
    ::msgcat::mcset ja PM "午後"
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
    ::msgcat::mcset ja DATE_FORMAT "%Y/%m/%d"
    ::msgcat::mcset ja TIME_FORMAT "%k:%M:%S"
    ::msgcat::mcset ja TIME_FORMAT_12 "%P %I:%M:%S"
    ::msgcat::mcset ja DATE_TIME_FORMAT "%Y/%m/%d %k:%M:%S %z"
<<<<<<< HEAD
<<<<<<< HEAD
    ::msgcat::mcset ja LOCALE_DATE_FORMAT "%EY\u5e74%m\u6708%d\u65e5"
    ::msgcat::mcset ja LOCALE_TIME_FORMAT "%H\u6642%M\u5206%S\u79d2"
    ::msgcat::mcset ja LOCALE_DATE_TIME_FORMAT "%EY\u5e74%m\u6708%d\u65e5 (%a) %H\u6642%M\u5206%S\u79d2 %z"
    ::msgcat::mcset ja LOCALE_ERAS "\u007b-9223372036854775808 \u897f\u66a6 0\u007d \u007b-3061011600 \u660e\u6cbb 1867\u007d \u007b-1812186000 \u5927\u6b63 1911\u007d \u007b-1357635600 \u662d\u548c 1925\u007d \u007b600220800 \u5e73\u6210 1988\u007d"
=======
=======
>>>>>>> upstream/master
    ::msgcat::mcset ja LOCALE_DATE_FORMAT "%EY年%m月%d日"
    ::msgcat::mcset ja LOCALE_TIME_FORMAT "%H時%M分%S秒"
    ::msgcat::mcset ja LOCALE_DATE_TIME_FORMAT "%EY年%m月%d日 (%a) %H時%M分%S秒 %z"
    ::msgcat::mcset ja LOCALE_ERAS "{-9223372036854775808 西暦 0} {-3061011600 明治 1867} {-1812186000 大正 1911} {-1357635600 昭和 1925} {600220800 平成 1988}"
<<<<<<< HEAD
>>>>>>> upstream/master
=======
>>>>>>> upstream/master
}
