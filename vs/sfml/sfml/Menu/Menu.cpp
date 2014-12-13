#include <SFML/Graphics.hpp>
#include <sstream>

#include "../config.h"
#include "../helper.h"
#include "../ThinkingAI.h"
#include "../ResourceManager.h"
#include "../GameSettings.h"
#include "../ParticleSystem.h"
#include "../ParticleBuilder.h"
#include "../Tutorial.h"
#include "MenuConstants.h"
#include "Listbox.h"
#include "Checkbox.h"
#include "Button.h"
#include "AvatarChooser.h"
#include "AvatarEnum.h"
#include "MusicMutebox.h"
#include "SfxMutebox.h"

#include "Menu.h"

using namespace FourWins;

FourWins::Menu::MainMenu::MainMenu(sf::RenderWindow &window, Tutorial &tutorial) :
	window(&window),
	tutorial(&tutorial),
	particleSystem(nullptr),
	selectionParticleRain(nullptr),
	backgroundShape(new sf::RectangleShape()),
	headlineShape(new sf::RectangleShape()),
	labelPlayer1(new sf::Text()),
	labelPlayer2(new sf::Text()),
	lbPlayer1(new Menu::Listbox(window, 8u)),
	lbPlayer2(new Menu::Listbox(window, 8u)),
	cbMeepleChoose(new Menu::Checkbox(window)),
	cbMeeplePos(new Menu::Checkbox(window)),
	acPlayer1(new Menu::AvatarChooser(window, 8u)),
	acPlayer2(new Menu::AvatarChooser(window, 8u)),
	musicMutebox(new MusicMutebox()),
	sfxMutebox(new SfxMutebox()),
	btnTutorial(new Menu::Button(window)),
	btnStart(new Menu::Button(window)),
	startGame(false)
{}

FourWins::Menu::MainMenu::~MainMenu()
{
	delete this->backgroundShape;
	delete this->particleSystem;
	delete this->selectionParticleRain;
	delete this->headlineShape;
	delete this->labelPlayer1;
	delete this->labelPlayer2;
	delete this->lbPlayer1;
	delete this->lbPlayer2;
	delete this->cbMeepleChoose;
	delete this->cbMeeplePos;
	delete this->acPlayer1;
	delete this->acPlayer2;
	delete this->musicMutebox;
	delete this->sfxMutebox;
	delete this->btnTutorial;
	delete this->btnStart;
}

