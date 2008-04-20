#include "CreditsPage.h"

void CreditsPage::init(int screenWidth, int screenHeight) {
	m_backgroundImage = TextureIO::instance()->getTexture("creditsBg.bmp");
	m_hasBackground = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void CreditsPage::update(float dt) {

}