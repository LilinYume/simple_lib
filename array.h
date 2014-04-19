#include <iostream>
#include <iomanip>
#include <new>
#include <cstdlib>

#ifndef SIMPLE_ARRAY
#define SIMPLE_ARRAY

namespace simple
{
	template <class type> class Array
	{
	public:
		Array()
		{
			length = size = 1;
			try {
				type_ptr = new type[ size ];
			}
			catch ( std::bad_alloc ba ) {
				exit( EXIT_FAILURE );
			}
		}
		Array ( const int set_size )
		{
			length = size = set_size;
			try {
				type_ptr = new type[ size ];
			}
			catch ( std::bad_alloc ba ) {
				exit( EXIT_FAILURE );
			}
		}
		Array( const Array& l_obj )
		{
			this->size = l_obj.size;
			this->length = l_obj.length;
			this->type_ptr = l_obj.type_ptr;
		}
		~Array()
		{
			delete [] type_ptr;
		}
		Array operator [] ( int index ){}
		Array& operator = ( Array r_obj ){}

	private:
		type *type_ptr;
		int size, length;

		private is_valid( int num ){}
	};

}
#endif
//------------------------------------------------------------------------------------------