/** @file
* @author Nathan Belue
* @date october 22, 2015
*
* I need a clean easy-to-use way to reference NBT tags, for use with the
* Schematic class.  This is it.
*
*/

#include "NBTTag.h"


/**
 * @brief Copy constructor
 * @param src	The NBTTag to copy from
 */
Redstone::NBTTag::NBTTag(
	const Redstone::NBTTag & src) :
	_type(src._type), 
	_name(src._name), 
	_listType(src._listType)
{
	if (this->_type == Type::BYTE_ARRAY) {
		auto data = reinterpret_cast<const std::vector<__int8> *>(src._vOther);
		this->_vOther = new std::vector<__int8>(*data);
	}
	else if (this->_type == Type::STRING) {
		auto data = reinterpret_cast<const std::string *>(src._vOther);
		this->_vOther = new std::string(*data);
	}
	else if (this->_type == Type::LIST) {
		auto data = reinterpret_cast<const std::vector<NBTTag> *>(src._vOther);
		this->_vOther = new std::vector<NBTTag>(*data);
	}
	else if (this->_type == Type::COMPOUND) {
		auto data = reinterpret_cast<const std::list<NBTTag> *>(src._vOther);
		this->_vOther = new std::list<NBTTag>(*data);
	}
	else if (this->_type == Type::FLOAT)
		this->_vFloat = src._vFloat;
	else if (this->_type == Type::DOUBLE)
		this->_vDouble = src._vDouble;
	else
		this->_vLong = src._vLong; // close enough...  XD
}


/**
 * @brief Assignment operator
 * @param src	The NBTTag to set from
 * @returns *this
 */
Redstone::NBTTag & Redstone::NBTTag::operator =(
	const Redstone::NBTTag & src)
{
	this->_type = src._type;
	this->_name = src._name;

	if (this->_type == Type::BYTE_ARRAY) {
		auto data = reinterpret_cast<const std::vector<__int8> *>(src._vOther);
		this->_vOther = new std::vector<__int8>(*data);
	}
	else if (this->_type == Type::STRING) {
		auto data = reinterpret_cast<const std::string *>(src._vOther);
		this->_vOther = new std::string(*data);
	}
	else if (this->_type == Type::LIST) {
		auto data = reinterpret_cast<const std::vector<NBTTag> *>(src._vOther);
		this->_vOther = new std::vector<NBTTag>(*data);
	}
	else if (this->_type == Type::COMPOUND) {
		auto data = reinterpret_cast<const std::list<NBTTag> *>(src._vOther);
		this->_vOther = new std::list<NBTTag>(*data);
	}
	else if (this->_type == Type::FLOAT)
		this->_vFloat = src._vFloat;
	else if (this->_type == Type::DOUBLE)
		this->_vDouble = src._vDouble;
	else
		this->_vLong = src._vLong; // close enough...  XD

	return *this;
}


/**
 * @brief Read in the tag from a stream
 * @param in	The istream to read from
 */
void Redstone::NBTTag::read(std::istream & in)
{
	this->_clean();
	this->_name.clear();
	this->_vOther = nullptr;

	// Read the type
	switch (in.get())
	{
	case 0: this->_type = Type::END; return;
	case 1: this->_type = Type::BYTE; break;
	case 2: this->_type = Type::SHORT; break;
	case 3: this->_type = Type::INT; break;
	case 4: this->_type = Type::LONG; break;
	case 5: this->_type = Type::FLOAT; break;
	case 6: this->_type = Type::DOUBLE; break;
	case 7: this->_type = Type::BYTE_ARRAY; break;
	case 8: this->_type = Type::STRING; break;
	case 9: this->_type = Type::LIST; break;
	case 10: this->_type = Type::COMPOUND; break;
	}

	// Read the name
	__int16 length = (in.get() << 8) + in.get();
	this->_name.reserve(length);
	for (; length > 0; --length)
		this->_name += static_cast<char>(in.get());

	// Read the payload
	this->_processPayload(in);

}


/**
 * @brief Stream the tag out
 * @param out	The ostream to write to
 */
void Redstone::NBTTag::write(
	std::ostream & out)
{
	// Write the type
	switch (this->_type) {
	case Type::END:	out.put(0); break;
	case Type::BYTE: out.put(1); break;
	case Type::SHORT: out.put(2); break;
	case Type::INT: out.put(3); break;
	case Type::LONG: out.put(4); break;
	case Type::FLOAT: out.put(5); break;
	case Type::DOUBLE: out.put(6); break;
	case Type::BYTE_ARRAY: out.put(7); break;
	case Type::STRING: out.put(8); break;
	case Type::LIST: out.put(9); break;
	case Type::COMPOUND: out.put(10); break;
	}

	// Write the name
	__int16 length = this->_name.size();
	out.put(length >> 8); out.put(length & 0xFF);
	for (auto c : this->_name)
		out.put(c);

	// Write the payload
	this->_writePayload(out);
}


/**
 * @brief Process the payload, after the type and name are read
 * @param in	The istream to read in from
 */
