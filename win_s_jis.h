/*
概要
	ms932をディクス・ファイルの入出力用に利用する。

	符号化はShift-Jis
	SI/SOを利用しない。1Byteまたは2Byteの固定長をメモリ内表現に利用。
	メモリ内では2Byte分の領域をshort型とchar型2個の配列で出来た共用体で実現する。
	ストリームバッファに流すバイトシーケンスは下位1Byte → 上位1Byteの順。
	外部のディクス・ファイルには各1Byteをフォーマッティングし文字表現とする。
*/

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <climits>
#include <cstdlib>

/* 符号なしchar型 最大は255 (1Byte)であり　1Byteは8bitである必要あり。
	参考	CHAR_BIT : 8	 UCHAR_MAX : 0xff								*/

#define BYTE_SIZE 1
#define CHAR_SIZE BYTE_SIZE
#define WCHAR_SIZE (BYTE_SIZE * 2)

/*	Windows Japanese_Japan
	
	半角文字の範囲
		[0x21:0xDF] - 半角記号 ｶﾀｶﾅ 英数字(1Byte)

	全角文字の範囲
		[0x8041:0xFC4B] - 全角文字(2Byte)
	
	ひらがな 
		コードの範囲：0x829F ～ 82F1
	照合順序 
		各行につき 促音 -> 清音 -> 濁音 ->　半濁音

		ぁあぃいぅうぇえお
		かがきぎくぐけげこご
		さざしじすずせぜそぞ
		ただちぢっつづてでとど
		なにぬねの
		はばぱひびぴふぶぷへべぺほぼぽ
		まみめも
		ゃやゅゆょよ
		らりるれを
		ゎわゐゑをん

	全角カタカナ
		コードの範囲： 0x8340 ～ 8396
	 照合順序
		各行につき 促音 -> 清音 -> 濁音 ->　半濁音 (ヴヵヶは例外)
		
		ァアィイゥウェエォオ
		カガキギクグケゲコゴ
		サザシジスズセゼソゾ
		タダチヂッツヅテデトド
		ナニヌネノ
		ハバパヒピフブプヘベペホボポ
		マミムメモ
		ャヤュユョヨ
		ラリルレロ
		ヮワヰヱヲン
		ヴヵヶ															*/

/*	チェック・リスト or TODO
	・例外が発生したときの対処
	・エンディアンの考慮(？)
	・細かいチェック
*/

namespace ms932
{
	typedef unsigned char u_char;
	typedef unsigned short u_short;

	class Err
	{	public: Err( const char* msg )	{ std::cerr << msg << '\n'; }	};

	enum { first, second };
	
	/* wide_char 共用体
		char型配列2個(2Byte) / short(2Byte) 各符号なし
		char配列の先頭要素： Shift-JISの上位1Byte (位の大きい方)
		char配列の末尾素： JISコードまたはshift-JISの下位1Byte (位の小さい方)					*/
	union wide_char
	{
	public:
		u_char byte[ WCHAR_SIZE ];
		u_short code;

