//
// Created by mark on 16-12-5.
//

#include <blackpanther/base/Date.h>
#include <assert.h>
#include <stdio.h>

using namespace blackpanther;
const int kMonthsOfYear = 12;
int isLeapYear(int year){
    if(year%400 == 0)
        return 1;
    else if(year%100 ==0)
        return 0;
    else if(year%4 == 0)
        return 1;
    else
        return 0;
}
int dayOfMonth(int year, int month){
    static int arr[2][kMonthsOfYear+1] = {
        {0,31,28,31,30,31,30,31,31,30,31,30,31},
        {0,31,29,31,30,31,30,31,31,30,31,30,31}
    };
    return arr[isLeapYear(year)][month];
}
void passByConstReference(const Date &x){
    printf("%s\n",x.toIsoString().c_str());
}
void passByValue(Date x){
    printf("%s\n",x.toIsoString().c_str());
}

int main(void){
    time_t now = time(nullptr);
    struct tm t1 = *gmtime(&now);
    struct tm t2 = *localtime(&now);
    Date someDay(1994, 10, 28);
    printf("%s\n", someDay.toIsoString().c_str());
    passByValue(someDay);
    passByConstReference(someDay);
    Date todayUtc(t1);
    printf("%s\n", todayUtc.toIsoString().c_str());
    Date todayLocal(t2);
    printf("%s\n", todayLocal.toIsoString().c_str());

    int julianDayNumber = 2415021;
    int weekday = 1;

    for(int year = 1900; year < 2500; ++year){
        assert(Date(year, 3, 1).julianDayNumber() - Date(year, 2, 29).julianDayNumber() == isLeapYear(year));
        for(int month = 1; month <= kMonthsOfYear; ++month){
            for(int day = 1; day <= dayOfMonth(year, month); ++day){
                Date d(year, month, day);
                assert(year == d.year());
                assert(month == d.month());
                assert(day == d.day());
                assert(weekday == d.weekDay());
                assert(julianDayNumber == d.julianDayNumber());

                Date d2(julianDayNumber);
                assert(year == d2.year());
                assert(month == d2.month());
                assert(day == d2.day());
                assert(weekday == d2.weekDay());
                assert(julianDayNumber == d2.julianDayNumber());

                ++julianDayNumber;
                weekday = (weekday+1)%7;
            }
        }
    }
    return 0;
}