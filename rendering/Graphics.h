#ifndef _RES_GRAPHICS_H_
#define _RES_GRAPHICS_H_

/**
 * Graphics is used to encapsulate OpenGL.
 * Please try to not call OpenGL directly,
 * but use this class instead.
 */
class Graphics
{
public:
	Graphics();
	~Graphics();

	void reset();

	//void enableDepthTest() { glEnable(GL_DEPTH_TEST); }
	//void disableDepthTest() { glDisable(GL_DEPTH_TEST); }

	//void enableFaceCulling() { glEnable(GL_CULL_FACE); }
	//void enablePointSmoothing() { glEnable(GL_POINT_SMOOTH); }

	//void saveEnableFlags() { glPushAttrib(GL_ENABLE_BIT); }
	//void recallFlags() { glPopAttrib(); }

	//void pushMatrix() { glPushMatrix(); }
	//void popMatrix() { glPopMatrix(); }
	//void multCoordinateFrame(const CoordinateFrame& cf);

	//void setColor(const Color& c) { glColor4fv(c.cfp()); }
	//void setClearColor(const Color& c) { glClearColor(c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha()); }
	//void setMaterial(const Material& m);

	// Lights
	//void enableLighting();
	//void disableLighting() { glDisable(GL_LIGHTING); }
	//int pushLight(const Light& light);
	//void popLight();
	//void enableLight(int index) { glEnable(GL_LIGHT0 + index); }
	//void disableLight(int index) { glDisable(GL_LIGHT0 + index); }
	//void resetLights();
	//void setGlobalAmbient(const Color& c);

	// clearing buffers
	//void clearColorBuffer() { glClear(GL_COLOR_BUFFER_BIT); }
	//void clearDepthBuffer() { glClear(GL_DEPTH_BUFFER_BIT); }

	//// viewport
	//void setViewport(int x, int y, int width, int height) { glViewport(x, y, width, height); }




private:

};


#endif