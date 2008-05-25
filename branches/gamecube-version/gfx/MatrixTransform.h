#ifndef _RES_MATRIX_TRANSFORM_H_
#define _RES_MATRIX_TRANSFORM_H_

#include <demo.h>

class Matrix44;

class MatrixTransform {
private:
	static Mtx stateMtx[16];
	static int currentMtx;

public:
	static void PushMatrix(Mtx viewMtx, bool firstPush) { //TODO: the view matrix should be got as a static from the render engine
		if (firstPush)
			currentMtx = 0;
		else
			currentMtx += currentMtx < 15 ? 1 : 0;
		Mtx m;
		MTXIdentity(m);
		if (currentMtx > 0)
			MTXConcat(stateMtx[currentMtx-1], m, stateMtx[currentMtx]); 
		else
			MTXConcat(viewMtx, m, stateMtx[currentMtx]); 
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void ApplyNormalMatrix()
	{
		Mtx mvi,mv;
		MTXInverse(stateMtx[currentMtx], mvi); 
		MTXTranspose(mvi, mv); 
		GXLoadNrmMtxImm(mv, GX_PNMTX0);
	}

	static void PushMatrix()
	{
		memcpy(stateMtx[currentMtx + 1],stateMtx[currentMtx],sizeof(Mtx));
		currentMtx++;
	}

	static void PopMatrix() {
		currentMtx -= currentMtx > 0 ? 1 : 0;
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void LoadIdentity(Mtx viewMtx) {
		Mtx m;
		MTXIdentity(m);
		MTXConcat(viewMtx, m, stateMtx[currentMtx]); 
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void LoadIdentity() {
		MTXIdentity(stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	}

	static void Rotate(float degree, bool x, bool y, bool z) {
		Mtx m;
		MTXIdentity(m);
		if (x)
			MTXRotDeg(m, 'x', degree);
		if (y)
			MTXRotDeg(m, 'y', degree);
		if (z)
			MTXRotDeg(m, 'z', degree);
		MTXConcat(stateMtx[currentMtx], m, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void Translate(float x, float y, float z) {
		Mtx m;
		MTXIdentity(m);
		MTXTrans(m, x, y, z);
		MTXConcat(stateMtx[currentMtx], m, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void Scale(float x, float y, float z) {
		Mtx m;
		MTXIdentity(m);
		MTXScale(m, x, y, z);
		MTXConcat(stateMtx[currentMtx], m, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void MulMatrix(const Mtx &mat) {
		MTXConcat(stateMtx[currentMtx], mat, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void MulMatrix(const Matrix44 &mat);

	static void LoadMatrix(Mtx viewMtx, const Mtx &mat) {
		MTXConcat(viewMtx, mat, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
	};

	static void LoadMatrix(Mtx viewMtx, const Matrix44 &mat);

};

#endif