#ifndef _DF
#define _DF

#include "Filter.h"
#include "Color.h"

namespace imaging
{
	class DiffFilter : public Filter
	{

	public:
		virtual Image * apply( Image & src) ;
	
		DiffFilter() : 
		Filter("Color") {};
	};
}

#endif