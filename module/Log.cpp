#include "Log.h"
#include <stdarg.h>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>
#include <string>

#if defined(__MINGW32__) || defined(__MINGW64__)
class Logcxx::Imp
{
public:
	Imp()
	{
	}
	~Imp()
	{
	}

	void Init(const char *prop = "log4cxx.properties")
	{
		return;
	}

	void Print(Logcxx::Level level, std::string &buff)
	{
		return;
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

void Logcxx::Init(const char *prop)
{
	_pImp->Init(prop);
}

void Logcxx::Print(const char *file, long line, const char *funtion, Level level, const char *format, ...)
{
	return;
}

#else
#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/helpers/exception.h>
class Logcxx::Imp
{
public:
	Imp()
	{
	}
	~Imp()
	{
	}

	void Init(const char *prop = "log4cxx.properties")
	{

		setlocale(LC_ALL, "chs");

		log4cxx::PropertyConfigurator::configureAndWatch(prop);

		log4cxx::LoggerPtr rootLogger = log4cxx::Logger::getRootLogger();
		assert(rootLogger);
	}
	void Print(Logcxx::Level level, std::string &buff)
	{
		log4cxx::LoggerPtr rootLogger(log4cxx::Logger::getRootLogger());

		switch (static_cast<int>(level))
		{
		case static_cast<int>(Logcxx::Level::ERRORS):
			rootLogger->error(buff);
			break;
		case static_cast<int>(Logcxx::Level::WARN):
			rootLogger->warn(buff);
			break;
		case static_cast<int>(Logcxx::Level::DEBUG):
			rootLogger->debug(buff);
			break;
		case static_cast<int>(Logcxx::Level::INFO):
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

void Logcxx::Init(const char *prop)
{
	_pImp->Init(prop);
}

void Logcxx::Print(const char *file, long line, const char *funtion, Level level, const char *format, ...)
{
#ifdef _DEBUG
	return;
#endif

	char buff[4096];
	va_list ap;
	va_start(ap, format);
	vsnprintf(buff, 4096, format, ap);
	va_end(ap);

	std::string strlog = (boost::format(" %s %s %s : %s") % file % line % funtion % buff).str();

	_pImp->Print(level, strlog);
}
#endif