		// コンストラクタ
		explicit wide_char()
		{
			byte[ first ] = 0;
			byte[ second ] = 0;
			code = 0;
			check();
		}
		// コンストラクタ オーバーロード ( 宣言と共に代入が行われる用 )
		wide_char( u_short set_code )
		{
			byte[ first ] = set_code;
			byte[ second ] = set_code;
			code = set_code;
			check();
		}
		// コピーコンストラクタ
		wide_char( const wide_char& ob )
		{
			this-> byte[ first ] = ob.byte[ first ];
			this-> byte[ second ] = ob.byte[ second ];
			this-> code = ob.code;
		}
		// charが1Byte shortが2Byteかチェック
		void check()
		{
			if ( sizeof ( char ) != CHAR_SIZE ) throw Err( "size err" );
			if ( sizeof ( short ) != WCHAR_SIZE ) throw Err( "size err" );
		}
		/* 代入演算子のオーバーロード
			引数：文字コードに相当する整数値(unsigned short)	*/ 
		wide_char& operator = ( u_short set_code ) 
		{
			this-> code = set_code;
			return *this;
		}
	};
	/* jis クラス
		JISコード相当 [0x21:0xDF] をあつかう														*/
	class jis
	{
	private:
		u_char letter;
		u_short code;
		const int empty;
	public:
		// コンストラクタ
		jis() : empty( 0 )
		{
			letter = empty;
			code = empty;
		}
		// コピーコンストラクタ
		jis( const jis& ob ) : empty( 0 )
		{
			this-> letter = ob.letter;
			this-> code = ob.code;
		}
		/* range_check関数
			目的：JISコードの範囲にあるかチェック
			戻値：妥当な範囲であるか否かを表す真偽値					*/
		bool range_check( u_char ch )
		{
			if ( ch < 0x21 || ch > 0xdf ) {
				throw Err( "range err" );
 				return false;
			}
			return true;
		}
		/*	代入演算子オーバーロード
			 Jisクラスのオブジェクトをコピーする際
			 右辺が独自のメモリ領域を持つことを保証							*/
		jis& operator = ( jis ob )
		{
			*this = ob;
			return *this;
		}
		/* format_put関数
			目的：文字としてフォーマッティングされたJisコードを出力
			引数：第一引数の出力ストリームの参照へ文字コードを渡す
			戻値：Jisコードとして妥当な範囲にあることを表す真偽値			*/
		bool format_put( std::ostream& stream, wide_char jis_char ) {
			if ( range_check( jis_char.byte[ first ] ) ) {
				stream << jis_char.byte[ first ];
				return true;
			}
			return false;
		}
		/* binary_put関数
			目的：フォーマッティングされない文字コードの整数値を出力
			引数：第一引数の出力ストリームの参照へ文字コードを渡す
			戻値：Jisコードとして妥当な範囲にあることを表す真偽値			*/
		bool binary_put( std::ostream& stream, wide_char jis_char ) {
			if ( range_check( jis_char.byte[ first ] ) ) {
				stream << ( static_cast<int>( jis_char.byte[ first ]) );
				return true;
			}
			return false;
		}
		friend std::ostream& operator << ( std::ostream& , jis );
		friend std::istream& operator >> ( std::istream& , jis );
	};
	// Jisコードに相当する文字をストリームに出力
	std::ostream& operator << ( std::ostream& stream, jis &ob )
	{
		stream << ob.letter;
		return stream;
	}
	// Jisコードに相当する文字をストリームから入力
	std::istream& operator >> ( std::istream& stream, jis &ob )
	{
		stream >> ob.letter;

		if( stream.fail() ) {
			std::cerr << "Reading Fail\n";
			stream.clear();
		}
		return stream;
	}
	/* s_jis クラス
		[0x8041:0xFC4B] - 全角文字(2Byte) [ 0x[80:fc] [40:fc] ]
		Shift-Jis の全角文字の範囲を扱う														*/
	class s_jis
	{
	private:
		std::locale loc_jpn;
		wide_char letter;
		const int empty;
	public:
		// コンストラクタ
		s_jis() : empty( 0 )
		{
			// ロケールを日本語に設定
			loc_jpn =  std::locale( "Japanese_Japan" );
			letter.code = empty;
			letter.byte[ first ] = empty;
			letter.byte[ second ] = empty;
		}
		// コピーコンストラクタ
		s_jis( const s_jis& ob ) : empty( 0 )
		{
			this-> loc_jpn = ob.loc_jpn;
			this-> letter.code = ob.letter.code;
			this-> letter.byte[ first ] = ob.letter.byte[ first ];
			this-> letter.byte[ second ] = ob.letter.byte[ second ];
		}
		// Shift-Jisコード相当のshort int をコピー
		s_jis& operator = ( s_jis ob ) 
		{
			*this = ob;
			return *this;
		}
		bool format_put( std::ostream& stream, wide_char sjis_char ) {
			if ( range_check( sjis_char.code ) ) {
				stream << sjis_char.byte[ second ] << sjis_char.byte[ first ];
				return true;
			}
			return false;
		}
		bool binary_put( std::ostream& stream, wide_char sjis_char ) {
			if ( range_check( sjis_char.code ) ) {
				stream << static_cast<u_short>( sjis_char.code );
				return true;
			}
			return false;
		}
		// wide_char共用体型メンバ値がShift-Jisコード範囲かチェック
		bool range_check( u_short target )
		{
			letter.code = target;
			// Shit-Jisの2Byte分全体(short int)
			if ( this-> letter.code < 0x8041
				|| this-> letter.code > 0xfc4b ) {
				throw Err( "invaild range" );
				return false;
			}
			// 上位1Byteのchar配列
			if ( this-> letter.byte[ second ] < 0x80 
				|| this-> letter.byte[ second ] > 0xfc ) {
				throw Err( "invaild range" );
				return false;
			}
			// 下位1Byteのchar配列
			if ( this-> letter.byte[ first ] < 0x40 
				|| this-> letter.byte[ first ] > 0xfc ){
				throw Err( "invaild range" );
				return false;
			}
			return true;
		}
		friend std::ostream& operator << ( std::ostream& , s_jis& );
		friend std::istream& operator >> ( std::istream& , s_jis& );
	};
	// Shift-Jisコードを対応する文字相当で出力
	std::ostream& operator << ( std::ostream& stream, s_jis &ob )
	{
		stream.imbue( ob.loc_jpn );
		stream << ob.letter.byte[ second ] << ob.letter.byte[ first ];
		return stream;
	}
	std::istream& operator >> ( std::istream& stream, s_jis &ob )
	{
		stream.imbue( ob.loc_jpn );
		stream >> ob.letter.byte[ second ] >> ob.letter.byte[ first ];
		
		if ( stream.fail() ) {
			std::cerr << "Reading Fail\n";
			stream.clear();
		}
		return stream;
	}
} /* ms932 namespace End */