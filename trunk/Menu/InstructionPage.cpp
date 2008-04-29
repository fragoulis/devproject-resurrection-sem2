#include "InstructionPage.h"

void InstructionPage::init(int screenWidth, int screenHeight) {
	m_backgroundImage = TextureIO::instance()->getTexture("instructionsBg.dds");
	m_hasBackground = true;

	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
}

void InstructionPage::update(float dt) {

}