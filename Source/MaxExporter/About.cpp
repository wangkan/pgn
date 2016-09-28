#include <sstream>
#include "About.h"

About::About()
{
	version = 0;
	authorName = L"Wang Kan";
	copyright = L"Copyright";
	aboutCaption = L"About PGN Asset File Exporter for 3ds Max";

	std::wstringstream ss;
	ss << L"PGN File Exporter for 3ds Max\n"
		<< L"Version " << version << std::endl
		<< copyright;
	aboutText =	ss.str();
};

About about;
