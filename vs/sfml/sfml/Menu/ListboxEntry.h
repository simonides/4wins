#pragma once

namespace sf
{
	class RectangleShape;
	class Text;
	class Font;
	class Color;
	class String;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
}

namespace FourWins
{
	namespace Menu
	{
		class ListboxEntry
		{
		private:
			sf::RectangleShape *background;
			sf::Text *text;
			bool isActive;
			unsigned char value;

		public:
			ListboxEntry();
			~ListboxEntry();

			inline void setBackgroundColor(const sf::Color &color)
			{
				this->background->setFillColor(color);
			}
			inline void setTextColor(const sf::Color &color)
			{
				this->text->setColor(color);
			}
			inline void setSize(const sf::Vector2f &size)
			{
				this->background->setSize(size);
			}

			void setPosition(const sf::Vector2f &position);

			inline void setFont(const sf::Font &font)
			{
				this->text->setFont(font);
			}
			inline void setCharacterSize(unsigned int size)
			{
				this->text->setCharacterSize(size);
			}
			inline void setString(const sf::String &text)
			{
				this->text->setString(text);
			}
			inline void setIsActive(bool value)
			{
				this->isActive = value;
			}
			inline void setValue(unsigned char value)
			{
				this->value = value;
			}
			inline const sf::RectangleShape &getShape() const
			{
				return *this->background;
			}
			inline const sf::Text &getText() const
			{
				return *this->text;
			}
			inline bool getIsActive() const
			{
				return this->isActive;
			}
			inline unsigned char getValue() const
			{
				return this->value;
			}
		};
	}
}