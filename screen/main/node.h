#ifndef NODE_H
#define NODE_H
#include "core/object/object.h"
#include "imnodes.h"
#include <string>
#include <vector>

class Node : Object {
	virtual void SetLocalStyle() {}
	virtual void OnUIRender(bool* open) {}

	void Show() { m_open = true; }
	void Hide() { m_open = false; }
	void Toggle() { m_open = !m_open; }
	bool Visibility() { return m_open; }
	
	void SetName(const std::string& name) { m_Name = name; }
	const std::string GetName() { return m_Name; }
	
	bool* p_open = &m_open;
private:
	bool m_open = false;
	
	std::string m_Name = "Node";
};

#endif