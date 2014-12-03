#pragma once

namespace sf
{
	class RenderWindow;
	class RectangleShape;
	class Texture;
	class Font;
	class String;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;
}
class I_Player;

namespace FourWins
{
	namespace Menu
	{
		class ListboxEntry;

		class Listbox
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *container;
			sf::RectangleShape *topBox;
			sf::RectangleShape *bottomBox;
			ListboxEntry *entries;
			const unsigned int ENTRY_COUNT;

			static const float ENTRY_HEIGHT;

		public:
			Listbox(sf::RenderWindow &window, unsigned int entryCount);
			~Listbox();
			void init();

			Listbox &setFont(const sf::Font &font);
			Listbox &setPosition(const sf::Vector2f &position);
			Listbox &setTexture(const sf::Texture *texture);
			Listbox &setValueForEntry(unsigned int index, unsigned char value);
			Listbox &setStringForEntry(unsigned int index, const sf::String &text);
			Listbox &setTopTextureRect(const sf::IntRect &rect);
			Listbox &setBottomTextureRect(const sf::IntRect &rect);
			Listbox &setDefaultEntry(unsigned int index);
			unsigned char getValueOfActive() const;
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();

		private:
			void resetHover();
			void checkForHover(const sf::Vector2f &mousePosition);
			void checkForActive(const sf::Vector2f &mousePosition);
			Listbox &operator=(const Listbox&);
		};
	}
}