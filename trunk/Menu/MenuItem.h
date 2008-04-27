#pragma once

#include <vector>
#include <string>

using namespace std;

class Texture;
class Graphics;

class MenuItem {
public:
	enum ItemState {
		ITEM_STATE_UNSELECTED,
		ITEM_STATE_SELECTED,
		NUM_ITEM_STATES
	};

private:
	ItemState m_state;
	int m_width, m_height;
	float m_posX, m_posY;
	vector<Texture*> m_textureList;

	bool m_selectable;
	bool m_visible;

public:
	~MenuItem(); 

	void init(float m_posX, float m_posY, int width, int height, const string &unselectedTexture, const string &selectedTexture, ItemState state);
	void update(float dt);
	void render(Graphics& g) const;

	const ItemState &getState() const { return m_state; };
	void setState(ItemState state) { m_state = state; };

	void setSelectable(bool selectable) { m_selectable = selectable; };
	const bool isSelectable() const { return m_selectable; };

	void setVisible(bool visible) { m_visible = visible; };
	const bool isVisible() const { return m_visible; };

};