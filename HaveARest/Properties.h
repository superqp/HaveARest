/*
 *This header file mainly handle the properties in the conf file: haveARest.conf.
 */

#define PROP_DELIMITER ':'
#define PROP_VALUE_TRUE "TRUE"
#define PROP_VALUE_FALSE "FALSE"
#define WORKING_TIME_PROP "HAVEAREST.WORKING_TIME"
#define WORKING_TIME_DEF 30
#define REST_TIME_PROP "HAVEAREST.REST_TIME"
#define REST_TIME_DEF 3
#define START_AUTOMATICALLY_AT_INTERMITTENT_PROP "HAVEAREST.START_AUTOMATICALLY_AT_INTERMITTENT"
#define START_AUTOMATICALLY_AT_INTERMITTENT_DEF 1
#define ALERT_TEN_SECONDS_LEFT_PROP "HAVEAREST.ALERT_TEN_SECONDS_LEFT"
#define ALERT_TEN_SECONDS_LEFT_DEF 0
#define START_AND_MINIMIZE_MAIN_WINDOW_PROP "HAVEAREST.START_AND_MINIMIZE_MAIN_WINDOW"
#define START_AND_MINIMIZE_MAIN_WINDOW_DEF 0

// define the total property number. It needs to be adapted every time a property added or removed.
#define PROPERTY_AMOUNT 5

// define how many visiting bits will be set. It equals to property number.
#define DEFAULT_UNVISITED_BITS 0x0000000000011111

// define the unvisited bits respectively
#define WORKING_TIME_UNVISITED_BIT                                                  0x0000000000000001
#define REST_TIME_UNVISITED_BIT                                                           0x0000000000000010
#define START_AUTOMATICALLY_AT_INTERMITTENT_UNVISITED_BIT   0x0000000000000100
#define ALERT_TEN_SECONDS_LEFT_UNVISITED_BIT                               0x0000000000001000
#define START_AND_MINIMIZE_MAIN_WINDOW_UNVISITED_BIT          0x0000000000010000 

// define the number of each prop.
#define WORKING_TIME_NUMBER                                                  0
#define REST_TIME_NUMBER                                                           1
#define START_AUTOMATICALLY_AT_INTERMITTENT_NUMBER    2
#define ALERT_TEN_SECONDS_LEFT_NUMBER                                3
#define START_AND_MINIMIZE_MAIN_WINDOW_NUMBER          4