void FourWins::Menu::MainMenu::init(ResourceManager &resourceManager, SoundManager &soundManager)
{
	const sf::Texture *menuAtlas = resourceManager.getTexture(resourceManager.MENU_ATLAS);
	const sf::Font *font = resourceManager.getFont(resourceManager.ROBOTO);

	this->particleSystem = new ParticleSystem(*resourceManager.getTexture(resourceManager.PARTICLE_SPRITE), sf::Vector2u(4, 2));
	this->selectionParticleRain = new ParticleBuilder(sf::Vector2f(), { 5.0f, 18.0f });

	this->selectionParticleRain->setSprites(sf::Vector2u(0, 2), sf::Vector2u(0, 2))
		->setPath({ 100, 300 }, { 200, 340 })
		->setGravity(200, 800)
		->setRotation({ 0, 600 }, { 0.5, 2.5 })
		->setDynamicColor({ 2.0f, 8.0f });

	this->backgroundShape->setTexture(resourceManager.getTexture(resourceManager.BACKGROUND_TEX));
	this->backgroundShape->setTextureRect(resourceManager.getTextureRect(resourceManager.BACKGROUND));
	this->backgroundShape->setSize(sf::Vector2f(static_cast<float>(WINDOW_WIDTH_TO_CALCULATE), static_cast<float>(WINDOW_HEIGHT_TO_CALCULATE)));

	this->headlineShape->setTexture(menuAtlas);
	this->headlineShape->setTextureRect(resourceManager.getTextureRect(resourceManager.MENU_HEADLINE));
	this->headlineShape->setSize(sf::Vector2f(500.0f, 140.0f)); 
	this->headlineShape->setPosition(sf::Vector2f(425.0f, 10.0f));

	this->labelPlayer1->setFont(*font);
	this->labelPlayer1->setString("Select player 1:");
	this->labelPlayer1->setCharacterSize(30u);
	this->labelPlayer1->setColor(Menu::LABEL_COLOR);
	this->labelPlayer1->setPosition(sf::Vector2f(417.0f, 150.0f));

	this->labelPlayer2->setFont(*font);
	this->labelPlayer2->setString("Select player 2:");
	this->labelPlayer2->setCharacterSize(30u);
	this->labelPlayer2->setColor(Menu::LABEL_COLOR);
	this->labelPlayer2->setPosition(sf::Vector2f(725.0f, 150.0f));

	this->lbPlayer1->init();
	this->lbPlayer1->setFont(*font).
		setTexture(menuAtlas).
		setTopTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_UP)).
		setBottomTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_DOWN)).
		setPosition(sf::Vector2f(437.0f, 215.0f)).
		setStringForEntry(0, "Human Player").
		setValueForEntry(0, 'h').
		setStringForEntry(1u, "Stupid AI").
		setValueForEntry(1u, 's').
		setStringForEntry(2u, "Random AI").
		setValueForEntry(2u, 'r').
		setStringForEntry(3u, "Thinking AI").
		setValueForEntry(3u, 't').
		setStringForEntry(4u, "Smart AI").
		setValueForEntry(4u, 'm').
		setStringForEntry(5u, "Newton AI").
		setValueForEntry(5u, 'm').//TODO neu mappen
		setStringForEntry(6u, "Networking AI").
		setValueForEntry(6u, 'm').//TODO neu mappen
		setStringForEntry(7u, "??? AI").
		setValueForEntry(7u, 'x').
		setDefaultEntry(0);

	this->lbPlayer2->init();
	this->lbPlayer2->setFont(*font).
		setTexture(menuAtlas).
		setTopTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_UP)).
		setBottomTextureRect(resourceManager.getTextureRect(resourceManager.MENU_FRAME_DOWN)).
		setPosition(sf::Vector2f(745.0f, 215.0f)).
		setStringForEntry(0, "Human Player").
		setValueForEntry(0, 'h').
		setStringForEntry(1u, "Stupid AI").
		setValueForEntry(1u, 's').
		setStringForEntry(2u, "Random AI").
		setValueForEntry(2u, 'r').
		setStringForEntry(3u, "Thinking AI").
		setValueForEntry(3u, 't').
		setStringForEntry(4u, "Smart AI").
		setValueForEntry(4u, 'm').
		setStringForEntry(5u, "Newton AI").
		setValueForEntry(5u, 'm').//TODO neu mappen
		setStringForEntry(6u, "Networking AI").
		setValueForEntry(6u, 'm').//TODO neu mappen
		setStringForEntry(7u, "??? AI").
		setValueForEntry(7u, 'x').
		setDefaultEntry(0);

	this->cbMeepleChoose->init();
	this->cbMeepleChoose->setFont(*font);
	this->cbMeepleChoose->setLabelText("intelligent meeple choosing");
	this->cbMeepleChoose->setCharacterSize(24u);
	this->cbMeepleChoose->setYPos(515.0f);
	this->cbMeepleChoose->setLeftBoxXPos(445.0f);
	this->cbMeepleChoose->setLabelXPos(525.0f);
	this->cbMeepleChoose->setRightBoxXPos(875.0f);
	this->cbMeepleChoose->setLeftVisible(true);
	this->cbMeepleChoose->setRightVisible(true);

	this->cbMeeplePos->init();
	this->cbMeeplePos->setFont(*font);
	this->cbMeeplePos->setLabelText("intelligent meeple positioning");
	this->cbMeeplePos->setCharacterSize(24u);
	this->cbMeeplePos->setYPos(560.0f);
	this->cbMeeplePos->setLeftBoxXPos(445.0f);
	this->cbMeeplePos->setLabelXPos(515.0f);
	this->cbMeeplePos->setRightBoxXPos(875.0f);
	this->cbMeeplePos->setLeftVisible(true);
	this->cbMeeplePos->setRightVisible(true);

	const sf::IntRect smoothSteveRect = resourceManager.getTextureRect(resourceManager.SMOOTH_STEVE);
	const sf::IntRect profJenkinsRect = resourceManager.getTextureRect(resourceManager.PROFESSOR_JENKINS);
	const sf::IntRect hipsterHenryRect = resourceManager.getTextureRect(resourceManager.HIPSTER_HENRY);
	const sf::IntRect boybandBillyRect = resourceManager.getTextureRect(resourceManager.BOYBAND_BILLY);
	const sf::IntRect bookwormBettyRect = resourceManager.getTextureRect(resourceManager.BOOKWORM_BETTY);
	const sf::IntRect fashionFabienneRect = resourceManager.getTextureRect(resourceManager.FASHION_FABIENNE);
	const sf::IntRect hippieHildyRect = resourceManager.getTextureRect(resourceManager.HIPPIE_HILDY);
	const sf::IntRect smokinStacyRect = resourceManager.getTextureRect(resourceManager.SMOKIN_STACY);

	this->acPlayer1->init();
	this->acPlayer1->setFont(*font).
		setTexture(resourceManager.getTexture(resourceManager.AVATAR_SPRITE)).
		setPreviewPosition(sf::Vector2f(39.0f, 28.0f)).
		setThumbnailBoxPosition(sf::Vector2f(76.0f, 510.0f)).
		setEnumForEntry(0, Menu::Avatars::SMOOTH_STEVE).
		setTextureRectsForEntry(0, smoothSteveRect, smoothSteveRect).
		setStringForEntry(0, "Smooth Steve").
		setEnumForEntry(1u, Menu::Avatars::PROFESSOR_JENKINS).
		setTextureRectsForEntry(1u, profJenkinsRect, profJenkinsRect).
		setStringForEntry(1u, "Professor Jenkins").
		setEnumForEntry(2u, Menu::Avatars::HIPSTER_HENRY).
		setTextureRectsForEntry(2u, hipsterHenryRect, hipsterHenryRect).
		setStringForEntry(2u, "Hipster Henry").
		setEnumForEntry(3u, Menu::Avatars::BOYBAND_BILLY).
		setTextureRectsForEntry(3u, boybandBillyRect, boybandBillyRect).
		setStringForEntry(3u, "Boyband Billy").
		setEnumForEntry(4u, Menu::Avatars::BOOKWORM_BETTY).
		setTextureRectsForEntry(4u, bookwormBettyRect, bookwormBettyRect).
		setStringForEntry(4u, "Bookworm Betty").
		setEnumForEntry(5u, Menu::Avatars::FASHION_FABIENNE).
		setTextureRectsForEntry(5u, fashionFabienneRect, fashionFabienneRect).
		setStringForEntry(5u, "Fashion Fabienne").
		setEnumForEntry(6u, Menu::Avatars::HIPPIE_HILDY).
		setTextureRectsForEntry(6u, hippieHildyRect, hippieHildyRect).
		setStringForEntry(6u, "Hippie Hildy").
		setEnumForEntry(7u, Menu::Avatars::SMOKIN_STACY).
		setTextureRectsForEntry(7u, smokinStacyRect, smokinStacyRect).
		setStringForEntry(7u, "Smokin Stacy").
		setDefaultEntry();

	this->acPlayer2->init();
	this->acPlayer2->setFont(*font).
		setTexture(resourceManager.getTexture(resourceManager.AVATAR_SPRITE)).
		setPreviewPosition(sf::Vector2f(990.0f, 28.0f)).
		setThumbnailBoxPosition(sf::Vector2f(1027.0f, 510.0f)).
		setEnumForEntry(0, Menu::Avatars::SMOOTH_STEVE).
		setTextureRectsForEntry(0, smoothSteveRect, smoothSteveRect).
		setStringForEntry(0, "Smooth Steve").
		setEnumForEntry(1u, Menu::Avatars::PROFESSOR_JENKINS).
		setTextureRectsForEntry(1u, profJenkinsRect, profJenkinsRect).
		setStringForEntry(1u, "Professor Jenkins").
		setEnumForEntry(2u, Menu::Avatars::HIPSTER_HENRY).
		setTextureRectsForEntry(2u, hipsterHenryRect, hipsterHenryRect).
		setStringForEntry(2u, "Hipster Henry").
		setEnumForEntry(3u, Menu::Avatars::BOYBAND_BILLY).
		setTextureRectsForEntry(3u, boybandBillyRect, boybandBillyRect).
		setStringForEntry(3u, "Boyband Billy").
		setEnumForEntry(4u, Menu::Avatars::BOOKWORM_BETTY).
		setTextureRectsForEntry(4u, bookwormBettyRect, bookwormBettyRect).
		setStringForEntry(4u, "Bookworm Betty").
		setEnumForEntry(5u, Menu::Avatars::FASHION_FABIENNE).
		setTextureRectsForEntry(5u, fashionFabienneRect, fashionFabienneRect).
		setStringForEntry(5u, "Fashion Fabienne").
		setEnumForEntry(6u, Menu::Avatars::HIPPIE_HILDY).
		setTextureRectsForEntry(6u, hippieHildyRect, hippieHildyRect).
		setStringForEntry(6u, "Hippie Hildy").
		setEnumForEntry(7u, Menu::Avatars::SMOKIN_STACY).
		setTextureRectsForEntry(7u, smokinStacyRect, smokinStacyRect).
		setStringForEntry(7u, "Smokin Stacy").
		setDefaultEntry();

	this->musicMutebox->init(resourceManager, soundManager);
	
	this->sfxMutebox->init(resourceManager, soundManager);
	
	this->btnTutorial->init();
	this->btnTutorial->setTexture(resourceManager.getTexture(resourceManager.TUTORIAL_BTN_SRITE));
	this->btnTutorial->setTextureRect(resourceManager.getTextureRect(resourceManager.TUT_BTN));
	this->btnTutorial->setTextureHighlightRect(resourceManager.getTextureRect(resourceManager.TUT_BTN_HIGHLIGHT));
	this->btnTutorial->setPosition(sf::Vector2f(10.0f, 10.0f));
	this->btnTutorial->setSize(sf::Vector2f(65.0f, 65.0f));

	this->btnStart->init();
	this->btnStart->setTexture(menuAtlas);
	this->btnStart->setTextureRect(resourceManager.getTextureRect(resourceManager.MENU_STARTBTN));
	this->btnStart->setTextureHighlightRect(resourceManager.getTextureRect(resourceManager.MENU_STARTBTN_H));
}

