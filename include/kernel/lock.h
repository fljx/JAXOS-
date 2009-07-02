#ifndef __LOCK_H
	#define __LOCK_H


/// Placeholder for thread synchronization mutex.
class Mutex
{
	// placeholder for code to create, use and free a mutex.
};


///	Thread synchronization lock.
class Lock
{
public:
	/// Default constructor.
	Lock( Mutex &mutex );
	/// Default destructor.
	virtual ~Lock( );
protected:
private:
	Mutex &mutex; //!< Member variable "&mutex"
};


#endif // __LOCK_H
