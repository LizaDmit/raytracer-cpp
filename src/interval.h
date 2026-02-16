#ifndef INTERVAL_H
#define INTERVAL_H
#include "rtweekend.h"

class interval {
    public:
        double min, max;

        interval(): min(infinity), max(-infinity) {}

        interval(double min, double max) : min(min), max(max) {}

        double size() const {
            return max - min;
        }

        bool contains(double x) const {
            return min < x && x < max;
        }

        bool surrounds(double x) const {
            return min < x && x < max;
        }

        static const interval empty, universe;     // There is only one empty and one universe objects 
                                                   // shared by all members of the class
};

const interval interval::empty = interval(infinity, -infinity);    // Guaranteed empty interval, no possible values
const interval interval::universe = interval(-infinity, infinity); // Everything interval, all possible values



#endif