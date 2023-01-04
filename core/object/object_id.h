#ifndef OBJECT_ID_H
#define OBJECT_ID_H
#include <cstdint>

class ObjectID
{
public:
	inline bool is_valid() const { return id != 0; }
	inline bool is_null() const { return id == 0; }
	inline operator uint64_t() const { return id; }
	inline operator int64_t() const { return id; }
	
	inline bool operator==(const ObjectID &p_id) const { return id == p_id.id; }
	inline bool operator!=(const ObjectID &p_id) const { return id != p_id.id; }
	inline bool operator<(const ObjectID &p_id) const { return id < p_id.id; }

	inline void operator=(int64_t p_int64) { id = p_int64; }
	inline void operator=(uint64_t p_uint64) { id = p_uint64; }

	inline ObjectID() {}
	inline explicit ObjectID(const uint64_t p_id) { id = p_id; }
	inline explicit ObjectID(const int64_t p_id) { id = p_id; }
private:
	uint64_t id = 0;
};

#endif