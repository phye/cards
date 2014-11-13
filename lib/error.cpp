#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/time.h>
#include <time.h>
#include <string>
#include "error.h"

template<typename T>
LogStream& LogStream::operator<< (const T& v) 
{
    if (! m_line.size()) {
        struct timeval tv;  //Timeval

        gettimeofday(&tv, NULL);
        m_line = "[";

        m_line += __DATE__;
        m_line += " ";
        m_line += __TIME__; 
        m_line += "." + INTTOSTR(tv.tv_usec/1000) + "] ";
        m_line += "Pid(" + INTTOSTR(getpid()) + ") ";
        m_out << m_line;
    }
    m_out << v; 
    return (*this);
}

LogStream& LogStream::operator<< (ostream& (*F) (ostream&))
{
    F(m_out);
    m_line.clear();
    return (*this);
}

int main(void)
{
    LogStream log;
    //log << __FILE__ << ":" << INTTOSTR(__LINE__) << " " "Hello" << std::endl; 
    debug_info(log) << "How do you do" << std::endl;
    debug_info(log) << "Fine" << std::endl;
}
