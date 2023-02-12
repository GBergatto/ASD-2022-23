#include "tempo.h"

t_data DATEsetNull() {
    t_data d;
    d.year = 0;
    d.month = 0;
    d.day = 0;
    return d;
}

int DATEcmp(t_data a, t_data b) {
    if (a.year != b.year) return a.year - b.year;
    if (a.month != b.month) return a.month - b.month;
    if (a.day != b.day) return a.day - b.day;
    return 0;
}

t_orario TIMEsetNull() {
    t_orario o;
    o.hours = 0;
    o.minutes = 0;
    return o;
}

int TIMEcmp(t_orario a, t_orario b) {
    if (a.hours != b.hours) return a.hours - b.hours;
    if (a.minutes != b.minutes) return a.minutes - b.minutes;
    return 0;
}
