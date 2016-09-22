#ifndef _BLF
#define _BLF

#include "Filter.h"


namespace imaging
{
	class BlurFilter : public Filter
	{
	protected:
		const Color color ;

	public:
		Color getColor(){return color;}
		virtual Image * apply( Image & src) ;
	
		BlurFilter() : 
		Filter("Color") {};
	};
}

#endif