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

//
//class Singleton
//{   public:
//        static Singleton *	GetInstance();
//        int a;
//
//    private:
//        Singleton( )	{	}
//        ~Singleton( )	{	}
//
//        static Mutex	mutex;
//
//        // Not defined, to prevent copying
//        Singleton( const Singleton& );
//        Singleton& operator =( const Singleton& other );
//};
//
//Mutex	Singleton::mutex;


#endif // __LOCK_H
