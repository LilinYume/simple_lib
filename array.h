#ifndef SIMPLE_ARRAY
#define SIMPLE_ARRAY

#include <iostream>
#include <iomanip>
#include <new>
#include <stdexcept>
#include <cstdlib>

namespace simple
{
	template <class type> class Array
	{
	private:
		type *type_ptr;
		unsigned int size;
	public:
		/* �R���X�g���N�^
			�����Ɏw�肵���T�C�Y�œ��I���蓖��
			private �����o�̌^�ւ̃|�C���^�֊��蓖�Ă�B
			���̎��w�肳�ꂽ�T�C�Y���t�B�[���h�ϐ�"size"(int) �֕ێ�			*/
		explicit Array( const int set_size ) : size ( set_size )
		{
			try {
				if ( size < 0 ) throw std::invalid_argument("");
				type_ptr = new type[ size ]; 
			}
			catch ( std::invalid_argument bad_arg ) {
				std::cerr << "Err: index less than zero\n";
				exit( EXIT_FAILURE );
			}
			catch ( std::bad_alloc alloc_fail )	{
				std::cerr << "Err: allocation failed\n";
				exit( EXIT_FAILURE );
			}
		}
		/* �R�s�[�R���X�g���N�^													*/
		Array( const Array &l_obj )
		{
			// �R�s�[�R���X�g���N�^�̈����ɒ�`�ς�Array�^���n���ꂽ�Ƃ��p
			// ������Array�I�u�W�F�N�g�̕�����������΍Ċ����āB
			if ( this-> size < l_obj.size ) {
				try {
					this-> type_ptr = new type [ l_obj.size ];
				}
				catch ( std::bad_alloc alloc_fail )	{
					std::cerr << "Err: allocation failed\n";
					exit( EXIT_FAILURE );
				}
			}
			this-> size = l_obj.size;

			// �����o�̒l�V�[�P���X���R�s�[
			for( int i = 0; i < this-> size; ++i ) {
				this-> type_ptr[i] = l_obj.type_ptr[i];
			}
		}
		/* [] ���Z�q�I�[�o�[���[�h
			�Y���A�N�Z�X�ɂ��l�̐ݒ�Ǝ擾										*/
		type& operator [] ( int index ) 
		{
			if ( index < 0 ) throw std::invalid_argument("");

			if ( this-> size - 1 < index ) {
				throw std::out_of_range("");
			}
			return this-> type_ptr[ index ];
		}
		/* ������Z�q�I�[�o�[���[�h
			Array�^�̃C���X�^���X�E�����o���R�s�[								*/
		Array& operator = ( Array& r_obj )
		{
			// �����̃T�C�Y����������΍Ċ�����
			if ( this-> size < r_obj.size ) {
				try {
					this-> type_ptr = new type [ r_obj.size ];
				}
				catch ( std::bad_alloc alloc_fail )	{
					std::cerr << "Err: allocation failed\n";
					exit( EXIT_FAILURE );
				}
			}
			this-> size = r_obj.size;

			// �����o�̒l�V�[�P���X���R�s�[
			for( int i = 0; i < this-> size; ++i ) {
				this-> type_ptr[i] = r_obj.type_ptr[i];
			}
			return *this;
		}
		/* �f�X�g���N�^															*/
		~Array() 
		{
			delete [] type_ptr;
		}
		friend int length( Array& );
	};
}
#endif