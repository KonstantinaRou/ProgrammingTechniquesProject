#ifndef _GREYF
#define _GREYF

#include "Filter.h"

namespace imaging
{
	class GreyFilter : public Filter
	{
		public:
			virtual Image * apply(Image & src);
			GreyFilter() : 
				Filter("grey"){};
	};
}

#endif