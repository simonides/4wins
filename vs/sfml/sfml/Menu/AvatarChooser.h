#pragma once
#include "AvatarEnum.h"

namespace sf
{
	class RenderWindow;
	class Font;
	class Text;
	class String;
	class RectangleShape;
	class Texture;
	class Event;

	template<typename T> class Vector2;
	typedef Vector2<float> Vector2f;

	template<typename T> class Rect;
	typedef Rect<float> FloatRect;
	typedef Rect<int> IntRect;
}

namespace FourWins
{
	namespace Menu
	{
		struct AvatarChoice
		{
			sf::RectangleShape *thumbnailShape;
			sf::String *previewString;
			sf::IntRect *previewTextureRect;
			Avatars avatar;
		};

		class AvatarChooser
		{
		private:
			sf::RenderWindow *window;
			sf::RectangleShape *bb;
			sf::RectangleShape *previewShape;
			sf::RectangleShape *previewTextShape;
			sf::Text *previewText;
			AvatarChoice *choices;
			const unsigned int CHOICE_COUNT;
			AvatarChoice *selectedChoice;
			bool selectionChanged;

			static const float THUMB_WIDTH;
			static const float THUMB_HEIGHT;
			static const float THUMB_SPACE;
			static const float PREVIEW_WIDTH;
			static const float PREVIEW_HEIGHT;

		public:
			AvatarChooser(sf::RenderWindow &window, unsigned int choiceCount);
			~AvatarChooser();
			void init();

			AvatarChooser &setTexture(const sf::Texture *texture);
			inline AvatarChooser &setFont(const sf::Font &font)
			{
				this->previewText->setFont(font);
				return *this;
			}
			AvatarChooser &setThumbnailBoxPosition(const sf::Vector2f &position);
			AvatarChooser &setPreviewPosition(const sf::Vector2f &position);
			AvatarChooser &setTextureRectsForEntry(unsigned int index, const sf::IntRect &thumbRect, const sf::IntRect &previewRect);
			AvatarChooser &setEnumForEntry(unsigned int index, Avatars value);
			AvatarChooser &setStringForEntry(unsigned int index, const sf::String &str);
			//opterator[] useful ??
			AvatarChooser &setDefaultEntry(unsigned int index);
			inline const sf::RectangleShape &getPreviewShape() const
			{
				return *this->previewShape;
			}
			inline Avatars getSelectedAvatar() const
			{
				return this->selectedChoice->avatar;
			}
			inline bool getSelectionChanged() const
			{
				return this->selectionChanged;
			}
			inline void resetSelectionChanged()
			{
				this->selectionChanged = false;
			}
			void update(const sf::Event &e, const sf::Vector2f &mousePosition);
			void draw();

		private:
			void resetHover();
			void checkForHover(const sf::Vector2f &mousePosition);
			void checkForSelected(const sf::Vector2f &mousePosition);
			AvatarChooser &operator=(const AvatarChooser&);
		};
	}
}