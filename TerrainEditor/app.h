#pragma once

#include <string>

void glutResize(int width, int height); 
void glutDisplay(void);
void glutKeyboard(unsigned char key, int x, int y);
void glutMouse(int button, int state, int x, int y);   
void glutMotion(int x, int y);  
void glutPassiveMotion(int x, int y);  
void glutSpecial(int value, int x, int y); 
void glutMenu(int value);
void glutIdle();

std::string loadTextureDlg();

void FreeResources();