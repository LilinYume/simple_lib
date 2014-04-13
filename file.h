// つくりかけヾ(oﾟωﾟo)ﾉﾞ ﾌﾟﾆﾌﾟﾆ
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

struct Data_Separator
{
	const char column;
	const char record;
	const char table;
	const char file;

	Data_Separator() 
		: column( 0x1f ), record( 0x1e ), table( 0x1d ), file( 0x1c ){ ; }
};

class io_err{ };

class File
{
private:
	int col_size;
	int rec_size;
	Data_Separator sept;
	ofstream outStream;
public:
	explicit File( )
	{
		col_size = rec_size = 0;
	}
	File( const File& lval ) {
		this-> col_size = lval.col_size;
		this-> rec_size = lval.rec_size;
	}
	void init_create( const char* set_path )
	{
		if ( outStream.is_open() ) outStream.close();
		outStream.open( set_path, ios::binary );

		if( err_status() ) throw io_err();
	}
	void set_column( const int size ) {
		col_size = size;
		outStream << setw( size ) << ' ';
		outStream.put( sept.column );
	}
private:
	int err_status( )
	{
		int err_bit = 0;

		if ( outStream.good() ) { std::cout << "ok" << endl; }

		else if ( outStream.fail() ) {
			err_bit = outStream.failbit;
			std::cerr << "stream status: fail\n"; 
			throw io_err();
		}
		else if ( outStream.bad() ) {
			err_bit = outStream.badbit;
			std::cerr << "stream status: bad\n"; 
			throw io_err();
		}
		return err_bit; 
	}
};

int main()
{
	File t;
	t.init_create( "tbl.txt" );
	
	
	
	return 0;
}