// random_time_generator.cpp
#include "random_time_generator.h"
#include <iomanip>
#include <sstream>
#include <cstdlib>
#include <QDebug>

bool RandomTimeGenerator::seeded = false;

RandomTimeGenerator::RandomTimeGenerator() : userTimezoneDifference(0) {
    if (!seeded) { // seeding the time gen with the date and year of yesterday
        std::tm previousDate = getPreviousDateGMT();
        std::srand(previousDate.tm_yday + previousDate.tm_year);
        seeded = true;
    }
}

std::tm RandomTimeGenerator::getPreviousDateGMT() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* previousDateGMT = std::gmtime(&currentTime);
    previousDateGMT->tm_hour = 0; // remove the time variables so it remains static all day
    previousDateGMT->tm_min = 0;
    previousDateGMT->tm_sec = 0;

    // Subtract one day (24 hours)
    std::time_t previousTime = currentTime - 24 * 60 * 60;
    std::tm* previousDateGMTStruct = std::gmtime(&previousTime);
    previousDateGMTStruct->tm_hour = 0;
    previousDateGMTStruct->tm_min = 0;
    previousDateGMTStruct->tm_sec = 0;

    return *previousDateGMTStruct;
}

QString RandomTimeGenerator::getCurrentTimeGMT() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* currentTimeGMT = std::gmtime(&currentTime);
    return QString("%1:%2:%3").arg(currentTimeGMT->tm_hour, 2, 10, QChar('0')).arg(currentTimeGMT->tm_min, 2, 10, QChar('0')).arg(currentTimeGMT->tm_sec, 2, 10, QChar('0'));
}

int RandomTimeGenerator::generateRandomTime() {
    int randomTime = std::rand() % (24 * 60); // now we generate the time for the schedule!
    randomTime += userTimezoneDifference;
    randomTime = (randomTime + (24 * 60)) % (24 * 60);
    return randomTime;
}

QString RandomTimeGenerator::formatTime(int hours, int minutes) { //formatting for the schedule time label
    return QString("%1:%2:00").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0'));
}

QString RandomTimeGenerator::scheduleTime() { //main function to call when we want to generate a time.
    int randomTime = generateRandomTime();
    int hours = randomTime / 60;
    int minutes = randomTime % 60;
    QString formattedTime = formatTime(hours, minutes);

    qDebug() << "Generated Random Time: " << formattedTime;

    return formattedTime;
}
