// random_time_generator.h
#ifndef RANDOM_TIME_GENERATOR_H
#define RANDOM_TIME_GENERATOR_H

#include <QString>
#include <ctime>

class RandomTimeGenerator {
public:
    RandomTimeGenerator();

    QString scheduleTime();
    QString getCurrentTimeGMT();

private:
    int generateRandomTime();
    QString formatTime(int hours, int minutes);

    std::tm getPreviousDateGMT();
    int userTimezoneDifference;

    // Seed the random number generator during initialization
    static bool seeded;
};

#endif // RANDOM_TIME_GENERATOR_H
