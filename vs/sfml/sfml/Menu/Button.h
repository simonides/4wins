#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Texture;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;

	template<typename T> class Rect;
	typedef Rect<int> IntRect;
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
			sf::IntRect *textureRect;
			sf::IntRect *textureHighlightRect;
			bool isReleased;

		public:
			explicit Button(sf::RenderWindow &window);
			~Button();
			void init();

			void setPosition(const sf::Vector2f &position);
			void setSize(const sf::Vector2f &size);
			void setTexture(const sf::Texture *texture);
			void setTextureRect(const sf::IntRect &rect);
			void setTextureHighlightRect(const sf::IntRect &rect);
			bool getIsReleased() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();
		};
	}
}