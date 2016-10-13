#include "ssplit.h"

using namespace std;

string
trim(const string & s, const string & padding)
{
	unsigned begin, end;

	begin = s.find_first_not_of(padding);
	if ( begin == s.npos )
		return string();
	end = s.find_last_not_of(padding)+1;
	return s.substr(begin, end-begin);
}

vector <string>
ssplit(const string & s, unsigned maxfields, const string & separator)
{
	vector<string> fields;
	unsigned begin, end;

	for ( begin = s.find_first_not_of(separator) ;
			begin != s.npos ; begin = s.find_first_not_of(separator, end) )
	{
		if ( maxfields && !--maxfields )	// ultimo campo
			end = s.find_last_not_of(separator)+1;
		else
			end = s.find_first_of(separator, begin);
		fields.push_back(s.substr(begin, end-begin));
	}
	return fields;
}

vector<string>
ssplit(const string & s, const string & separator, unsigned maxfields)
{
	return ssplit(s, maxfields, separator);
}

vector <string>
csplit(const string & s, const string & separator, bool trimfields)
{
	vector<string> fields;

	unsigned len = s.length();
	if ( !len )
		return fields;
	unsigned begin, end;
	for ( begin = end = 0 ; end <= len && begin <= len ; begin = end+1 )
	{
		end = s.find_first_of(separator, begin);
		string field = s.substr(begin, end-begin);
		fields.push_back(trimfields ? trim(field) : field);
	}
	return fields;
}

vector <string>
qsplit(const string & s, const string & separator)
{
	vector<string> fields;
	unsigned len = s.length();
	if ( !len )
		return fields;
	
	enum parser_state { ST_START, ST_QUOTED, ST_UNQUOTED, ST_FINISHED };
	enum char_type { CH_QUOTE, CH_SPACE, CH_SEP, CH_OTHER };
	const char ESCAPE = '\\';
	const char QUOTE = '"';

	string field;
	parser_state state = ST_START;
	for ( unsigned i = 0 ; i < len ; i++ )
	{
		char c = s[i];
		char_type type;
		switch ( c )
		{
			case ESCAPE:
				if ( i < len-1 && s[i+1] == QUOTE )
				{
					i++;	
					c = QUOTE;
				}
				type = CH_OTHER;
				break;
			case QUOTE:
				type = CH_QUOTE;
				break;
			default:
				if ( split_space.find(c) != split_space.npos )
					type = CH_SPACE;
				else if ( separator.find(c) != separator.npos )
					type = CH_SEP;
				else
					type = CH_OTHER;
				break;
		}
		switch ( state )
		{
			case ST_START:
				switch ( type )
				{
					case CH_QUOTE:
						field = "";
						state = ST_QUOTED;
						break;
					case CH_SPACE:
						break;
					case CH_SEP:
						fields.push_back("");
						break;
					case CH_OTHER:
						field = c;
						state = ST_UNQUOTED;
						break;
				}
				break;
			case ST_QUOTED:
				switch ( type )
				{
					case CH_QUOTE:
						state = ST_FINISHED;
						break;
					case CH_SPACE:
					case CH_SEP:
					case CH_OTHER:
						field += c;
						break;
				}
				break;
			case ST_UNQUOTED:
				switch ( type )
				{
					case CH_QUOTE:
						fields.push_back(field);
						field = "";
						state = ST_QUOTED;
						break;
					case CH_SPACE:
						state = ST_FINISHED;
						break;
					case CH_SEP:
						fields.push_back(field);
						state = ST_START;
						break;
					case CH_OTHER:
						field += c;
						break;
				}
				break;
			case ST_FINISHED:
				switch ( type )
				{
					case CH_QUOTE:
						fields.push_back(field);
						field = "";
						state = ST_QUOTED;
						break;
					case CH_SPACE:
						break;
					case CH_SEP:
						fields.push_back(field);
						state = ST_START;
						break;
					case CH_OTHER:
						fields.push_back(field);
						field = c;
						state = ST_UNQUOTED;
						break;
				}
				break;
		}
	}
	if ( state != ST_START )
		fields.push_back(field);
	return fields;
}

