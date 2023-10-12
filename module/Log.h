
#pragma once

#include <boost/serialization/singleton.hpp>

class Logcxx : public boost::serialization::singleton<Logcxx>
{
public:
	enum class Level
	{
		ERRORS,
		WARN,
		INFO,
		DEBUG
	};

	class Imp;

public:
	void Init(const char *prop);

	void Print(const char *file, long line, const char *funtion, Level level, const char *format, ...);

protected:
	Logcxx(void);
	~Logcxx(void);

private:
	Imp *_pImp;
};

#define _LOG(...) Logcxx::get_mutable_instance().Print(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
