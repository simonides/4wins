#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Text;
	class Font;
	class String;
	class Color;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
	typedef unsigned char Uint8;
}

namespace FourWins
{
	namespace Menu
	{
		class Button
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *background;
			sf::Text *text;
			bool isReleased;

		public:
			explicit Button(sf::RenderWindow &window);
			~Button();
			void init();

			void setFont(const sf::Font &font);
			void setPosition(const sf::Vector2f &position);
			void setSize(const sf::Vector2f &size);
			void setText(const sf::String &text);
			void setCharacterSize(unsigned int size);
			void setAlpha(sf::Uint8 alpha);
			bool getIsReleased() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();
		};
	}
}