GameSettings *FourWins::Menu::MainMenu::loop()
{
	sf::Clock clock;
	float elapsedTime;
	float fpsElapsedTime = 0;

	this->musicMutebox->fitStateToVolume();
	this->sfxMutebox->fitStateToVolume();

	while (this->window->isOpen() && !this->startGame)
	{
		elapsedTime = clock.getElapsedTime().asSeconds();
		float fps = 1.f / elapsedTime;
		clock.restart();

		pollEvents();

		checkListboxes();

		if (this->btnStart->getIsReleased())
		{
			this->startGame = true;
		}
		if (this->btnTutorial->getIsReleased())
		{
			this->btnTutorial->resetReleased();
			this->tutorial->reset();
			this->tutorial->runLoop();
		}
		if (this->acPlayer1->getSelectionChanged())
		{
			this->selectionParticleRain->setPosition(sf::Vector2f(190.0f, 0.0f), { -5.0f, 5.0f });
			this->particleSystem->newParticleCloud(100, *this->selectionParticleRain);
			this->acPlayer1->resetSelectionChanged();
		}
		if (this->acPlayer2->getSelectionChanged())
		{
			this->selectionParticleRain->setPosition(sf::Vector2f(1140.0f, 0.0f), { -5.0f, 5.0f });
			this->particleSystem->newParticleCloud(100, *this->selectionParticleRain);
			this->acPlayer2->resetSelectionChanged();
		}
		this->particleSystem->update(elapsedTime);

		fpsElapsedTime += elapsedTime;
		if (fpsElapsedTime > 0.75)
		{
			std::ostringstream ss;
			ss.precision(4) ;
			ss << WINDOW_TITLE << fps << "fps";
			window->setTitle(ss.str());
			fpsElapsedTime = 0;
		}
	
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//draw
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		this->window->clear(sf::Color::White);
		this->window->draw(this->acPlayer1->getPreviewShape());
		this->window->draw(this->acPlayer2->getPreviewShape());
		this->particleSystem->draw(*this->window);
		this->window->draw(*this->backgroundShape);
		this->window->draw(*this->labelPlayer1);
		this->window->draw(*this->labelPlayer2);
		this->lbPlayer1->draw();
		this->lbPlayer2->draw();
		this->cbMeepleChoose->draw();
		this->cbMeeplePos->draw();
		this->acPlayer1->draw();
		this->acPlayer2->draw();
		this->musicMutebox->draw(*this->window);
		this->sfxMutebox->draw(*this->window);
		this->btnTutorial->draw();
		this->btnStart->draw();
		this->window->draw(*this->headlineShape);

		this->window->display();
	}
	this->startGame = false;
	this->btnStart->resetReleased();

	return createSettings();
}

