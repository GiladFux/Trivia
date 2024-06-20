#include "WSAInitializer.h"
#include <exception>


WSAInitializer::WSAInitializer()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

}

WSAInitializer::~WSAInitializer()
{
	// Q: why is this try necessary ?
	// A: to avoid throwing exceptions in d-tors !
	// if we do throw think what will happened in regular exception, our object 
	// will be destroyed because an exception occurred and then the d-tor will be 
	// called and if we throw now there is no one to handle the exception because 
	// we are already in the flow of exception handling !! (inception...)
	// please read more about exception handling and why it's forbidden to throw
	// exception from the destructor.
	try
	{
		WSACleanup();
	}
	catch (...) {}
}