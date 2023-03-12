#pragma once

#include <boost/json.hpp>

/// <summary>
/// Monitor settings.
/// </summary>
struct MonitorSettings {
public:
	/// <summary>
	/// Object with default values.
	/// </summary>
	/// <returns></returns>
	static MonitorSettings default_value();

	/// <summary>
	/// Service port.
	/// </summary>
	unsigned int	Port;
}; // Nmea2000MonitorSettings


/// <summary>
/// Helper for handyman::json::to_file, from_file_or_defaults.
/// </summary>
void tag_invoke(boost::json::value_from_tag, boost::json::value& jv, MonitorSettings const& c);

/// <summary>
/// Helper for handyman::json::to_file, from_file_or_defaults.
/// </summary>
MonitorSettings tag_invoke(boost::json::value_to_tag<MonitorSettings>, boost::json::value const& jv);
