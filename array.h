/*
	境界検証をする配列
	(大幅に見直し予定)
*/

#include <iostream>
#include <iomanip>
#include <new>
#include <typeinfo>
#include <cstdlib>

#ifndef SIMPLE
#define SIMPLE

namespace simple
{
	// 例外用クラス
	/* コンストラクタ引数にとるエラーメッセージを出力し
	   プログラムを終了させる。									 */
	class index_err
	{
	public:
		explicit index_err( const char* msg )
		{
			std::cerr << msg << std::endl;
			exit( EXIT_FAILURE );
		}
	};

	//	汎用配列クラス
	/* ・要素数はコンストラクタに指定した個数分
		境界検証をする												 */
	template <class T> class Array 
	{
	private:
		int count;
		const int set_size;
		T* data;

		// valid_index_range関数
		/*	働き：配列の添字アクセスに利用する値が正当であるか判定する
			負の数ではないことと、整数であることをチェック。
			引数：調べたいインデックス(int)
			戻値：なし														*/
		void valid_index_range( int target_idx )
		{
			int type_int = 0;
			// 正の数がチェック
			if ( target_idx < 0 ) {
				throw index_err( "Size is less then zero" );
			}
			// 引数がintかチェック
			if ( typeid( type_int ) != typeid( target_idx ) ) {
				throw index_err( "Type Err: argument is not integer" );
			}
		}
		// defined_range関数
		/*	働き：配列の境界検証を行う
			引数：チェックしたい配列のインデックス（int）
			戻値：なし														*/
		void defined_range( int target_idx )
		{
			if ( 0 > target_idx || target_idx >= set_size ) {
				throw index_err( "index outbound" );
			}
		}
	public:
		// コンストラクタ
		/* 働き：サイズ1の要素を定義する メンバ変数の初期化			*/
		Array() : set_size( 1 )
		{
			count = 0;
			data = NULL;
			try {
				data = new T [ set_size ];
			}
			catch ( std::bad_alloc ba ) {
				std::cerr << "Allocation failed\n";
			}
			catch ( index_err idxErr ){ }
		}
		
		// コンストラクタ オーバロード
		/*	働き：引数に取った数の要素数をnew演算で確保する
				   メンバ変数の初期化 
			引数：ゼロ以上の正の整数								 */
		explicit Array( const int size ) : set_size( size )
		{
			count = 0;
			data = NULL;
			try {
				valid_index_range( size ); 
				data = new T [ set_size ];
			}
			catch ( std::bad_alloc ba ) {
				std::cerr << "Allocation failed\n";
			} 
			catch ( index_err idxErr ){ }
		}
		// コピーコンストラクタ
		/* クラスオブジェクトのコピーを作成する					 */ 
		Array( const Array& ob ) : set_size( this-> set_size )
		{
			count = 0;
			data = NULL;
			try {
				valid_index_range( ob.set_size ); 
				data = new T [ ob.set_size ];
			}
			catch ( std::bad_alloc ba ) {
				std::cerr << "Allocation failed\n";
			}
			catch ( index_err idxErr ){ }

			// 引数オブジェクトの保持するメンバからコピー
			for( int i = 0; i < set_size; ++i ) {
				this-> data[i] = ob.data[i];
			}
		}
		// デストラクタ
		/* コンストラクタでnewした領域を解放する */
		~Array() { if ( data ) delete [] data; }
		
		// put関数 
		/*	働き：外部から1つのデータをクラスメンバに格納する
				  データを格納したらカウンタ（インデックス）を一つ進める
				  配列の先頭から格納
			引数：実行時に特定されるT型
			戻値：なし													*/
		void put( T arg )
		{
			int index = 0;

			if ( count < set_size ) index = count;
			data[ index ] = arg;
			++count;
		}
		// pull関数
		/*	働き：put関数によってメンバに格納されている値を取り出す
				   データを取り出しても取り出したデータは破棄しない
				   取り出し位置は先頭のみ。
			引数：なし
			戻値：T 型														 */
		T pull( )
		{
			if ( 0 < set_size ) {
				return data[ 0 ]; // 先頭要素のみ返す
			}
		}
		// pull関数 オーバロード
		/*	働き：引数により指定されたインデックスの指し示す要素を返す
			引数：取得したい配列ののインデックス( 正のint )
			戻値：T型														 */
		T pull( int index )
		{
			try {
				// indexがゼロ以上でint型かチェック
				valid_index_range( index );
				// indexが定義済みの範囲かチェック
				defined_range( index );
			}
			catch( index_err idxErr ){ }
			return data[ index ];
		}
		// size関数
		/*	働き：配列の要素数を返す
			引数：なし
			戻値：要素数（int）*/
		int size() const { return set_size; }

		const T operator [] ( int index ) const
		{
			try {
				// indexがゼロ以上でint型かチェック
				valid_index_range( index );
				// indexが定義済みの範囲かチェック
				defined_range( index );
			}
			catch( index_err idxErr ){ }
			return data[ index ];
		}
		T& operator [] ( int index )
		{
			try {
				// indexがゼロ以上でint型かチェック
				valid_index_range( index );
				// indexが定義済みの範囲かチェック
				defined_range( index );
			}
			catch( index_err idxErr ){ }
			return data[ index ];
		}
		Arrary& operator = ( Array& ob )
		{
			if( this-> size() <= ob.size() ) {
				throw index_err("Can not copy to samll case\n");
			}
			else {
				// this-> data[0:Tail] = ob.data[0:Tail];
			}
		}
	};
}
#endif
//------------------------------------------------------------------------------------------