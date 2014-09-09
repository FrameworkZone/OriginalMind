/****************************************************************************
  Copyright (c) 2013-2014 libo.
 
  losemymind.libo@gmail.com

****************************************************************************/

#ifndef Foundation_Runnable_h
#define Foundation_Runnable_h

namespace Foundation {

/** 
 * The Runnable interface with the run() method
 * must be implemented by classes that provide
 * an entry point for a thread.
 */
class Runnable
{
public:	
	Runnable(){};
	virtual ~Runnable(){};

	/** 
	 * Do whatever the thread needs to do. Must
	 * be overridden by subclasses.
	 */
	virtual void run() = 0;
};

} // namespace Foundation
#endif // Foundation_Runnable_h