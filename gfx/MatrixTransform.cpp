#include "MatrixTransform.h"

#include "../math/Matrix44.h"

Mtx MatrixTransform::stateMtx[16];
int MatrixTransform::currentMtx(0);

void MatrixTransform::MulMatrix(const Matrix44 &mat) {
		const float *f = mat.cfp();
		Mtx m;

		MTXRowCol(m, 0, 0)=f[0];
		MTXRowCol(m, 1, 0)=f[1];
		MTXRowCol(m, 2, 0)=f[2];

		MTXRowCol(m, 0, 1)=f[4];
		MTXRowCol(m, 1, 1)=f[5];
		MTXRowCol(m, 2, 1)=f[6];

		MTXRowCol(m, 0, 2)=f[8];
		MTXRowCol(m, 1, 2)=f[9];
		MTXRowCol(m, 2, 2)=f[10];

		MTXRowCol(m, 0, 3)=f[12];
		MTXRowCol(m, 1, 3)=f[13];
		MTXRowCol(m, 2, 3)=f[14];

		MTXConcat(stateMtx[currentMtx], m, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
}

void MatrixTransform::LoadMatrix(Mtx viewMtx, const Matrix44 &mat) {
		const float *f = mat.cfp();
		Mtx m;

		MTXRowCol(m, 0, 0)=f[0];
		MTXRowCol(m, 1, 0)=f[1];
		MTXRowCol(m, 2, 0)=f[2];

		MTXRowCol(m, 0, 1)=f[4];
		MTXRowCol(m, 1, 1)=f[5];
		MTXRowCol(m, 2, 1)=f[6];

		MTXRowCol(m, 0, 2)=f[8];
		MTXRowCol(m, 1, 2)=f[9];
		MTXRowCol(m, 2, 2)=f[10];

		MTXRowCol(m, 0, 3)=f[12];
		MTXRowCol(m, 1, 3)=f[13];
		MTXRowCol(m, 2, 3)=f[14];

		MTXConcat(viewMtx, m, stateMtx[currentMtx]);
		GXLoadPosMtxImm(stateMtx[currentMtx], GX_PNMTX0);
}