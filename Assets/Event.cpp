#include "Event.h"

Events::Events()
{}

Events::~Events()
{
	ClearEvents();
}

void Events::NewEvent(float* v1, bool (*Op)(float,float), float* v2, void (*Do)(int, ...), bool Repeating, int nparams, ...)
{
	Event< float, float > fev;
	floatEvents.push_back( fev );
	floatEvents[ floatEvents.size() - 1 ].NParams = nparams;
	floatEvents[ floatEvents.size() - 1 ].param = new Param[ floatEvents[ floatEvents.size() - 1 ].NParams ];
	va_list list;
	va_start( list, nparams );
	for( int i = 0; i < floatEvents[ floatEvents.size() - 1 ].NParams; i++ )
	{
		floatEvents[ floatEvents.size() - 1 ].param[i].param = va_arg( list, void* );
	}
	va_end( list );
		
	floatEvents[ floatEvents.size() - 1 ].Do = Do;
	floatEvents[ floatEvents.size() - 1 ].Op = Op;
	floatEvents[ floatEvents.size() - 1 ].v1 = v1;
	floatEvents[ floatEvents.size() - 1 ].v2 = v2;
	floatEvents[ floatEvents.size() - 1 ].Repeating = Repeating;
}

void Events::NewEvent(int* v1, bool (*Op)(int,int), int* v2, void (*Do)(int, ...), bool Repeating, int nparams, ...)
{
	Event< int,int > iev;
	intEvents.push_back( iev );
	intEvents[ intEvents.size() - 1 ].NParams = nparams;
	intEvents[ intEvents.size() - 1 ].param = new Param[ intEvents[ intEvents.size() - 1 ].NParams ];
	va_list list;
	va_start( list, nparams );
	for( int i = 0; i < intEvents[ intEvents.size() - 1 ].NParams; i++ )
	{
		intEvents[ intEvents.size() - 1 ].param[i].param = va_arg( list, void* );
	}
	va_end( list );
		
	intEvents[ intEvents.size() - 1 ].Do = Do;
	intEvents[ intEvents.size() - 1 ].Op = Op;
	intEvents[ intEvents.size() - 1 ].v1 = v1;
	intEvents[ intEvents.size() - 1 ].v2 = v2;
	intEvents[ intEvents.size() - 1 ].Repeating = Repeating;
}

void Events::NewEvent(float* v1, bool (*Op)(float,float), float v2, void (*Do)(int, ...), bool Repeating, int nparams, ...)
{
	EventR< float,float > rfev;
	RfloatEvents.push_back( rfev );
	RfloatEvents[ RfloatEvents.size() - 1 ].NParams = nparams;
	RfloatEvents[ RfloatEvents.size() - 1 ].param = new Param[ RfloatEvents[ RfloatEvents.size() - 1 ].NParams ];
	va_list list;
	va_start( list, nparams );
	for( int i = 0; i < RfloatEvents[ RfloatEvents.size() - 1 ].NParams; i++ )
	{
		RfloatEvents[ RfloatEvents.size() - 1 ].param[i].param = va_arg( list, void* );
	}
	va_end( list );
		
	RfloatEvents[ RfloatEvents.size() - 1 ].Do = Do;
	RfloatEvents[ RfloatEvents.size() - 1 ].Op = Op;
	RfloatEvents[ RfloatEvents.size() - 1 ].v1 = v1;
	RfloatEvents[ RfloatEvents.size() - 1 ].v2 = v2;
	RfloatEvents[ RfloatEvents.size() - 1 ].Repeating = Repeating;
}

void Events::NewEvent(int* v1, bool (*Op)(int,int), int v2, void (*Do)(int, ...), bool Repeating, int nparams, ...)
{
	EventR< int,int > riev;
	RintEvents.push_back( riev );
	RintEvents[ RintEvents.size() - 1 ].NParams = nparams;
	RintEvents[ RintEvents.size() - 1 ].param = new Param[ RintEvents[ RintEvents.size() - 1 ].NParams ];
	va_list list;
	va_start( list, nparams );
	for( int i = 0; i < RintEvents[ RintEvents.size() - 1 ].NParams; i++ )
	{
		RintEvents[ RintEvents.size() - 1 ].param[i].param = va_arg( list, void* );
	}
	va_end( list );
		
	RintEvents[ RintEvents.size() - 1 ].Do = Do;
	RintEvents[ RintEvents.size() - 1 ].Op = Op;
	RintEvents[ RintEvents.size() - 1 ].v1 = v1;
	RintEvents[ RintEvents.size() - 1 ].v2 = v2;
	RintEvents[ RintEvents.size() - 1 ].Repeating = Repeating;
}

