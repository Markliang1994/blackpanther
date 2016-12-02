//
// Created by mark on 16-12-1.
//

#ifndef BLACKPANTHER_BASE_TIMESTAMP_H
#define BLACKPANTHER_BASE_TIMESTAMP_H

namespace blackpanther{
    class Timestamp{
    public:
        static Timestamp now();
        static Timestamp invalid(){
            return Timestamp();
        }
    };
}
#endif //BLACKPANTHER_TIMESTAMP_H
