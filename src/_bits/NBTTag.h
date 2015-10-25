/** @file
* @author Nathan Belue
* @date october 22, 2015
*
* I need a clean easy-to-use way to reference NBT tags, for use with the 
* Schematic class.  This is it.
*
*/

#ifndef REDSTONE_BITS_NBTTAG_H
#define REDSTONE_BITS_NBTTAG_H

#include <list>
#include <stdexcept>
#include <string>
#include <vector>


/* Redstone namespace */
namespace Redstone
{


	/**
	 * @brief NBTTag struct
	 */
	struct NBTTag
	{

		/**
		 * @brief Types of data
		 */
		enum class Type {
			END, BYTE, SHORT, INT, LONG, FLOAT, DOUBLE,
			BYTE_ARRAY, STRING, LIST, COMPOUND
		};


		/* Data Access */
		
		__int8 vByte() const;
		__int16 vShort() const;
		__int32 vInt() const;
		__int64 vLong() const;
		float vFloat() const;
		double vDouble() const;
		const std::vector<__int8> * vByteArray() const;
		const std::string * vString() const;
		const std::vector<NBTTag> * vList() const;
		const std::list<NBTTag> * vCompound() const;

		void vByte(__int8);
		void vShort(__int16);
		void vInt(__int32);
		void vLong(__int64);
		void vFloat(float);
		void vDouble(double);
		void vByteArray(const std::vector<__int8> &);
		void vString(const std::string &);
		void vList(const std::vector<NBTTag> &);
		void vCompound(const std::list<NBTTag> &);


		/* Public functions */

		/**
		 * @brief Constructor
		 */
		NBTTag() : _type(Type::END), _vOther(nullptr) {}

		/**
		 * @brief Copy constructor
		 * @param src	The NBTTag to copy from
		 */
		NBTTag(const NBTTag & src);

		/**
		 * @brief Destructor
		 */
		~NBTTag()
		{
			this->_clean();
		}

		/**
		* @brief Assignment operator
		* @param src	The NBTTag to set from
		* @returns *this
		*/
		NBTTag & operator =(const NBTTag & src);

		/**
		 * @brief Get the type of data it holds
		 * @returns The type of data it holds
		 */
		Type getType() const
		{
			return this->_type;
		}

		/**
		 * @brief Set the name of the tag
		 * @param name	The name of the tag
		 */
		void setName(const std::string & name)
		{
			this->_name = name;
		}

		/**
		 * @brief Get the name of the tag
		 * @returns The name
		 */
		const std::string & getName() const
		{
			return this->_name;
		}

		/**
		 * @brief Set the list type
		 * @param type	The type of the list
		 */
		void setListType(Type type)
		{
			this->_listType = type;
		}

		/**
		 * @brief Get the list type
		 * @returns The type of the list
		 */
		Type getListType() const
		{
			return this->_listType;
		}

		/**
		* @brief Stream the tag in
		* @param in	The istream to read from
		*/
		void read(std::istream & in);

		/**
		 * @brief Stream the tag out
		 * @param out	The ostream to write to
		 */
		void write(std::ostream & out);


	private:

		/* Helper functions */

		/**
		 * @brief Clean up the data
		 */
		void _clean()
		{
			if (_type == Type::BYTE_ARRAY)
				delete reinterpret_cast<std::vector<__int8> *>(_vOther);
			else if (_type == Type::STRING)
				delete reinterpret_cast<std::string *>(_vOther);
			else if (_type == Type::LIST)
				delete reinterpret_cast<std::vector<NBTTag> *>(_vOther);
			else if (_type == Type::COMPOUND)
				delete reinterpret_cast<std::list<NBTTag> *>(_vOther);
		}

		/**
		 * @brief Process just the payload, after type and name are read
		 * @param in	The istream to read from
		 */
		void _processPayload(std::istream & in);

		/**
		 * @brief Write just the payload, not the type and name
		 * @param out	The ostream to write to
		 */
		void _writePayload(std::ostream & out);


	private:

		/* Private Data */
		
		Type _type;
		std::string _name;

		union {
			__int8 _vByte;
			__int16 _vShort;
			__int32 _vInt;
			__int64 _vLong;
			float _vFloat;
			double _vDouble;
			void * _vOther;
		};

		Type _listType; // For lists only


	};


}


#endif