void Redstone::NBTTag::_processPayload(
	std::istream & in )
{
	Type subType;
	__int32 length;

	switch (this->_type)
	{

	case Type::BYTE:
		this->_vByte = in.get();
		break;

	case Type::SHORT:
		this->_vShort = in.get();
		this->_vShort = (this->_vShort << 8) | in.get();
		break;

	case Type::INT:
		this->_vInt = in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		break;

	case Type::LONG:
		this->_vLong = in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		break;

	case Type::FLOAT:
		this->_vInt = in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		this->_vInt = (this->_vInt << 8) | in.get();
		break;

	case Type::DOUBLE:
		this->_vLong = in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		this->_vLong = (this->_vLong << 8) | in.get();
		break;

	case Type::BYTE_ARRAY:
		length = in.get();
		length = (length << 8) | in.get();
		length = (length << 8) | in.get();
		length = (length << 8) | in.get();
		{
			auto p = new std::vector<__int8>();
			this->_vOther = p;
			p->reserve(length);
			for (; length > 0; --length)
				p->push_back(in.get());
		}
		break;

	case Type::STRING:
		length = in.get();
		length = (length << 8) | in.get();
		{
			auto p = new std::string();
			this->_vOther = p;
			p->reserve(length);
			for (; length > 0; --length)
				p->push_back(in.get());
		}
		break;

	case Type::LIST:
		switch (in.get()) {
		case 0: subType = Type::END; break;
		case 1: subType = Type::BYTE; break;
		case 2: subType = Type::SHORT; break;
		case 3: subType = Type::INT; break;
		case 4: subType = Type::LONG; break;
		case 5: subType = Type::FLOAT; break;
		case 6: subType = Type::DOUBLE; break;
		case 7: subType = Type::BYTE_ARRAY; break;
		case 8: subType = Type::STRING; break;
		case 9: subType = Type::LIST; break;
		case 10: subType = Type::COMPOUND; break;
		}
		length = in.get();
		length = (length << 8) | in.get();
		length = (length << 8) | in.get();
		length = (length << 8) | in.get();
		{
			auto p = new std::vector<NBTTag>();
			this->_vOther = p;
			p->reserve(length);
			for (; length > 0; --length) {
				NBTTag subTag;
				subTag._type = subType;
				subTag._processPayload(in);
			}
		}
		break;

	case Type::COMPOUND:
		{
			auto p = new std::list<NBTTag>();
			this->_vOther = p;
			NBTTag subTag;
			do {
				subTag.read(in);
				p->push_back(subTag);
			} while (subTag.getType() != Type::END);
		}
		break;

	default:
		throw std::runtime_error("Invalid file format!");
		break;

	}
}


// Write the payload
/**
 * @brief Write just the payload, not the type and name
 * @param out	The ostream to write to
 */
void Redstone::NBTTag::_writePayload(
	std::ostream & out )
{
	switch (this->_type)
	{

	case Type::BYTE:
		out.put(this->_vByte);
		break;

	case Type::SHORT:
		out.put(this->_vShort >> 8);
		out.put(this->_vShort & 0xFF);
		break;

	case Type::INT:
		out.put(this->_vInt >> 24);
		out.put((this->_vInt >> 16) & 0xFF);
		out.put((this->_vInt >> 8) & 0xFF);
		out.put(this->_vInt & 0xFF);
		break;

	case Type::LONG:
		out.put(this->_vLong >> 56);
		out.put((this->_vLong >> 48) & 0xFF);
		out.put((this->_vLong >> 40) & 0xFF);
		out.put((this->_vLong >> 32) & 0xFF);
		out.put((this->_vLong >> 24) & 0xFF);
		out.put((this->_vLong >> 16) & 0xFF);
		out.put((this->_vLong >> 8) & 0xFF);
		out.put(this->_vLong & 0xFF);
		break;

	case Type::FLOAT:
		out.put(this->_vInt >> 24);
		out.put((this->_vInt >> 16) & 0xFF);
		out.put((this->_vInt >> 8) & 0xFF);
		out.put(this->_vInt & 0xFF);
		break;

	case Type::DOUBLE:
		out.put(this->_vLong >> 56);
		out.put((this->_vLong >> 48) & 0xFF);
		out.put((this->_vLong >> 40) & 0xFF);
		out.put((this->_vLong >> 32) & 0xFF);
		out.put((this->_vLong >> 24) & 0xFF);
		out.put((this->_vLong >> 16) & 0xFF);
		out.put((this->_vLong >> 8) & 0xFF);
		out.put(this->_vLong & 0xFF);
		break;

	case Type::BYTE_ARRAY:
	{
		auto p = this->vByteArray();
		out.put((p->size() >> 24) & 0xFF);
		out.put((p->size() >> 16) & 0xFF);
		out.put((p->size() >> 8) & 0xFF);
		out.put(p->size() & 0xFF);
		for (auto v : *p)
			out.put(v);
	}
	break;

	case Type::STRING:
	{
		auto p = this->vString();
		out.put((p->length() >> 8) & 0xFF);
		out.put(p->length() & 0xFF);
		for (auto v : *p)
			out.put(v);
	}
	break;

	case Type::LIST:
		switch (this->_listType) {
		case Type::END: out.put(0); break;
		case Type::BYTE: out.put(1); break;
		case Type::SHORT: out.put(2); break;
		case Type::INT: out.put(3); break;
		case Type::LONG: out.put(4); break;
		case Type::FLOAT: out.put(5); break;
		case Type::DOUBLE: out.put(6); break;
		case Type::BYTE_ARRAY: out.put(7); break;
		case Type::STRING: out.put(8); break;
		case Type::LIST: out.put(9); break;
		case Type::COMPOUND: out.put(10); break;
		}
		{
			auto p = this->vList();
			out.put((p->size() >> 24) & 0xFF);
			out.put((p->size() >> 16) & 0xFF);
			out.put((p->size() >> 8) & 0xFF);
			out.put(p->size() & 0xFF);
			for (auto v : *p)
				v._writePayload(out);
		}
		break;

	case Type::COMPOUND:
	{
		auto p = this->vCompound();
		for (auto v : *p)
			v.write(out);
		if (p->back().getType() != Type::END)
			NBTTag().write(out);
	}
	break;

	}
}


