#pragma once

namespace sf
{
	class RenderWindow;
	class Font;
	class Text;
	class RectangleShape;
	class Texture;
}
class ResourceManager;
class ParticleSystem;
class ParticleBuilder;
struct GameSettings;

namespace FourWins
{
	namespace Menu
	{
		class Button;
		class Listbox;
		class Checkbox;
		class AvatarChooser;

		class MainMenu
		{
		private:
			sf::RenderWindow *window;
			ParticleSystem *particleSystem;
			ParticleBuilder *selectionParticleRain;
			sf::RectangleShape *backgroundShape;
			sf::RectangleShape *headlineShape;
			sf::Text *labelPlayer1;
			sf::Text *labelPlayer2;
			Listbox *lbPlayer1;
			Listbox *lbPlayer2;
			Checkbox *cbMeepleChoose;
			Checkbox *cbMeeplePos;
			AvatarChooser *acPlayer1;
			AvatarChooser *acPlayer2;
			Button *btnStart;
			bool startGame;

		public:
			explicit MainMenu(sf::RenderWindow &window);
			~MainMenu();
			void init(ResourceManager &resourceManager);

			GameSettings *loop();

		private:
			void pollEvents();
			void checkListboxes();
			GameSettings *createSettings();
		};
	}
}