#pragma once

#include "StandardLibraryIncludes.h"

#include "D3DGraphics.h"
#include "InputController.h"
#include "Button.h"
#include "GameState.h"
#include "ScoreController.h"

class GameMenu : public GameState
{
public:
	GameMenu();
	GameMenu( StateController &sc, StateCore &core);
	virtual ~GameMenu();
	virtual void Do() = 0;
	virtual void Draw() = 0;
	void BeginFrame()override
	{
		gfx.BeginFrame(); 
	}
	void ComposeFrame()override;
	
protected:
	std::string filename;
	std::unique_ptr<Button *[]> mButtons;
	int nBtns;
};

class MainMenu : public GameMenu
{
public:
	MainMenu();
	MainMenu(  StateController &sc, StateCore &core, bool isPaused );
	~MainMenu();
	void Do()override;
	void Draw();

private:
	std::unique_ptr<Button> mScore;
	std::unique_ptr<Button> mStart;
	std::unique_ptr<Button> mQuit;
	std::unique_ptr<Button> mShop;
	std::unique_ptr<Button> mOptions;
};

class ScoreMenu : public GameMenu
{
public:
	ScoreMenu();
	ScoreMenu( StateController &sc, StateCore &core );
	~ScoreMenu();
	void Do()override;
	void Draw();
private:
	void LoadScoreList();
	std::unique_ptr<Button> mBack;
	std::vector<std::string> entries;
	ScoreController cScore;
};

class OptionsMenu : public GameMenu
{
public:
	OptionsMenu( StateController &sc, StateCore &core);
	~OptionsMenu();

	void LoadConfig();
	void SaveConfig();
	void Do()override;
	void Draw();
	void GetControlsOptions(std::string& controllerType, std::string *keyAssignments);
	void GetGraphicsOptions(unsigned int* scrnRes, bool &isFullScreen);
	void GetSoundOptions(float &musicVolume, float &sfxVolume);

private:
	enum key
	{
		UP, DOWN, LEFT, RIGHT, FIRE
	};
	std::string controller;
	std::string keyAssign[5];
	unsigned int resX, resY;
	bool fScreen;
	float mVolume, sfxVolume;
	std::string filename, header;
	std::unique_ptr<Button> mBack;

};
