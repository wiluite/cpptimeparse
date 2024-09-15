#include "timeparse.h"
#include <cassert>

int main() {

    //GOOD CASES
    assert(time_parser().parse("32m"));
    assert(time_parser().parse("2.1234s"));
    assert(time_parser().parse("2h3m"));
    assert(time_parser().parse("3d2h32m"));
    assert(time_parser().parse("1w3d2h32m"));
    assert(time_parser().parse("1w 3d 2h 32m"));
    assert(time_parser().parse("1 w 3 d 2 h 32 m"));
    assert(time_parser().parse("4:16"));
    assert(time_parser().parse("04:16"));
    assert(time_parser().parse("4:13:02"));
    assert(time_parser().parse("04:13:02"));
    assert(time_parser().parse("4:13:02.266"));
    assert(time_parser().parse("2 days, 4:13:02"));
    assert(time_parser().parse("2 days,4:13:02.266"));
    assert(time_parser().parse("5hr34m56s"));
    assert(time_parser().parse("5 hours 34 minutes 56 seconds"));
    assert(time_parser().parse("5 minute"));
    assert(time_parser().parse("5 hrs, 34 mins, 56 secs"));
    assert(time_parser().parse("2 days, 5 hours, 34 minutes, 56 seconds"));
    assert(time_parser().parse("1.2 m"));
    assert(time_parser().parse("1.2 min"));
    assert(time_parser().parse("1.2 mins"));
    assert(time_parser().parse("1.2 minute"));
    assert(time_parser().parse("1.2 minutes"));
    assert(time_parser().parse("172 hours"));
    assert(time_parser().parse("172 hr"));
    assert(time_parser().parse("172 h"));
    assert(time_parser().parse("172 hrs"));
    assert(time_parser().parse("172 hour"));
    assert(time_parser().parse("1.24 days"));
    assert(time_parser().parse("5 d"));
    assert(time_parser().parse("5 day"));
    assert(time_parser().parse("5 days"));
    assert(time_parser().parse("5.6 wk"));
    assert(time_parser().parse("5.6 week"));
    assert(time_parser().parse("5.6 weeks"));
    assert(time_parser().parse("5hr ,34m"));
    assert(time_parser().parse("5hr, 34m"));
    assert(time_parser().parse("5hr,34m"));
    assert(time_parser().parse("02:01:13:02.166"));
    assert(time_parser().parse("2:01:13:02.166"));
    assert(time_parser().parse("2:01:13:02.166"));
    assert(time_parser().parse("1:13:02.166"));
    assert(time_parser().parse("01:13:02.166"));
    assert(time_parser().parse("1d 06:60:99"));
    time_parser tp;
    tp.parse("1d 06:60:61");
    assert(tp.str() == "1 day, 7:01:01");

    assert(time_parser().parse("+02d 06:15:69.000333"));

    time_parser tp2;
    tp2.parse("+02d 06:15:69.000333");
    assert(tp2.str() == "2 days, 6:16:09.333");


    // BAD CASES
    assert(!time_parser().parse("02:01:13:022"));
    assert(!time_parser().parse("5hr,,34m"));
    assert(!time_parser().parse("5hr ,,34m"));
    assert(!time_parser().parse("5hr, ,34m"));
    assert(!time_parser().parse("5hr,34m,"));
    assert(!time_parser().parse("2:1:13:02.166"));
    assert(!time_parser().parse("01:3:02.166"));
    assert(!time_parser().parse("01:13:2.167"));
    assert(!time_parser().parse("4:13.:02.266"));
    assert(!time_parser().parse("4.:13:02.266"));
    assert(!time_parser().parse("4:13:02."));
    assert(!time_parser().parse("4:13:02.s"));
    assert(!time_parser().parse("4:13:02.1.1"));
    assert(!time_parser().parse("2:2:04:13:02.266"));
    assert(!time_parser().parse("1 w 3 d 2 h 32:32"));
    assert(!time_parser().parse("     1 "));
    assert(!time_parser().parse(""));
    assert(!time_parser().parse(" "));
    assert(!time_parser().parse("days"));
    assert(!time_parser().parse(":"));
    assert(!time_parser().parse("1.3"));
    assert(!time_parser().parse("1: 12"));
    assert(!time_parser().parse("1 :12"));
    assert(!time_parser().parse("1: 12.14"));
    assert(!time_parser().parse("1:12. 14"));
    assert(!time_parser().parse("1:11abc"));
    assert(!time_parser().parse("1:12:13b"));
    assert(!time_parser().parse("1.334 days, 12"));
    assert(!time_parser().parse("1w 5hr 34 n 18.5s"));
    assert(!time_parser().parse("1w5hr34m18.5"));
    assert(!time_parser().parse("4 w 2 days 01:04:13:02.266"));
    assert(!time_parser().parse("34m5hr56s"));
    assert(!time_parser().parse("1d 06:60:100"));
    assert(!time_parser().parse("+02 06:15:69.000333"));

    //BAD CASES WITH A LOCALE OTHER THAN C
    assert(!time_parser().parse("32м"));
    assert(!time_parser().parse("2.1234с"));
    assert(!time_parser().parse("2ч3м"));
    assert(!time_parser().parse("3д2ч32м"));
    assert(!time_parser().parse("1нед3д2ч32м"));
    assert(!time_parser().parse("1нед 3д 2ч 32м"));
    assert(!time_parser().parse("1 нед 3 д 2 ч 32 м"));
    assert(!time_parser().parse("2 дня, 4:13:02"));
    assert(!time_parser().parse("2 дня,4:13:02.266"));
    assert(!time_parser().parse("5часов34м56с"));
    assert(!time_parser().parse("5 часов 34 минуты 56 секунд"));
    assert(!time_parser().parse("5 минут"));
    assert(!time_parser().parse("5 час, 34 мин, 56 сек"));
    assert(!time_parser().parse("2 дня, 5 часов, 34 минуты, 56 секунд"));
    assert(!time_parser().parse("1.2 м"));
    assert(!time_parser().parse("1.2 мин"));
    assert(!time_parser().parse("1.2 минуты"));
    assert(!time_parser().parse("172 часа"));
    assert(!time_parser().parse("172 час"));
    assert(!time_parser().parse("172 ч"));
    assert(!time_parser().parse("1.24 дня"));
    assert(!time_parser().parse("5 д"));
    assert(!time_parser().parse("5 дн"));
    assert(!time_parser().parse("5 дней"));
    assert(!time_parser().parse("5.6 н"));
    assert(!time_parser().parse("5.6 нед"));
    assert(!time_parser().parse("5.6 недель"));
    assert(!time_parser().parse("5ч ,34м"));
    assert(!time_parser().parse("5час, 34м"));
    assert(!time_parser().parse("5час,34м"));
}
