#include "BuffFactory.h"
#include "Buff.h"
#include "SpeedReductionDebuff.h"
#include "LaserSwapDebuff.h"
#include "../../gfxutils/ConfParser/ConfParser.h"
#include "../../utility/deleters.h"
#include "../../utility/assert.h"
using namespace std;

const char* BUFF_CLASS = "Generic";
const char* SPEED_REDUCTION_CLASS = "SpeedReduction";
const char* LASER_SWAP_CLASS = "LaserSwap";
//const char* LASER_POWER_CLASS = "LaserPower"; // uses Generic instead


BuffFactory::BuffFactory()
{
}

BuffFactory::~BuffFactory()
{
	onApplicationUnload();
}


int BuffFactory :: getTypeFromName(const std::string& name) const
{
	for (int i = 0; i != int(m_typeNames.size()); ++i)
	{
		if (m_typeNames[i] == name) return i;
	}
	assert(0);
	return -1;
}

const string& BuffFactory :: getNameFromType(int type) const
{
	return m_typeNames[type];
}


Buff* BuffFactory :: createBuff(int type) const
{
	Buff* prototype = m_buffPrototypes[type];
	assert(prototype != 0);
	return prototype->clone();
}

void BuffFactory :: onApplicationLoad(const ParserSection& ps)
{
	// get filename for enemy factory
	const ParserSection* psFiles = ps.getSection("BuffFactory");
	string filename = string("config/") + psFiles->getVal("file");

	// open enemy factory file and get data
	ConfParser cp(filename);
	const ParserSection& psRoot = cp.rootSection();
	typedef std::vector<const ParserSection *> PSVector;
	PSVector psTypes = psRoot.getChildren();

	m_buffPrototypes.resize(psTypes.size());
	m_typeNames.reserve(psTypes.size());

	// loop over all sections
	// each section is an enemy type
	// and the name should correspond to a world object type
	for (PSVector::iterator it = psTypes.begin(); it != psTypes.end(); it++)
	{
		const ParserSection* ps = *it;
		const std::string name = ps->getName();
		int type = int(m_typeNames.size());
		m_typeNames.push_back(ps->getName());
		Buff* buff = 0;
		std::string className = ps->getVal("Class");
		if (className == BUFF_CLASS) buff = new Buff(type);
		else if (className == SPEED_REDUCTION_CLASS) buff = new SpeedReductionDebuff(type);
		else if (className == LASER_SWAP_CLASS) buff = new LaserSwapDebuff(type);
		//else if (className == LASER_POWER_CLASS) buff = new LaserPowerBuff(type);
		assert(buff != 0);
		buff->loadSettings(*ps);
		m_buffPrototypes[type] = buff;
	}
}

void BuffFactory :: onApplicationUnload()
{
	deleteVector(m_buffPrototypes);
}

