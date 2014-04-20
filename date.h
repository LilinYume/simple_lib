#include <iostream>
#include <ctime>

#ifndef DATE_H
#define DATE_H

namespace simple
{
	class Day
	{
	private:
		int year, month, day, week, passed;
	public:
		Day() 
		{
			year = month = day = week = passed = 0;
			create_day_value();
		}
		Day( const Day& l_ob )
		{
			this-> year   = l_ob.year;
			this-> month  = l_ob.month;
			this-> day    = l_ob.day;
			this-> week   = l_ob.week;
			this-> passed = l_ob.passed;
		}
		void create_day_value() 
		{
			struct tm today;
			time_t t_val; 

			t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			year   = today.tm_year + 1900;
			month  = today.tm_mon + 1;
			day    = today.tm_mday;
			week   = today.tm_wday;
			passed = today.tm_yday;
		}
		int get_year()   { return year; }
		int get_month()  { return month; }
		int get_day()    { return day; }
		int get_week()   { return week; }
		int get_passed() { return passed; }
	};
	class Time
	{
	private:
		int hour, miniture, second;
	public:
		Time()
		{
			hour = miniture = second = 0;
			create_time_value();
		}
		Time( const Time& l_ob )
		{
			this-> hour     = l_ob.hour;
			this-> miniture = l_ob.miniture;
			this-> second   = l_ob.second;
		}
		void create_time_value()
		{
			struct tm today;
			time_t t_val; 

			t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			hour     = oday.tm_hour;
			miniture = today.tm_min;
			second   = today.tm_sec;
		}
		int get_hour()     { return hour; }
		int get_miniture() { return miniture; }
		int get_second()   { return second; }
	};
	class Date
	{
	private:
		Day Today;
		Time Now;
	public:
		Date()
		{
			
		}
		Date( const Date& l_ob ){}
	};

}
#endif