/* A bunch of annoying simple functions */

__int8 Redstone::NBTTag::vByte() const
{
	if (this->_type != Type::BYTE)
		throw std::runtime_error("Invalid return type!");
	return this->_vByte;
}

__int16 Redstone::NBTTag::vShort() const
{
	if (this->_type != Type::SHORT)
		throw std::runtime_error("Invalid return type!");
	return this->_vShort;
}

__int32 Redstone::NBTTag::vInt() const
{
	if (this->_type != Type::INT)
		throw std::runtime_error("Invalid return type!");
	return this->_vInt;
}

__int64 Redstone::NBTTag::vLong() const
{
	if (this->_type != Type::LONG)
		throw std::runtime_error("Invalid return type!");
	return this->_vLong;
}

float Redstone::NBTTag::vFloat() const
{
	if (this->_type != Type::FLOAT)
		throw std::runtime_error("Invalid return type!");
	return this->_vFloat;
}

double Redstone::NBTTag::vDouble() const
{
	if (this->_type != Type::DOUBLE)
		throw std::runtime_error("Invalid return type!");
	return this->_vDouble;
}

const std::vector<__int8> * Redstone::NBTTag::vByteArray() const
{
	if (this->_type != Type::BYTE_ARRAY)
		throw std::runtime_error("Invalid return type!");
	return reinterpret_cast<const std::vector<__int8> *>(this->_vOther);
}

const std::string * Redstone::NBTTag::vString() const
{
	if (this->_type != Type::STRING)
		throw std::runtime_error("Invalid return type!");
	return reinterpret_cast<const std::string *>(this->_vOther);
}

const std::vector<Redstone::NBTTag> * Redstone::NBTTag::vList() const
{
	if (this->_type != Type::LIST)
		throw std::runtime_error("Invalid return type!");
	return reinterpret_cast<const std::vector<Redstone::NBTTag> *>(this->_vOther);
}

const std::list<Redstone::NBTTag> * Redstone::NBTTag::vCompound() const
{
	if (this->_type != Type::COMPOUND)
		throw std::runtime_error("Invalid return type!");
	return reinterpret_cast<const std::list<Redstone::NBTTag> *>(this->_vOther);
}

void Redstone::NBTTag::vByte(__int8 val)
{
	this->_clean();
	this->_vByte = val;
	this->_type = Type::BYTE;
}

void Redstone::NBTTag::vShort(__int16 val)
{
	this->_clean();
	this->_vShort = val;
	this->_type = Type::SHORT;
}

void Redstone::NBTTag::vInt(__int32 val)
{
	this->_clean();
	this->_vInt = val;
	this->_type = Type::INT;
}

void Redstone::NBTTag::vLong(__int64 val)
{
	this->_clean();
	this->_vLong = val;
	this->_type = Type::LONG;
}

void Redstone::NBTTag::vFloat(float val)
{
	this->_clean();
	this->_vFloat = val;
	this->_type = Type::FLOAT;
}

void Redstone::NBTTag::vDouble(double val)
{
	this->_clean();
	this->_vDouble = val;
	this->_type = Type::DOUBLE;
}

void Redstone::NBTTag::vByteArray(const std::vector<__int8> & val)
{
	this->_clean();
	this->_vOther = new std::vector<__int8>(val);
	this->_type = Type::BYTE_ARRAY;
}

void Redstone::NBTTag::vString(const std::string & val)
{
	this->_clean();
	this->_vOther = new std::string(val);
	this->_type = Type::STRING;
}

void Redstone::NBTTag::vList(const std::vector<Redstone::NBTTag> & val)
{
	this->_clean();
	this->_vOther = new std::vector<Redstone::NBTTag>(val);
	this->_type = Type::LIST;
}

void Redstone::NBTTag::vCompound(const std::list<Redstone::NBTTag> & val)
{
	this->_clean();
	this->_vOther = new std::list<Redstone::NBTTag>(val);
	this->_type = Type::COMPOUND;
}