void Events::NewEvent(bool* v1, bool (*Op)(bool,bool), bool v2, void (*Do)(int, ...), bool Repeating, int nparams, ...)
{
	EventR< bool,bool > rbev;
	RboolEvents.push_back( rbev );
	RboolEvents[ RboolEvents.size() - 1 ].NParams = nparams;
	RboolEvents[ RboolEvents.size() - 1 ].param = new Param[ RboolEvents[ RboolEvents.size() - 1 ].NParams ];
	va_list list;
	va_start( list, nparams );
	for( int i = 0; i < RboolEvents[ RboolEvents.size() - 1 ].NParams; i++ )
	{
		RboolEvents[ RboolEvents.size() - 1 ].param[i].param = va_arg( list, void* );
	}
	va_end( list );
		
	RboolEvents[ RboolEvents.size() - 1 ].Do = Do;
	RboolEvents[ RboolEvents.size() - 1 ].Op = Op;
	RboolEvents[ RboolEvents.size() - 1 ].v1 = v1;
	RboolEvents[ RboolEvents.size() - 1 ].v2 = v2;
	RboolEvents[ RboolEvents.size() - 1 ].Repeating = Repeating;
}

void Events::RunDoInt( int i )
{
	switch( intEvents[i].NParams )
	{
		case 0:
			{
				intEvents[i].Do(0);
			} break;
		case 1:
			{
				intEvents[i].Do( 1, intEvents[i].param[0].param);
			} break;
		case 2:
			{
				intEvents[i].Do( 2, intEvents[i].param[0].param, intEvents[i].param[1].param);
			} break;
		case 3:
			{
				intEvents[i].Do( 3, intEvents[i].param[0].param, intEvents[i].param[1].param, intEvents[i].param[2].param);
			} break;
		case 4:
			{
				intEvents[i].Do( 4, intEvents[i].param[0].param, intEvents[i].param[1].param, intEvents[i].param[2].param, intEvents[i].param[3].param);
			} break;
	}
}

void Events::RunDoFloat( int i )
{
	switch( floatEvents[i].NParams )
	{
		case 0:
			{
				floatEvents[i].Do(0);
			} break;
		case 1:
			{
				floatEvents[i].Do( 1, floatEvents[i].param[0].param);
			} break;
		case 2:
			{
				floatEvents[i].Do( 2, floatEvents[i].param[0].param, floatEvents[i].param[1].param);
			} break;
		case 3:
			{
				floatEvents[i].Do( 3, floatEvents[i].param[0].param, floatEvents[i].param[1].param, floatEvents[i].param[2].param);
			} break;
		case 4:
			{
				floatEvents[i].Do( 4, floatEvents[i].param[0].param, floatEvents[i].param[1].param, floatEvents[i].param[2].param, floatEvents[i].param[3].param);
			} break;
	}
}

void Events::RunDoRInt( int i )
{
	switch( RintEvents[i].NParams )
	{
		case 0:
			{
				RintEvents[i].Do(0);
			} break;
		case 1:
			{
				RintEvents[i].Do( 1, RintEvents[i].param[0].param);
			} break;
		case 2:
			{
				RintEvents[i].Do( 2, RintEvents[i].param[0].param, RintEvents[i].param[1].param);
			} break;
		case 3:
			{
				RintEvents[i].Do( 3, RintEvents[i].param[0].param, RintEvents[i].param[1].param, RintEvents[i].param[2].param);
			} break;
		case 4:
			{
				RintEvents[i].Do( 4, RintEvents[i].param[0].param, RintEvents[i].param[1].param, RintEvents[i].param[2].param, RintEvents[i].param[3].param);
			} break;
	}
}

