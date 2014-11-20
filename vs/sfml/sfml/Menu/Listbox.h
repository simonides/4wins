#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Vertex;
	class Text;
	class Font;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
}
class I_Player;

namespace FourWins
{
	namespace Menu
	{
		class Listbox
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *container;	//The container for the text rendering as background with border
			sf::RectangleShape *textsBB;	//Array of BoundingBoxes(BB) for the text entries
			sf::Vertex *linesBetween;		//Array of verticies representing the lines between the BBs
			sf::Text *texts;				//Array of texts for the strings to render
			bool *isEntryActive;

			static const float ENTRY_HEIGHT;
			static const float LINE_TICKNESS;

		public:
			explicit Listbox(sf::RenderWindow &window);
			~Listbox();
			void init();

			void setFont(const sf::Font &font);
			void setPosition(const sf::Vector2f &position);
			const bool *getIsEntryActive() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();

		private:
			void resetHover();
			void checkForHover(const sf::Vector2f &mousePosition);
			void checkForActive(const sf::Vector2f &mousePosition);
		};
	}
}