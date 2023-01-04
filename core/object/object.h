#ifndef OBJECT_H
#define OBJECT_H
#include "object_id.h"

class Object {
public:
	Object();
	ObjectID GetID()  const { return id; }
private:
	ObjectID id;
};

class ObjectDB {
// This needs to add up to 63, 1 bit is for reference.
#define OBJECTDB_VALIDATOR_BITS 39
#define OBJECTDB_VALIDATOR_MASK ((uint64_t(1) << OBJECTDB_VALIDATOR_BITS) - 1)
#define OBJECTDB_SLOT_MAX_COUNT_BITS 24
#define OBJECTDB_SLOT_MAX_COUNT_MASK ((uint64_t(1) << OBJECTDB_SLOT_MAX_COUNT_BITS) - 1)
#define OBJECTDB_REFERENCE_BIT (uint64_t(1) << (OBJECTDB_SLOT_MAX_COUNT_BITS + OBJECTDB_VALIDATOR_BITS))

	struct ObjectSlot { // 128 bits per slot.
		uint64_t validator : OBJECTDB_VALIDATOR_BITS;
		uint64_t next_free : OBJECTDB_SLOT_MAX_COUNT_BITS;
		uint64_t is_ref_counted : 1;
		Object *object = nullptr;
	};

	static uint32_t slot_count;
	static uint32_t slot_max;
	static ObjectSlot *object_slots;
	static uint64_t validator_counter;
	friend class Object;

	static ObjectID add_instance(Object *p_object);
	static void remove_instance(Object *p_object);

public:
	static Object *get_instance(ObjectID p_instance_id) {
		uint64_t id = p_instance_id;
		uint32_t slot = id & OBJECTDB_SLOT_MAX_COUNT_MASK;

		uint64_t validator = (id >> OBJECTDB_SLOT_MAX_COUNT_BITS) & OBJECTDB_VALIDATOR_MASK;

		if (object_slots[slot].validator != validator) {
			return nullptr;
		}

		Object *object = object_slots[slot].object;

		return object;
	}
};

#endif