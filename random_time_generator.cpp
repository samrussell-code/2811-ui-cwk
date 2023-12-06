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
        std::srand((previousDate.tm_yday + previousDate.tm_year)*746392013); //abritrary multiplier to eliminate pseudorandom patterns
        seeded = true;
    }
}

std::tm RandomTimeGenerator::getPreviousDateGMT() {
    std::time_t currentTime = std::time(nullptr);
    std::tm* previousDateGMT = std::gmtime(&currentTime);
    previousDateGMT->tm_hour = 0; // remove the time variables so it remains static all day
    previousDateGMT->tm_min = 0;
    previousDateGMT->tm_sec = 0;

    // subtract one day
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
    savedRandomTime = std::rand() % (24 * 60);
    savedRandomTime += userTimezoneDifference;
    savedRandomTime = (savedRandomTime + (24 * 60)) % (24 * 60);
    return savedRandomTime;
}


QString RandomTimeGenerator::formatTime(int hours, int minutes) const {
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

QString RandomTimeGenerator::getSavedTime() const {
    int hours = savedRandomTime / 60;
    int minutes = savedRandomTime % 60;
    return formatTime(hours, minutes);
}

QString RandomTimeGenerator::getSavedTimeDifference() const {
    std::time_t currentTime = std::time(nullptr);
    std::tm currentTimeStruct;
    if (std::tm* tempCurrentTimeStruct = std::gmtime(&currentTime)) {
        currentTimeStruct = *tempCurrentTimeStruct;
    } else {
        // Handle the case where std::gmtime returns nullptr (error)
        // You can add appropriate error handling here
        // For example, log an error message and set some default values
        currentTimeStruct = {}; // Set to a default value
    }

    // Cast savedRandomTime to time_t before passing it to std::localtime
    std::time_t savedRandomTimeAsTimeT = static_cast<std::time_t>(savedRandomTime);
    std::tm savedTimeStruct;
    if (std::tm* tempSavedTimeStruct = std::localtime(&savedRandomTimeAsTimeT)) {
        savedTimeStruct = *tempSavedTimeStruct;
    } else {
        // Handle the case where std::localtime returns nullptr (error)
        // You can add appropriate error handling here
        // For example, log an error message and set some default values
        savedTimeStruct = {}; // Set to a default value
    }

    // Calculate the time difference in minutes
    int timeDifference = (currentTimeStruct.tm_hour - savedTimeStruct.tm_hour) * 60 + (currentTimeStruct.tm_min - savedTimeStruct.tm_min);

    // Ensure the difference is positive
    timeDifference = (timeDifference + (24 * 60)) % (24 * 60);

    int hours = timeDifference / 60;
    int minutes = timeDifference % 60;

    return formatTime(hours, minutes);
}
