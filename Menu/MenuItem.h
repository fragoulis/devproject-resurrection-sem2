#ifndef _RES_MENUITEM_H_
#define _RES_MENUITEM_H_

#include <vector>
#include <string>

class Texture;
class Graphics;

class MenuItem {
public:
	enum ItemState {
		ITEM_STATE_UNSELECTED,
		ITEM_STATE_SELECTED,
		NUM_ITEM_STATES
	};

protected:
	ItemState m_state;
	int m_width, m_height;
	int m_posX, m_posY;
	std::vector<Texture*> m_textureList;

	bool m_selectable;
	bool m_visible;

	float m_currentTime;

public:
	virtual ~MenuItem(); 

	void init(int m_posX, int m_posY, int width, int height, const std::string &unselectedTexture, const std::string &selectedTexture, ItemState state);
	virtual void update(float dt);
	virtual void render(Graphics& g) const;

	const ItemState getState() const { return m_state; };
	void setState(ItemState state) { m_state = state; };
	bool isSelected() const { return m_state == ITEM_STATE_SELECTED; }

	void setSelectable(bool selectable) { m_selectable = selectable; };
	const bool isSelectable() const { return m_selectable; };

	void setVisible(bool visible) { m_visible = visible; };
	const bool isVisible() const { return m_visible; };

};

#endif
