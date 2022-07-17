#include "Log.h"
#include <stdarg.h>
#include <boost/serialization/singleton.hpp> 
#include <boost/format.hpp>
#include <log4cxx/logger.h>  
#include <log4cxx/basicconfigurator.h>  
#include <log4cxx/propertyconfigurator.h>  
#include <log4cxx/helpers/exception.h>  
#include <string>
 
class Logcxx::Imp
{
public:
	Imp()
	{
 
	}
	~Imp()
	{
 
	}
 
	void Init(const char* prop = "log4cxx.properties" )
	{
 
		setlocale(LC_ALL, "chs");
 
		log4cxx::PropertyConfigurator::configureAndWatch(prop);  
		
		log4cxx::LoggerPtr rootLogger = log4cxx::Logger::getRootLogger();
		assert(rootLogger);
	}
	void Print(Logcxx::Level level, std::string& buff)
	{
		log4cxx::LoggerPtr rootLogger(log4cxx::Logger::getRootLogger());
 
 
		switch(level)
		{
		case Logcxx::ERROR:
			rootLogger->error(buff);
			break;
		case Logcxx::WARN:
			rootLogger->warn(buff);
			break;
		case Logcxx::DEBUG:
			rootLogger->debug(buff);
			break;
		case Logcxx::INFO:
			rootLogger->info(buff);
			break;
		default:
			assert(false);
			rootLogger->info(buff);
			break;
		}
	}
};
 
Logcxx::Logcxx(void)
{
	_pImp = new Imp();
}
 
 
Logcxx::~Logcxx(void)
{
	if (_pImp)
	{
		delete _pImp;
	}
}
 
void Logcxx::Init( const char* prop )
{
	_pImp->Init(prop);
}
 
void Logcxx::Print( const char* file, long line, const char* funtion, Level level, const char* format, ... )
{
	char buff[4096];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buff, 4096, format, ap);
	va_end(ap);
 
	std::string strlog = (boost::format(" %s %s %s : %s") %file %line %funtion %buff).str();
 
	_pImp->Print(level, strlog);
}
