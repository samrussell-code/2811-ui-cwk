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
    QString getSavedTime() const; // New method to get the saved time
    QString formatTime(int hours, int minutes) const; // Corrected declaration
    QString getSavedTimeDifference() const;
    QString getTimeUntilTomorrow() const;
    std::tm getNextDateGMT() const;

private:
    int generateRandomTime();

    std::tm getPreviousDateGMT();

    int userTimezoneDifference;

    // Seed the random number generator during initialization
    static bool seeded;

    int savedRandomTime; // New member variable to store the generated random time
};

#endif // RANDOM_TIME_GENERATOR_H
