//------------------------------------------------------------
//
// C++ course assignment code 
//
// G. Papaioannou, 2014
//
//
//-------------------------------------------------------------

#ifndef _FILTER
#define _FILTER

#include "Image.h"
#include "Logger.h"

#define LOGGED_CLASS(a) class a : public Logged

namespace imaging
{
	LOGGED_CLASS ( Filter )
	{
		protected:
		const std::string name;
		public:
			Filter(std::string filter_name) :
				name(filter_name){};
			const std::string getName() {return name;}
			virtual Image * apply(Image & src) = 0;

	};
}
#endif