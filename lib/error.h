#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <sstream>
#include <string>
using std::cout;
using std::ostream;
using std::string;

#define INTTOSTR( x ) ( dynamic_cast< std::ostringstream & > \
    ( ( std::ostringstream() << std::dec << x) ).str() )

#define debug_info(x) \
            (x << __FILE__ << ":" << INTTOSTR(__LINE__) << " " << LogLevelStr[INFO] << " -> ")

enum LogLevel {
    FATAL = 0,
    ERROR,
    WARNING,
    INFO,
    DEBUG,
    TRACE
};

const char* LogLevelStr [] = {
    "FATA",
    "ERRO",
    "WARN",
    "INFO",
    "DBUG",
    "TRCE",
};

class LogStream {
public:
    LogStream(ostream& os=std::cout, LogLevel lvl=INFO):
        m_out(os) {}
    template<typename T>
    LogStream& operator<< (const T& v);
    LogStream& operator<< (ostream& (*F) (ostream&));
    void SetLevel(const LogLevel lvl) { m_level = lvl; }
    LogLevel GetLevel() { return m_level; }

private:
    LogLevel m_level;
    ostream& m_out;
    string m_line;
};
#endif
