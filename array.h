#include <iostream>
#include <iomanip>
#include <new>
#include <stdexcept>
#include <cstdlib>

#ifndef SIMPLE_ARRAY
#define SIMPLE_ARRAY

namespace simple
{
	template <class type> class Array
	{
	private:
		type *type_ptr;
		int size;
	public:
		/* コンストラクタ
			引数に指定したサイズで動的割り当て
			private メンバの型へのポインタへ割り当てる。
			この時指定されたサイズをフィールド変数"size"(int) へ保持			*/
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
		/* コピーコンストラクタ												*/
		Array( const Array &l_obj )
		{
			// コピーコンストラクタの引数に定義済みArray型が渡されたとき用
			// 代入先のArrayオブジェクトの方が小さければ再割当て。
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

			// メンバの値シーケンスをコピー
			for( int i = 0; i < this-> size; ++i ) {
				this-> type_ptr[i] = l_obj.type_ptr[i];
			}
		}
		/* [] 演算子オーバーロード
			添字アクセスによる値の設定と取得								*/
		type& operator [] ( int index ) 
		{
			if ( index < 0 ) throw std::invalid_argument("");

			if ( this-> size - 1 < index ) {
				throw std::out_of_range("");
			}
			return this-> type_ptr[ index ];
		}
		/* = 演算子オーバーロード
			Array型のインスタンス・メンバをコピー							*/
		Array& operator = ( Array& r_obj )
		{
			// 代入先のサイズが小さければ再割当て
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

			// メンバの値シーケンスをコピー
			for( int i = 0; i < this-> size; ++i ) {
				this-> type_ptr[i] = r_obj.type_ptr[i];
			}
			return *this;
		}
		/* デストラクタ													*/
		~Array() 
		{
			delete [] type_ptr;
		}
	};
}
#endif
//------------------------------------------------------------------------dummy