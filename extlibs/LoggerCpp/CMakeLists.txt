add_library (LoggerCpp STATIC
 include/LoggerCpp/Channel.h
 include/LoggerCpp/Config.h
 include/LoggerCpp/DateTime.h
 include/LoggerCpp/Exception.h
 include/LoggerCpp/Formatter.h
 include/LoggerCpp/Log.h
 include/LoggerCpp/Logger.h
 include/LoggerCpp/LoggerCpp.h
 include/LoggerCpp/Manager.h
 include/LoggerCpp/Output.h
 include/LoggerCpp/OutputConsole.h
 include/LoggerCpp/OutputDebug.h
 include/LoggerCpp/OutputFile.h
 include/LoggerCpp/OutputSyslog.h
 include/LoggerCpp/shared_ptr.hpp
 include/LoggerCpp/Utils.h
 src/Config.cpp
 src/DateTime.cpp
 src/Log.cpp
 src/Logger.cpp
 src/Manager.cpp
 src/OutputConsole.cpp
 src/OutputDebug.cpp
 src/OutputFile.cpp
 src/OutputSyslog.cpp
)

