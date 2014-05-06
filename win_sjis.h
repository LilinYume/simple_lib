/*
�T�v
	ms932���f�B�N�X�E�t�@�C���̓��o�͗p�ɗ��p����B

	��������Shift-Jis
	SI/SO�𗘗p���Ȃ��B1Byte�܂���2Byte�̌Œ蒷�����������\���ɗ��p�B
	���������ł�2Byte���̗̈��short�^��char�^2�̔z��ŏo�������p�̂Ŏ�������B
	�X�g���[���o�b�t�@�ɗ����o�C�g�V�[�P���X�͉���1Byte �� ���1Byte�̏��B
	�O���̃f�B�N�X�E�t�@�C���ɂ͊e1Byte���t�H�[�}�b�e�B���O�������\���Ƃ���B
*/

#pragma once

#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <climits>
#include <cstdlib>

/* �����Ȃ�char�^ �ő��255 (1Byte)�ł���@1Byte��8bit�ł���K�v����B
	�Q�l	CHAR_BIT : 8	 UCHAR_MAX : 0xff								*/

#define BYTE_SIZE 1
#define CHAR_SIZE BYTE_SIZE
#define WCHAR_SIZE (BYTE_SIZE * 2)

/*	Windows Japanese_Japan
	
	���p�����͈̔�
		[0x21:0xDF] - ���p�L�� ���� �p����(1Byte)

	�S�p�����͈̔�
		[0x8041:0xFC4B] - �S�p����(2Byte)
	
	�Ђ炪�� 
		�R�[�h�͈̔́F0x829F �` 82F1
	�ƍ����� 
		�e�s�ɂ� ���� -> ���� -> ���� ->�@������

		������������������
		��������������������
		��������������������
		�����������ÂĂłƂ�
		�Ȃɂʂ˂�
		�͂΂ςЂт҂ӂԂՂւׂ؂قڂ�
		�܂݂߂�
		�������
		������
		��������

	�S�p�J�^�J�i
		�R�[�h�͈̔́F 0x8340 �` 8396
	 �ƍ�����
		�e�s�ɂ� ���� -> ���� -> ���� ->�@������ (�������͗�O)
		
		�@�A�B�C�D�E�F�G�H�I
		�J�K�L�M�N�O�P�Q�R�S
		�T�U�V�W�X�Y�Z�[�\�]
		�^�_�`�a�b�c�d�e�f�g�h
		�i�j�k�l�m
		�n�o�p�q�s�t�u�v�w�x�y�z�{�|
		�}�~������
		������������
		����������
		������������
		������															*/

/*	�`�F�b�N�E���X�g or TODO
	�E��O�����������Ƃ��̑Ώ�
	�E�G���f�B�A���̍l��(�H)
	�E�ׂ����`�F�b�N
*/

namespace ms932
{
	typedef unsigned char u_char;
	typedef unsigned short u_short;

	class Err
	{	public: Err( const char* msg )	{ std::cerr << msg << '\n'; }	};

	enum { first, second };
	
	/* wide_char ���p��
		char�^�z��2��(2Byte) / short(2Byte) �e�����Ȃ�
		char�z��̐擪�v�f�F Shift-JIS�̏��1Byte (�ʂ̑傫����)
		char�z��̖����f�F JIS�R�[�h�܂���shift-JIS�̉���1Byte (�ʂ̏�������)					*/
	union wide_char
	{
	public:
		u_char byte[ WCHAR_SIZE ];
		u_short code;

