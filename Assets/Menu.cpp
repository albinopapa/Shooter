#include "Menu.h"
#include "Shop.h"
#include "Loading.h"
#include "Play.h"

GameMenu::GameMenu(  StateController &sc, StateCore &core)
	:
	GameState(sc, core)
{
}

GameMenu::~GameMenu()
{

}

void GameMenu::ComposeFrame( )
{
	Draw();
	Vec2 mPos = ic.GetMousePos();
	gfx.DrawCircle( mPos.x, mPos.y, 16, gfx.FILLCOLOR_XRGB(255, 255, 255));

}

// MainMenu/PauseMenu class
MainMenu::MainMenu( StateController &sc, StateCore &core, bool isPaused )
	:
	GameMenu( sc, core)
{
	unsigned int height = 50;
	unsigned int width = 100;
	D3DGraphics::Color c = gfx.FILLCOLOR_XRGB(255, 255, 255);

	if(!isPaused)
	{
		mStart = std::make_unique<Button>(c, 100, 100, width, height, "Start");
	}
	else
	{
		mStart = std::make_unique<Button>(c, 100, 100, width, height, "Resume");
	}
	mScore = std::make_unique<Button>(c, 100, 200, width, height, "Score");
	mQuit = std::make_unique<Button>(c, 100, 300, width, height, "Quit");
	//mShop = new Button(c, 100, 400, width, height, "Shop" );
	//mOptions = new Button(c, 100, 500, width, height, "Options");

	nBtns = 3;
	mButtons = std::make_unique<Button*[]>(nBtns);
	mButtons[0] = mStart.get();
	mButtons[1] = mScore.get();
	mButtons[2] = mQuit.get();
	if(core.prevState == StateCore::GState::NONE)
	{
		core.prevState = core.curState;
	}
	//mButtons[3] = mShop;
	//mButtons[4] = mOptions;
}

MainMenu::~MainMenu()
{
}

void MainMenu::Do()
{
	BeginFrame();
	const char* bText = mStart->GetText();
	if(mStart->ButtonClicked(ic, gfx))
	{
		if(strcmp(bText, "Start") == 0 )
		{
			core.curState = StateCore::GState::LOADING;
		}
		else
		{			
			core.curState = core.prevState;
			core.prevState = StateCore::GState::PAUSE;
		}
	}
	else if(mScore->ButtonClicked(ic, gfx))
	{
		core.prevState = core.curState;
		core.curState = StateCore::GState::SCOREMENU;
	}
	else if(mQuit->ButtonClicked(ic, gfx))
	{
		core.curState = StateCore::GState::QUIT;
	}
	ComposeFrame(  );

}

void MainMenu::Draw()
{
	for( int i = 0; i < nBtns; i++ )
	{
		mButtons[ i ]->Draw(f, gfx);
	}
}

// ScoreMenu class
ScoreMenu::ScoreMenu( StateController &sc, StateCore &core )
	:
	GameMenu( sc,core)
{
	unsigned int width = 50;
	unsigned int height = 100;
	D3DGraphics::Color c = gfx.FILLCOLOR_XRGB(255, 255, 255);
	mBack = std::make_unique<Button>(c, 600, 50, width, height, "Back");
	filename = "Scoreboard.txt";
	LoadScoreList();
}

void ScoreMenu::LoadScoreList()
{
	entries = cScore.GetEntries();
}

void ScoreMenu::Do()
{
	BeginFrame();
	ComposeFrame(  );	
	if(mBack->ButtonClicked(ic, gfx))
	{		
		core.curState = core.prevState;
		core.prevState = StateCore::GState::SCOREMENU;
	}
}

void ScoreMenu::Draw()
{
	mBack->Draw(f, gfx);
		
	unsigned int y = 25;
	for( unsigned int index = 0; index < entries.size() && index < 10; index++ )
	{
		gfx.DrawString( entries[ index ].c_str(), 50, y, &f, gfx.FILLCOLOR_XRGB( 255,255,255 ) );
		y += 50;
	}
}

ScoreMenu::~ScoreMenu()
{
	entries.clear();
}

// OptionsMenu class
OptionsMenu::OptionsMenu( StateController &sc, StateCore &core)
	:
	GameMenu(sc,core )
{
	unsigned int width = 50;
	unsigned int height = 100;
	D3DGraphics::Color c = gfx.FILLCOLOR_XRGB(255, 255, 255);
	mBack = std::make_unique<Button>(c, 600, 50, width, height, "Back");

	filename = std::string("options.txt");
	LoadConfig();
}

