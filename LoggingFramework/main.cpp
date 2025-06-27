#include "Logger.hpp"
#include "ConsoleAppender.hpp"
#include "FileAppender.hpp"
#include <memory>

int main() {
    Logger logger("App"); 
    auto consoleAppender = std::make_shared<ConsoleAppender>(); 
    logger.addAppender(consoleAppender); 

    auto fileAppender = std::make_shared<FileAppender>("app.log"); 
    logger.addAppender(fileAppender); 

    logger.trace("This is a trace message"); 
    logger.debug("This is a debug message"); 
    logger.info("Applicaition Started"); 

    logger.warn("This is a warning message"); 
    logger.error("This is an error message"); 
    logger.fatal("Fata error: application shutting down"); 

    // changing min level 
    logger.setMinLevel(LogLevel::DEBUG); 
    logger.debug("Now debugged messages would be logged");


    return 0; 

}