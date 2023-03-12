#include <iostream>	// iostream
#include <fstream>	// ofstream
#include <string>

#include <boost/json/value.hpp>

#include <handyman/json.h>

#include "MonitorSettings.h"


MonitorSettings MonitorSettings::default_value()
{
	MonitorSettings r;
	r.Port = 9009u;
	return r;
}

void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, MonitorSettings const& c)
{
	jv = {
		{ "port" , c.Port },
	};
}

MonitorSettings tag_invoke(boost::json::value_to_tag<MonitorSettings>, boost::json::value const& jv)
{
	MonitorSettings c = MonitorSettings::default_value();
	boost::json::object const& obj = jv.as_object();
	handyman::json::extract_if_contains(obj, c.Port, "port");
	return c;
}
