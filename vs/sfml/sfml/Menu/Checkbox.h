#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Text;
	class Font;
	class Color;
	class String;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
}

namespace FourWins
{
	namespace Menu
	{
		class Checkbox
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *box;
			sf::Text *boxCross;
			sf::Text *label;
			bool isVisible;

		public:
			explicit Checkbox(sf::RenderWindow &window);
			~Checkbox();
			void init();

			void setFont(const sf::Font &font);
			void setCharacterSize(unsigned int size);
			void setPosition(const sf::Vector2f &position);
			void setLabelText(const sf::String &text);
			bool isChecked() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();
		};
	}
}