void OptionsMenu::Do()
{
	BeginFrame();
	ComposeFrame();
}

void OptionsMenu::Draw()
{
	mBack->Draw(f, gfx);
}

void OptionsMenu::LoadConfig()
{
	//FILE *pFile = fopen(fileName, "r");
	std::ifstream cFile;
	cFile.open(filename, std::ios::in);

	if(!cFile.is_open())
	{
		// Set defaults
		controller = "kbd";

		keyAssign[UP] = "up";
		keyAssign[DOWN] = "down";
		keyAssign[LEFT] = "left";
		keyAssign[RIGHT] = "right";
		keyAssign[FIRE] = "space";

		resX = 800;
		resY = 600;
		fScreen = false;

		mVolume = 1.0f;
		sfxVolume = 1.0f;
	}
	else
	{
		// fscanf(pFile, "%s", controller);
		/*fscanf(pFile, "%s", keyAssign[UP]);
		fscanf(pFile, "%s", keyAssign[DOWN]);
		fscanf(pFile, "%s", keyAssign[LEFT]);
		fscanf(pFile, "%s", keyAssign[RIGHT]);
		fscanf(pFile, "%s", keyAssign[FIRE]);*/
		/*fscanf(pFile, "%d", resX);
		fscanf(pFile, "%d", resY);
		fscanf(pFile, "%d", fScreen);
		fscanf(pFile, "%f", mVolume);
		fscanf(pFile, "%f", sfxVolume);*/

		char temp[10];

		cFile.getline(temp, 10);
		controller.assign(temp);
		cFile.getline(temp, 10);
		keyAssign[UP].assign(temp);
		cFile.getline(temp, 10);
		keyAssign[DOWN].assign(temp);
		cFile.getline(temp, 10);
		keyAssign[LEFT].assign(temp);
		cFile.getline(temp, 10);
		keyAssign[RIGHT].assign(temp);
		cFile.getline(temp, 10);
		keyAssign[FIRE].assign(temp);
		cFile.getline(temp, 4);
		resX = atoi(temp);
		cFile.getline(temp, 4);
		resY = atoi(temp);
		cFile.getline(temp, 1);
		if(temp)
		{
			fScreen = true;
		}
		else
		{
			fScreen = false;
		}
		cFile.getline(temp, 5);
		mVolume = atof(temp);
		cFile.getline(temp, 5);
		sfxVolume = atof(temp);
	}
	cFile.close();
}

void OptionsMenu::GetControlsOptions(std::string& controllerType, std::string *keyAssignments)
{
	 controllerType = controller;
	 keyAssignments = keyAssign;
}

void OptionsMenu::GetGraphicsOptions(unsigned int* scrnRes, bool &isFullScreen)
{
	scrnRes[0] = resX;
	scrnRes[1] = resY;
	isFullScreen = fScreen;
}

void OptionsMenu::GetSoundOptions(float &musicVolume, float &sfxVolume)
{
	musicVolume = mVolume;
	sfxVolume = this->sfxVolume;
}

void OptionsMenu::SaveConfig()
{
	/*FILE *pFile = fopen(fileName, "w");

	fprintf(pFile, "%s", controller);
	fprintf(pFile, "%s", keyAssign[UP]);
	fprintf(pFile, "%s", keyAssign[DOWN]);
	fprintf(pFile, "%s", keyAssign[LEFT]);
	fprintf(pFile, "%s", keyAssign[RIGHT]);
	fprintf(pFile, "%s", keyAssign[FIRE]);
	fprintf(pFile, "%d", resX);
	fprintf(pFile, "%d", resY);
	fprintf(pFile, "%d", fScreen);
	fprintf(pFile, "%f", mVolume);
	fprintf(pFile, "%f", sfxVolume);*/
	char temp[10]{};
	std::ofstream cFile;
	cFile.open(filename, std::ios::out);
	cFile << controller;
	cFile << keyAssign[UP];
	cFile << keyAssign[DOWN];
	cFile << keyAssign[LEFT];
	cFile << keyAssign[RIGHT];
	cFile << keyAssign[FIRE];
	sprintf(temp, "%d", resX);
	cFile << temp;
	sprintf(temp, "%d", resY);
	cFile << temp;
	if(fScreen)
	{
		temp[0] = 1;
	}
	else
	{
		temp[0] = 0;
	}
	cFile << temp;
	sprintf(temp, "%f", mVolume);
	cFile << temp;
	sprintf(temp, "%f", sfxVolume);
	cFile << temp;

	cFile.close();
	delete [] temp;
}

OptionsMenu::~OptionsMenu()
{
	SaveConfig();
}
