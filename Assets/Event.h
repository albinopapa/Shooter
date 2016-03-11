#pragma once
#include "StandardLibraryIncludes.h"

template <typename V1, typename V2>
bool Equal(const V1 v1, const V2 v2)
{
	return v1 == v2;
}
template <typename V1, typename V2>
bool Less(const V1 v1, const V2 v2)
{
	return v1 < v2;
}
template <typename V1, typename V2>
bool LessEqual(const V1 v1, const V2 v2)
{
	return v1 <= v2;
}
template <typename V1, typename V2>
bool Greater(const V1 v1, const V2 v2)
{
	return v1 > v2;
}
template <typename V1, typename V2>
bool GreaterEqual(const V1 v1, const V2 v2)
{
	return v1 >= v2;
}
template <typename V1, typename V2>
bool NotEqual(const V1 v1, const V2 v2)
{
	return v1 != v2;
}
class Events
{
public:
	struct Param
	{
		void* param;
	};
	template <typename V1, typename V2>
	struct Event
	{
		V1* v1;
		bool (*Op)(V1,V2);
		V2* v2;
		Param* param;
		void (*Do)(int, ...);
		bool Repeating;
		int NParams;
	};
	template <typename V1, typename V2>
	struct EventR
	{
		V1* v1;
		bool (*Op)(V1,V2);
		V2 v2;
		Param* param;
		void (*Do)(int, ...);
		bool Repeating;
		int NParams;
	};

	Events();
	~Events();
	void NewEvent(float* v1, bool (*Op)(float,float), float* v2, void (*Do)(int, ...), bool Repeating, int nparams, ...); //only send nparams once
	void NewEvent(int* v1, bool (*Op)(int,int), int* v2, void (*Do)(int, ...), bool Repeating, int nparams, ...);		  //only send nparams once
	void NewEvent(float* v1, bool (*Op)(float,float), float v2, void (*Do)(int, ...), bool Repeating, int nparams, ...);  //only send nparams once
	void NewEvent(int* v1, bool (*Op)(int,int), int v2, void (*Do)(int, ...), bool Repeating, int nparams, ...);          //only send nparams once
	void NewEvent(bool* v1, bool (*Op)(bool,bool), bool v2, void (*Do)(int, ...), bool Repeating, int nparams, ...);	  //only send nparams once
	void RunEvents();
	bool NoEventsLeft() const;
	void ClearEvents();
private:
	void RunDoInt( int );
	void RunDoFloat( int );
	void RunDoRInt( int );
	void RunDoRFloat( int );
	void RunDoRBool( int );
	
	std::vector<Event<float,float>> floatEvents;
	std::vector<Event<int,int>> intEvents;
	std::vector<EventR<float,float>> RfloatEvents;
	std::vector<EventR<int,int>> RintEvents;
	std::vector<EventR<bool,bool>> RboolEvents;

};