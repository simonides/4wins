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
		class ImageCheckbox
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *shape;
			sf::IntRect *textureRect;
			sf::IntRect *textureHighlightRect;
			bool isChecked;

		public:
			explicit ImageCheckbox(sf::RenderWindow &window);
			~ImageCheckbox();

			inline void setTexture(const sf::Texture *texture)
			{
				this->shape->setTexture(texture);
			}
			inline void setPosition(const sf::Vector2f &position)
			{
				this->shape->setPosition(position);
			}
			inline void setSize(const sf::Vector2f &size)
			{
				this->shape->setSize(size);
			}

			void setTextureRect(const sf::IntRect &rect);
			void setTextureHighlightRect(const sf::IntRect &rect);

			inline bool getIsChecked() const
			{
				return this->isChecked;
			}

			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();

		private:
			void switchTextureRect();
		};
	}
}