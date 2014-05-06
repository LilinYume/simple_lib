#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <locale>

#ifndef DATE_H
#define DATE_H

namespace simple
{
	const int days_buf = 11;
	const int times_buf = 9;
	class Date
	{
	private:
		std::string month_lable;
		std::string week_lable;
		char days[ days_buf ];
		char times[ times_buf ];
		int year, month, day, week;
		int hour, miniture, second;
	public:
		Date()
		{
			month_lable = "";
			week_lable = "";

			for( int i = 0; i < sizeof days; ++i ) {
				days[ i ] = '\0';
			}
			for( int i = 0; i < sizeof times; ++i ) {
				times[ i ] = '\0';
			}
			year = month = day = week = 0;
			hour = miniture = second = 0;
		}
		Date( const Date& l_ob )
		{
			this-> month_lable = l_ob.month_lable;
			this-> week_lable = l_ob.week_lable;

			for( int i = 0; i < sizeof days; ++i ) {
				this-> days[ i ] = l_ob.days[ i ];
			}
			for( int i = 0; i < sizeof times; ++i ) {
				this-> times[ i ] = l_ob.times[ i ];
			}
			this-> year = l_ob.year;
			this-> month = l_ob.month;
			this-> day = l_ob.day;
			this-> week = l_ob.week;
			
			this-> hour = l_ob.hour;
			this-> miniture = l_ob.miniture;
			this-> second = l_ob.second;
		}
	private: 
		void set_month_lable()
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			int current_month = today.tm_mon;

			std::string tmp[][12] = 
								{
									{ "Jan" },
									{ "Feb" },
									{ "Mar" },
									{ "Apr" },
									{ "May" },
									{ "Jun" },
									{ "Jul" },
									{ "Aug" },
									{ "Sep" },
									{ "Oct" },
									{ "Nov" },
									{ "Dec" },
								};
			month_lable = *tmp[ current_month ];
		}
		void set_week_lable() 
		{
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			int current_week = today.tm_wday;
			
			std::string tmp[][7] = 
									{
										{ "Sun" },
										{ "Mon" },
										{ "Tue" },
										{ "Wed" },
										{ "Thu" },
										{ "Fri" },
										{ "Sat" },
									};
			week_lable = *tmp[ current_week ];
		}
		void set_day_value() 
		{
			char format[] = "%Y/%m/%d";
			
			if ( sizeof format > days_buf ) {
				std::cout << "not enough buffer about day" << std::endl;
				exit( EXIT_FAILURE );
			}
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			year = today.tm_year + 1900;
			month = today.tm_mon + 1;
			day = today.tm_mday;
			week = today.tm_wday;

			std::strftime( days, days_buf, format, &today );
		}
		void set_time_value() 
		{
			char format[] = "%H:%M:%S";

			if ( sizeof format > times_buf ) {
				std::cout << "not enough buffer about time" << std::endl;
				exit( EXIT_FAILURE );
			}
			tm today;
			time_t t_val = std::time( NULL );
			localtime_s( &today, &t_val );

			hour = today.tm_hour;
			miniture = today.tm_min;
			second = today.tm_sec;

			std::strftime( times, times_buf, format , &today );
		}
	public:
		const char* get_month_lable()
		{
			set_month_lable();
			return month_lable.c_str();
		}
		const char* get_week_lable() 
		{
			set_week_lable();
			return week_lable.c_str(); 
		}
		const char* get_fmt_day() 
		{
			set_day_value();
			return days;
		}
		const char* get_fmt_time()
		{
			set_time_value();
			return times;
		}
		int get_year()
		{
			set_day_value();
			return year;
		}
		int get_month()
		{
			set_day_value();
			return month;
		}
		int get_day()
		{
			set_day_value();
			return day;
		}
		int get_week()
		{
			set_day_value();
			return week;
		}
		int get_hour()
		{
			set_time_value();
			return hour;
		}
		int get_miniture()
		{
			set_time_value();
			return miniture;
		}
		int get_second()
		{
			set_time_value();
			return second;
		}
	};
}
#endif