void FourWins::Menu::MainMenu::pollEvents()
{
	sf::Event event;

	while (this->window->pollEvent(event))
	{
		sf::Vector2i mousepos = sf::Mouse::getPosition(*this->window);
		sf::Vector2f converted = this->window->mapPixelToCoords(mousepos);

		this->lbPlayer1->update(event, converted);
		this->lbPlayer2->update(event, converted);
		this->cbMeepleChoose->update(event, converted);
		this->cbMeeplePos->update(event, converted);
		this->acPlayer1->update(event, converted);
		this->acPlayer2->update(event, converted);
		this->btnTutorial->update(event, converted);
		this->btnStart->update(event, converted);

		switch (event.type)
		{
		case sf::Event::MouseMoved:
			this->musicMutebox->update(false, converted);
			this->sfxMutebox->update(false, converted);
			break;
		case sf::Event::MouseButtonPressed:
			this->musicMutebox->update(true, converted);
			this->sfxMutebox->update(true, converted);
			break;
		case sf::Event::Resized:
			handleResizeWindowEvent(window);
			break;

		case sf::Event::Closed:
			this->window->close();
			break;
		}
	}
}

void FourWins::Menu::MainMenu::checkListboxes()
{
	unsigned char player1Val = this->lbPlayer1->getValueOfActive();
	unsigned char player2Val = this->lbPlayer2->getValueOfActive();

	bool showLeftCb = (player1Val == 't' || player1Val == 'm');
	bool showRightCb = (player2Val == 't' || player2Val == 'm');

	this->cbMeepleChoose->setLeftVisible(showLeftCb);
	this->cbMeepleChoose->setRightVisible(showRightCb);

	this->cbMeeplePos->setLeftVisible(showLeftCb);
	this->cbMeeplePos->setRightVisible(showRightCb);
}