		// �R���X�g���N�^
		explicit wide_char()
		{
			byte[ first ] = 0;
			byte[ second ] = 0;
			code = 0;
			check();
		}
		// �R���X�g���N�^ �I�[�o�[���[�h ( �錾�Ƌ��ɑ�����s����p )
		wide_char( u_short set_code )
		{
			byte[ first ] = set_code;
			byte[ second ] = set_code;
			code = set_code;
			check();
		}
		// �R�s�[�R���X�g���N�^
		wide_char( const wide_char& ob )
		{
			this-> byte[ first ] = ob.byte[ first ];
			this-> byte[ second ] = ob.byte[ second ];
			this-> code = ob.code;
		}
		// char��1Byte short��2Byte���`�F�b�N
		void check()
		{
			if ( sizeof ( char ) != CHAR_SIZE ) throw Err( "size err" );
			if ( sizeof ( short ) != WCHAR_SIZE ) throw Err( "size err" );
		}
		/* ������Z�q�̃I�[�o�[���[�h
			�����F�����R�[�h�ɑ������鐮���l(unsigned short)	*/ 
		wide_char& operator = ( u_short set_code ) 
		{
			this-> code = set_code;
			return *this;
		}
	};
	/* jis �N���X
		JIS�R�[�h���� [0x21:0xDF] ��������														*/
	class jis
	{
	private:
		u_char  letter;
		u_short code;
		const int empty;
	public:
		// �R���X�g���N�^
		jis() : empty( 0 )
		{
			letter = empty;
			code   = empty;
		}
		// �R�s�[�R���X�g���N�^
		jis( const jis& l_ob ) : empty( 0 )
		{
			this-> letter = l_ob.letter;
			this-> code   = l_ob.code;
		}
		/* range_check�֐�
			�ړI�FJIS�R�[�h�͈̔͂ɂ��邩�`�F�b�N
			�ߒl�F�Ó��Ȕ͈͂ł��邩�ۂ���\���^�U�l					*/
		bool range_check( u_char ch )
		{
			if ( ch < 0x21 || ch > 0xdf ) {
				throw Err( "range err" );
 				return false;
			}
			return true;
		}
		/*	������Z�q�I�[�o�[���[�h
			 Jis�N���X�̃I�u�W�F�N�g���R�s�[�����
			 �E�ӂ��Ǝ��̃������̈�������Ƃ�ۏ�							*/
		jis& operator = ( jis& r_ob )
		{
			this-> letter = r_ob.letter;
			this-> code   = r_ob.code;
			return *this;
		}
		/* format_put�֐�
			�ړI�F�����Ƃ��ăt�H�[�}�b�e�B���O���ꂽJis�R�[�h���o��
			�����F�������̏o�̓X�g���[���̎Q�Ƃ֕����R�[�h��n��
			�ߒl�FJis�R�[�h�Ƃ��đÓ��Ȕ͈͂ɂ��邱�Ƃ�\���^�U�l			*/
		bool format_put( std::ostream& stream, wide_char jis_char ) {
			if ( range_check( jis_char.byte[ first ] ) ) {
				stream << jis_char.byte[ first ];
				return true;
			}
			return false;
		}
		/* binary_put�֐�
			�ړI�F�t�H�[�}�b�e�B���O����Ȃ������R�[�h�̐����l���o��
			�����F�������̏o�̓X�g���[���̎Q�Ƃ֕����R�[�h��n��
			�ߒl�FJis�R�[�h�Ƃ��đÓ��Ȕ͈͂ɂ��邱�Ƃ�\���^�U�l			*/
		bool binary_put( std::ostream& stream, wide_char jis_char ) {
			if ( range_check( jis_char.byte[ first ] ) ) {
				stream << ( static_cast<int>( jis_char.byte[ first ] ) );
				return true;
			}
			return false;
		}
		friend std::ostream& operator << ( std::ostream& , jis& );
		friend std::istream& operator >> ( std::istream& , jis& );
	};
	// Jis�R�[�h�ɑ������镶�����X�g���[���ɏo��
	std::ostream& operator << ( std::ostream& stream, jis &ob )
	{
		stream << ob.letter;
		return stream;
	}
	// Jis�R�[�h�ɑ������镶�����X�g���[���������
	std::istream& operator >> ( std::istream& stream, jis &ob )
	{
		stream >> ob.letter;

		if( stream.fail() ) {
			std::cerr << "Reading Fail\n";
			stream.clear();
		}
		return stream;
	}
	/* s_jis �N���X
		[0x8041:0xFC4B] - �S�p����(2Byte) [ 0x[80:fc] [40:fc] ]
		Shift-Jis �̑S�p�����͈̔͂�����														*/
	class s_jis
	{
	private:
		std::locale loc_jpn;
		wide_char letter;
		const int empty;
	public:
		// �R���X�g���N�^
		s_jis() : empty( 0 )
		{
			// ���P�[������{��ɐݒ�
			loc_jpn =  std::locale( "Japanese_Japan" );
			letter.code = empty;
			letter.byte[ first ]  = empty;
			letter.byte[ second ] = empty;
		}
		// �R�s�[�R���X�g���N�^
		s_jis( const s_jis& l_ob ) : empty( 0 )
		{
			this-> loc_jpn = l_ob.loc_jpn;
			this-> letter.code = l_ob.letter.code;
			this-> letter.byte[ first ]  = l_ob.letter.byte[ first ];
			this-> letter.byte[ second ] = l_ob.letter.byte[ second ];
		}
		// ������Z�q �I�[�o�[���[�h
		s_jis& operator = ( s_jis& r_ob ) 
		{
			this-> loc_jpn = r_ob.loc_jpn;
			this-> letter.byte[ first ]  = r_ob.letter.byte[ first ];
			this-> letter.byte[ second ] = r_ob.letter.byte[ second ];
			this-> letter.code = r_ob.letter.code;
			return *this;
		}
		// Shift-Jis�����Ƃ��ăt�H�[�}�b�g���ďo��
		bool format_put( std::ostream& stream, wide_char sjis_char ) {
			if ( range_check( sjis_char.code ) ) {
				stream << sjis_char.byte[ second ] << sjis_char.byte[ first ];
				return true;
			}
			return false;
		}
		// Shift-Jis�̕����R�[�h(�F���l)�ŏo��
		bool binary_put( std::ostream& stream, wide_char sjis_char ) {
			if ( range_check( sjis_char.code ) ) {
				stream << static_cast<u_short>( sjis_char.code );
				return true;
			}
			return false;
		}
		// wide_char���p�̌^�����o�l��Shift-Jis�R�[�h�͈͂��`�F�b�N
		bool range_check( u_short target )
		{
			letter.code = target;
			// Shit-Jis��2Byte���S��(short int)
			if ( this-> letter.code   < 0x8041
				|| this-> letter.code > 0xfc4b ) {
				throw Err( "invaild range" );
				return false;
			}
			// ���1Byte��char�z��
			if ( this-> letter.byte[ second ]   < 0x80 
				|| this-> letter.byte[ second ] > 0xfc ) {
				throw Err( "invaild range" );
				return false;
			}
			// ����1Byte��char�z��
			if ( this-> letter.byte[ first ]   < 0x40 
				|| this-> letter.byte[ first ] > 0xfc ) {
				throw Err( "invaild range" );
				return false;
			}
			return true;
		}
		friend std::ostream& operator << ( std::ostream& , s_jis& );
		friend std::istream& operator >> ( std::istream& , s_jis& );
	};
	// Shift-Jis�R�[�h��Ή����镶�������ŏo��
	std::ostream& operator << ( std::ostream& stream, s_jis &ob )
	{
		stream.imbue( ob.loc_jpn );
		stream << ob.letter.byte[ second ] << ob.letter.byte[ first ];
		return stream;
	}
	// ���̓X�g���[������Shift-Jis�����̕����Ƃ��ē���
	std::istream& operator >> ( std::istream& stream, s_jis &ob )
	{
		stream.imbue( ob.loc_jpn );
		stream >> ob.letter.byte[ second ] >> ob.letter.byte[ first ];
		
		if ( stream.fail() ) {
			std::cerr << "Reading Fail\n";
			stream.clear();
			stream.ignore( sizeof ob.letter );
		}
		return stream;
	}
} /* ms932 namespace End */