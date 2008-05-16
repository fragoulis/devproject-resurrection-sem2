####################################################
# NGC Makefile generated for GNU Make by ProDG VSI #
####################################################

.SILENT:
.SUFFIXES:
.SUFFIXES: .c .cpp .cxx .cc .s .asm

OUTDIR = NGC_Debug
INTDIR = NGC_Debug

LCFILE = "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/ngc.ld"
LIBS=\
	 "base.a"\
	 "os.a"\
	 "exi.a"\
	 "si.a"\
	 "db.a"\
	 "mtx.a"\
	 "dvd.a"\
	 "vi.a"\
	 "demo.a"\
	 "pad.a"\
	 "ai.a"\
	 "ar.a"\
	 "ax.a"\
	 "axfx.a"\
	 "mix.a"\
	 "sp.a"\
	 "am.a"\
	 "axart.a"\
	 "syn.a"\
	 "seq.a"\
	 "dsp.a"\
	 "dtk.a"\
	 "card.a"\
	 "hio.a"\
	 "mcc.a"\
	 "gx.a"\
	 "G2D.a"\
	 "texPalette.a"\
	 "fileCache.a"\
	 "support.a"\
	 "perf.a"\
	 "amcstubs.a"\
	 "odemustubs.a"\

AS=ngccc
AS_FLAGS=\
	-DSN_TARGET_NGC\
	-DHW2\
	-D__SN__\
	-DSNSYS\
	-D__HWSIM=0\
	-DEPPC\
	-DMARLIN=DI\
	-DORCA\
	-DGEKKO\
	-DGX_REV=2\
	-DBUG_CLR_LOAD_DLY\
	-DBUG_XF_STALL\
	-I"C:/DolphinSDK1.0/include"\
	-I"C:/Program Files/SN Systems/ngc/include"\
	-I. -c\
	-fdevstudio\
	-g\

CC=ngccc
CC_FLAGS=\
	-DSN_TARGET_NGC\
	-DHW2\
	-D__SN__\
	-DSNSYS\
	-D__HWSIM=0\
	-DEPPC\
	-DMARLIN=DI\
	-DORCA\
	-DGEKKO\
	-DGX_REV=2\
	-DBUG_CLR_LOAD_DLY\
	-DBUG_XF_STALL\
	-I"C:/DolphinSDK1.0/include"\
	-I"C:/Program Files/SN Systems/ngc/include"\
	-I. -c\
	-fdevstudio\
	-MMD\
	-g\
	-O0\
	-Wall\
	-fno-exceptions\

CXX_FLAGS=$(CC_FLAGS)

LD=ngccc
LD_FLAGS=\
	-L"C:/DolphinSDK1.0/HW2/lib"\
	-fdevstudio -nostartfiles -T $(LCFILE)\

OBJS=\
	"NGC_Debug/Camera.obj"\
	"NGC_Debug/main.obj"\
	"NGC_Debug/Matrix33.obj"\
	"NGC_Debug/Vector3.obj"\

COMPILE: $(OBJS)

"NGC_Debug/Camera.obj":
	@echo ProDG Compiling "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Camera.cpp"
	$(CC) $(CC_FLAGS) "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Camera.cpp" -o $@

"NGC_Debug/main.obj":
	@echo ProDG Compiling "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/main.cpp"
	$(CC) $(CC_FLAGS) "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/main.cpp" -o $@

"NGC_Debug/Matrix33.obj":
	@echo ProDG Compiling "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Matrix33.cpp"
	$(CC) $(CC_FLAGS) "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Matrix33.cpp" -o $@

"NGC_Debug/Vector3.obj":
	@echo ProDG Compiling "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Vector3.cpp"
	$(CC) $(CC_FLAGS) "g:/Visual Studio 2005/Projects/GameCube Elf3/GameCube Elf3/Vector3.cpp" -o $@

LINK: "NGC_Debug/GameCube Elf3.elf"

"NGC_Debug/GameCube Elf3.elf" : $(OBJS) $(LCFILE)
	@echo ProDG Linking $@
	$(LD) $(LD_FLAGS) --start-group -lsn -lm -lc -lgcc $(OBJS) $(LIBS) --end-group -o $@