void Events::RunDoRFloat( int i )
{
	switch( RfloatEvents[i].NParams )
	{
		case 0:
			{
				RfloatEvents[i].Do(0);
			} break;
		case 1:
			{
				RfloatEvents[i].Do( 1, RfloatEvents[i].param[0].param);
			} break;
		case 2:
			{
				RfloatEvents[i].Do( 2, RfloatEvents[i].param[0].param, RfloatEvents[i].param[1].param);
			} break;
		case 3:
			{
				RfloatEvents[i].Do( 3, RfloatEvents[i].param[0].param, RfloatEvents[i].param[1].param, RfloatEvents[i].param[2].param);
			} break;
		case 4:
			{
				RfloatEvents[i].Do( 4, RfloatEvents[i].param[0].param, RfloatEvents[i].param[1].param, RfloatEvents[i].param[2].param, RfloatEvents[i].param[3].param);
			} break;
	}
}

void Events::RunDoRBool( int i )
{
	switch( RboolEvents[i].NParams )
	{
		case 0:
			{
				RboolEvents[i].Do(0);
			} break;
		case 1:
			{
				RboolEvents[i].Do( 1, RboolEvents[i].param[0].param);
			} break;
		case 2:
			{
				RboolEvents[i].Do( 2, RboolEvents[i].param[0].param, RboolEvents[i].param[1].param);
			} break;
		case 3:
			{
				RboolEvents[i].Do( 3, RboolEvents[i].param[0].param, RboolEvents[i].param[1].param, RboolEvents[i].param[2].param);
			} break;
		case 4:
			{
				RboolEvents[i].Do( 4, RboolEvents[i].param[0].param, RboolEvents[i].param[1].param, RboolEvents[i].param[2].param, RboolEvents[i].param[3].param);
			} break;
	}
}

void Events::RunEvents()
{
	int i;
	/*for(std::vector<Event<float,float>>::iterator it = floatEvents.begin(); it != floatEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, *it->v2 ) )
		{
			RunDo( i );
			if( !it->Repeating )
			{
				it = floatEvents.erase( it );
				it = floatEvents.erase( it );
				if( it == floatEvents.end() ) break;
			}
		}
	}*/
	
	i = 0;
	for(std::vector<Event<int,int>>::iterator it = intEvents.begin(); it < intEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, *it->v2 ) )
		{
			RunDoInt( i );
			if( !it->Repeating )
			{
				delete [] it->param;
				it = intEvents.erase( it );
				if( it == intEvents.end() ) break;
			}
		}
	}
	i = 0;
	for(std::vector<Event<float,float>>::iterator it = floatEvents.begin(); it < floatEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, *it->v2 ) )
		{
			RunDoFloat( i );
			if( !it->Repeating )
			{
				delete [] it->param;
				it = floatEvents.erase( it );
				if( it == floatEvents.end() ) break;
			}
		}
	}
	i = 0;
	for(std::vector<EventR<float,float>>::iterator it = RfloatEvents.begin(); it < RfloatEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, it->v2 ) )
		{
			RunDoFloat( i );
			if( !it->Repeating )
			{
				delete [] it->param;
				it = RfloatEvents.erase( it );
				if( it == RfloatEvents.end() ) break;
			}
		}
	}
	i = 0;
	for(std::vector<EventR<int,int>>::iterator it = RintEvents.begin(); it < RintEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, it->v2 ) )
		{
			RunDoFloat( i );
			if( !it->Repeating )
			{
				delete [] it->param;
				it = RintEvents.erase( it );
				if( it == RintEvents.end() ) break;
			}
		}
	}

	i = 0;
	for(std::vector<EventR<bool,bool>>::iterator it = RboolEvents.begin(); it < RboolEvents.end(); it++, i++)
	{
		if( it->Op( *it->v1, it->v2 ) )
		{
			RunDoFloat( i );
			if( !it->Repeating )
			{
				delete [] it->param;
				it = RboolEvents.erase( it );
				if( it == RboolEvents.end() ) break;
			}
		}
	}
}

bool Events::NoEventsLeft() const
{
	return floatEvents.begin() == floatEvents.end() && intEvents.begin() == intEvents.end() 
		&& RfloatEvents.begin() == RfloatEvents.end() && RintEvents.begin() == RintEvents.end()
		&& RboolEvents.begin() == RboolEvents.end();
}

void Events::ClearEvents()
{
	floatEvents.clear();
	RfloatEvents.clear();
	intEvents.clear();
	RintEvents.clear();
	RboolEvents.clear();
}