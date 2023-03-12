#define	WIN32_LEAN_AND_MEAN
#define	VC_EXTRALEAN

#include "shlobj_core.h"

#include <Windows.h>
#include "handyman.h"

namespace handyman {

std::string appdata_path()
{
	PWSTR	str;
	const HRESULT	r = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &str);


	std::string	r_str;
	r_str = str;

	return r_str;
}

}