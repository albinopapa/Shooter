#include "ScoreController.h"

ScoreController::ScoreController()
	:
	fileName("ScoreBoard.txt"),
	playerScore(0)
{}

void ScoreController::Add( unsigned int val )
{
	playerScore += val;
}

unsigned int ScoreController::GetPlayerScore() const
{
	return playerScore;
}

std::vector< std::string > ScoreController::GetEntries() const
{
	std::vector<std::string> txtList;
	for(unsigned int i = 0; i < list.size(); i++)
	{
		std::string temp = list[ i ].GetEntry();
		txtList.push_back( temp );
	}
	return txtList;
}

void ScoreController::AddToList(const std::string &Name, const std::string &Date, 
	unsigned int Score)
{
	list.push_back( ScoreList( Name, Date, Score) );
}

void ScoreController::SortList()
{
	for( unsigned int index = 0; index < list.size(); index++ )
	{
		for( unsigned int a = 0; a < list.size(); a++ )
		{
			if( list[ a ].GetScore() < list[ index ].GetScore() )
			{
				ScoreList buffer = list[ index ];
				list[ index ] = list[ a ];
				list[ a ] = buffer;
			}
		}
	}
	for( unsigned int index = 0; index < list.size(); index++ )
	{
		char buffer[ 75 ];
		list[ index ].AddEntry( index, buffer );
	}

}

void ScoreController::SaveList()
{
	SortList();
	FILE *pfile = fopen( "ScoreBoard.txt", "w" );
	for( unsigned int index = 0; index < list.size(); index++ )
	{
		list[ index ].Save(pfile);
	}
	fclose( pfile );
}

std::string ScoreController::GetTime()
{
	std::string time;
	time_t curTime = ::time( NULL );
	strftime( (char*)time.c_str(), 20, "%X/%x",localtime( &curTime ) );
	return time;
	// char buffer[ 20 ];
	//strftime( buffer, 20, "%X/%x",localtime( &curTime ) );
	//std::string time = buffer;
}

void ScoreController::Load()
{
	FILE *pfile;
	pfile = fopen( (const char*)fileName.c_str(), "r" );
	if( pfile )
	{
		fseek( pfile,2,SEEK_CUR );
		fgetc( pfile );
	}
	while( !feof( pfile ) )
	{
		unsigned int score;
		std::string date;
		std::string name;
		fscanf( pfile,"%s %d %s",name.c_str(),&score,date.c_str() );
		AddToList( name.c_str(), date.c_str(), score );
		fseek( pfile,4,SEEK_CUR );
		fgetc( pfile );
	}
	fclose( pfile );
}

// ScoreList class
ScoreList::ScoreList(const std::string &Name, const std::string &Date,
	unsigned int Score)
	:
	Name(Name),
	Date(Date),
	Score(Score)
{

}

void ScoreList::Save( FILE *pFile )
{
		fprintf( pFile,"%s", Text.c_str() );
}

void ScoreList::AddEntry( unsigned int i, char* buf )
{
	sprintf( buf,"%d.\t%s\t%d\t%s\n",i, Name.c_str(), Score, Date.c_str() );
	Text = buf;
}

std::string ScoreList::GetEntry() const
{
	return Text;
}

unsigned int ScoreList::GetScore() const
{
	return Score;
}