GameSettings *FourWins::Menu::MainMenu::createSettings()
{
    GameSettings *settings = new GameSettings();

    ResourceManager::ResourceRect player1Rect;
    ResourceManager::ResourceRect player2Rect;
    GameSettings::PlayerType player1Type;
    GameSettings::PlayerType player2Type;

    switch (this->acPlayer1->getSelectedAvatar())
    {
    case Menu::SMOOTH_STEVE:
        player1Rect = ResourceManager::SMOOTH_STEVE;
        break;
    case Menu::PROFESSOR_JENKINS:
        player1Rect = ResourceManager::PROFESSOR_JENKINS;
        break;
    case Menu::HIPSTER_HENRY:
        player1Rect = ResourceManager::HIPSTER_HENRY;
        break;
    case Menu::BOYBAND_BILLY:
        player1Rect = ResourceManager::BOYBAND_BILLY;
        break;
    case Menu::BOOKWORM_BETTY:
        player1Rect = ResourceManager::BOOKWORM_BETTY;
        break;
    case Menu::FASHION_FABIENNE:
        player1Rect = ResourceManager::FASHION_FABIENNE;
        break;
    case Menu::HIPPIE_HILDY:
        player1Rect = ResourceManager::HIPPIE_HILDY;
        break;
    case Menu::SMOKIN_STACY:
        player1Rect = ResourceManager::SMOKIN_STACY;
        break;
    default:
        player1Rect = ResourceManager::SMOOTH_STEVE;
        break;
    }
    switch (this->acPlayer2->getSelectedAvatar())
    {
    case Menu::SMOOTH_STEVE:
        player2Rect = ResourceManager::SMOOTH_STEVE;
        break;
    case Menu::PROFESSOR_JENKINS:
        player2Rect = ResourceManager::PROFESSOR_JENKINS;
        break;
    case Menu::HIPSTER_HENRY:
        player2Rect = ResourceManager::HIPSTER_HENRY;
        break;
    case Menu::BOYBAND_BILLY:
        player2Rect = ResourceManager::BOYBAND_BILLY;
        break;
    case Menu::BOOKWORM_BETTY:
        player2Rect = ResourceManager::BOOKWORM_BETTY;
        break;
    case Menu::FASHION_FABIENNE:
        player2Rect = ResourceManager::FASHION_FABIENNE;
        break;
    case Menu::HIPPIE_HILDY:
        player2Rect = ResourceManager::HIPPIE_HILDY;
        break;
    case Menu::SMOKIN_STACY:
        player2Rect = ResourceManager::SMOKIN_STACY;
        break;
    default:
        player2Rect = ResourceManager::SMOOTH_STEVE;
        break;
    }
    switch (this->lbPlayer1->getValueOfActive())
    {
    case 'h':
        player1Type = GameSettings::HUMAN;
        break;
    case 's':
        player1Type = GameSettings::STUPID_AI;
        break;
    case 'r':
        player1Type = GameSettings::RANDOM_AI;
        break;
    case 't':
        player1Type = GameSettings::THINKING_AI;
        break;
    case 'm':
        player1Type = GameSettings::SMART_AI;
        break;
    default:
        player1Type = GameSettings::HUMAN;
        break;
    }
    switch (this->lbPlayer2->getValueOfActive())
    {
    case 'h':
        player2Type = GameSettings::HUMAN;
        break;
    case 's':
        player2Type = GameSettings::STUPID_AI;
        break;
    case 'r':
        player2Type = GameSettings::RANDOM_AI;
        break;
    case 't':
        player2Type = GameSettings::THINKING_AI;
        break;
    case 'm':
        player2Type = GameSettings::SMART_AI;
        break;
    default:
        player2Type = GameSettings::HUMAN;
        break;
    }

    settings->avatar[0] = player1Rect;
    settings->avatar[1] = player2Rect;
    settings->playerType[0] = player1Type;
    settings->playerType[1] = player2Type;
    settings->aiOptions[0].useIntelligentMeepleChoosing = this->cbMeepleChoose->isLeftChecked();
    settings->aiOptions[0].useIntelligentMeeplePositioning = this->cbMeeplePos->isLeftChecked();
    settings->aiOptions[1].useIntelligentMeepleChoosing = this->cbMeepleChoose->isRightChecked();
    settings->aiOptions[1].useIntelligentMeeplePositioning = this->cbMeeplePos->isRightChecked();
	settings->musicMuted = this->musicMutebox->getIsChecked();
	settings->effectsMuted = this->sfxMutebox->getIsChecked();

	return settings;
}

//if (event.type == sf::Event::KeyPressed){
//if (event.key.code == sf::Keyboard::K){	}
//}
//if (event.type == sf::Event::KeyReleased){
//if (event.key.code == sf::Keyboard::K){	}
//}
//if (event.type == sf::Event::MouseWheelMoved){	}

//if (event.type == sf::Event::MouseButtonPressed)
//{
//if (event.mouseButton.button == sf::Mouse::Left){
//std::cout << "mouse x: " << event.mouseButton.x << std::endl;
//std::cout << "mouse y: " << event.mouseButton.y << std::endl;
//}
//if (event.mouseButton.button == sf::Mouse::Right){
//std::cout << "the right button was pressed" << std::endl;
//}
//}