inline VBOMgr * VBOMgr :: instance()
{
	assert(_init);
	return _instance;
}