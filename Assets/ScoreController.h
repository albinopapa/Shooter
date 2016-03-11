#pragma once
#include "StandardLibraryIncludes.h"

// ScoreController is used to
// Load the scores at game start
// add and keep track of the player score after each kill.
// Add entry to the list When player has lost all lives
// Sorts the list if player wants to quit
// Saves list to file after sorting
//
// Helper functions are used to get the Entries from the text file

class ScoreList 
{
public:
	ScoreList(const std::string &Name, const std::string &Date, unsigned int Score);
	
	void Save( FILE *pFile );	
	void AddEntry( unsigned int i, char* buf );
	std::string GetName() const;
	std::string GetDate() const;
	unsigned int GetScore() const;
	std::string GetEntry() const;

private:
	std::string Name, Date, Text;
	unsigned int Score;
};

class ScoreController
{
public:
	ScoreController();
	void Add( unsigned int val );
	void AddToList(const std::string &Name, const std::string &Date, unsigned int Score);
	std::vector< std::string > GetEntries() const;
	void SaveList();
	unsigned int GetPlayerScore() const;
private:
	std::string GetTime();
	void Load();
	void SortList();

private:
	std::vector<ScoreList> list;
	std::string fileName;
	unsigned int playerScore;
};

