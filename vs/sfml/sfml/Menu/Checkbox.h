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
			sf::RectangleShape *boxes;
			sf::Text *boxCrosses;
			sf::Text *label;
			bool *isVisible;

		public:
			explicit Checkbox(sf::RenderWindow &window);
			~Checkbox();
			void init();

			void setFont(const sf::Font &font);
			void setCharacterSize(unsigned int size);
			void setYPos(float y);
			void setLeftBoxXPos(float x);
			void setRightBoxXPos(float x);
			void setLabelXPos(float x);

			inline void setLabelText(const sf::String &text)
			{
				this->label->setString(text);
			}
			inline void setLeftVisible(bool value)
			{
				this->isVisible[0] = value;
			}
			inline void setRightVisible(bool value)
			{
				this->isVisible[1] = value;
			}
			inline bool isLeftChecked() const
			{
				return !this->boxCrosses[0].getString().isEmpty();
			}
			inline bool isRightChecked() const
			{
				return !this->boxCrosses[1].getString().isEmpty();
			}

			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();
		};
	}
}