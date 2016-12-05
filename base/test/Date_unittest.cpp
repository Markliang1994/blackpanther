//
// Created by mark on 16-12-5.
//

#include <blackpanther/base/Date.h>

using namespace blackpanther;

int main(void){
    Date d(2016,2,3);
    printf("julianDayNumber of 2000/01/01 is :%d\n",d.julianDayNumber());
    d.toIsoString();
    return 0;
}