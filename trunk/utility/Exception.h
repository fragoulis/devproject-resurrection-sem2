#pragma once
#include <string>


class Exception
{
public:
	Exception() : m_text("Unknown exception") { }
	Exception(const std::string& text) : m_text(text) { }
	virtual ~Exception() { }

	std::string getText() const { return m_text; }
	void setText(const std::string& text) { m_text = text; }

private:
	std::string m_text;
};
