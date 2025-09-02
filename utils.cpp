#include "utils.h"




long calculate_hour_from_seconds(long seconds){
    if(seconds <= 0){
        return 0;
    }
    return seconds / 3600;
}
long calculate_mins_from_seconds(long seconds){
    if(seconds <= 0){
        return 0;
    }
    return (seconds % 3600) / 60;
}

long calculate_secs_from_hour_min(long hour, long min){
    return (min * 60) + (hour * 3600);
}