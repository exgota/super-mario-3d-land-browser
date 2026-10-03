namespace
{
struct UnknownObject
{
	char padding[0x9c];
	signed char value;
};
}

extern "C" signed char fn_003284A4(UnknownObject* self)
{
	return self->value;
}
