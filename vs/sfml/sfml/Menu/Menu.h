#pragma once

namespace sf
{
	class Font;
	class RenderWindow;
	class Text;
	class RectangleShape;
}
class I_Player;

struct GameSettings
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

		class MainMenu
		{
		private:
			sf::RenderWindow *window;
			sf::Font *font;
			sf::Text *labelHeader;
			sf::Text *labelPlayer1;
			sf::Text *labelPlayer2;
			Listbox *lbPlayer1;
			Listbox *lbPlayer2;
			Button *btnStart;
			bool startGame;

		public:
			explicit MainMenu(sf::RenderWindow &window);
			~MainMenu();
			void init();

			GameSettings loop();

		private:
			void pollEvents();
			GameSettings createSettings();
		};
	}
}