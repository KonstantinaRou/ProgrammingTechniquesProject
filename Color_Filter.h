#ifndef _CLF
#define _CLF

#include "Filter.h"
#include "Color.h"

namespace imaging
{
	class ColorFilter : public Filter
	{
	protected:
		const Color color ;

	public:
		Color getColor(){return color;}
		virtual Image * apply( Image & src) ;
	
		ColorFilter(const Color c) : 
		Filter("Color") , color(c) {};
	};
}

#endif