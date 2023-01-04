#include "object.h"
#include <stdint.h>
#include <xhash>
#include <random>

static std::random_device s_RandomDevice;
static std::mt19937_64 eng(s_RandomDevice());
static std::uniform_int_distribution<uint64_t> s_UniformDistribution;

Object::Object()
{
	id = ObjectDB::add_instance(this);
}

ObjectID ObjectDB::add_instance(Object *p_object) {
	uint64_t id = s_UniformDistribution(eng);
	return ObjectID(id);
}

void ObjectDB::remove_instance(Object *p_object) {
}