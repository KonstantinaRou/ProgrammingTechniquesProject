#ifndef _MF
#define _MF

#include "Filter.h"


namespace imaging
{
	class MedianFilter : public Filter
	{
	protected:
		const Color color ;

	public:
		Color getColor(){return color;}
		virtual Image * apply( Image & src) ;
	
		MedianFilter() : 
		Filter("Color") {};
	};
}

#endif