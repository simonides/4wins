#pragma once

namespace sf
{
	class RenderWindow;
	class Font;
	class Text;
	class RectangleShape;
	class Texture;
}
class I_Player;
class ResourceManager;

struct GameSettings_obsolete
{
	I_Player *playerOne;
	I_Player *playerTwo;
};

namespace FourWins
{
	namespace Menu
	{
		class Button;
		class Listbox;
		class Checkbox;

		class MainMenu
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *backgroundShape;
			sf::RectangleShape *headlineShape;
			sf::Text *labelPlayer1;
			sf::Text *labelPlayer2;
			Listbox *lbPlayer1;
			Listbox *lbPlayer2;
			Checkbox *cbMeepleChoose;
			Checkbox *cbMeeplePos;
			Button *btnStart;
			bool startGame;

		public:
			explicit MainMenu(sf::RenderWindow &window);
			~MainMenu();
			void init(ResourceManager &resourceManager);

			GameSettings loop();

		private:
			void pollEvents();
			void checkListboxes();
			GameSettings createSettings();
		};
	}
}