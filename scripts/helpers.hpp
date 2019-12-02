#ifndef HELPERS_H_INCLUDED_
#define HELPERS_H_INCLUDED_
#include <algorithm>
#include <random>
#include "declarations.hpp"
using namespace std;

namespace helpers {
string terminal_stdout(string command) {
  	string data;
  	FILE * stream;
  	const int max_buffer = 1024;
  	char buffer[max_buffer];
  	command.append(" 2>&1");

  	stream = popen(command.c_str(), "r");
  	if (stream) {
  		while (!feof(stream)) if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
  		pclose(stream);
  	}
  	return data;
}

vector<string> split(string _string, char del = ' ', bool strict = false) {
	vector<string> return_value;
	string temp_string = "";
	for(auto _char : _string) {
		if(_char==del) {
			if(strict and temp_string.empty()) continue;
			return_value.push_back(temp_string);
			temp_string="";
		}
		else {
			temp_string+=_char;
		}
	}
	if(strict and !temp_string.empty()) return_value.push_back(temp_string);

	return return_value;
}


string ISPFinder(string number) {
	if(number[0] == '6') {
		switch(number[1]) {
			case '5':
				switch(number[2]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
						return "MTN";
					break;

					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						return "ORANGE";
					break;

				}
			break;

			case '7': return "MTN";
			break;

			case '8':
				  switch(number[2]) {
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
						return "MTN";
					break;

					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						return "NEXTEL";
					break;
				  }
			break;

			case '9': return "ORANGE";
			break;
		}
	}
	return "";
						
}

string random_string()
{
     string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

     random_device rd;
     mt19937 generator(rd());

     shuffle(str.begin(), str.end(), generator);

     return str.substr(0, 32);    // assumes 32 < number of characters in str         
}

string to_upper(string input) {
	string str = input;
	transform(str.begin(), str.end(),str.begin(), ::toupper);
	return str;
}


//Customized just to work for those needing tools to continue working on deku
bool modem_is_available(string modem_imei) {
	string list_of_modem_indexes = helpers::terminal_stdout("./modem_information_extraction.sh list");
	vector<string> modem_indexes = helpers::split(list_of_modem_indexes, '\n', true);

	for(auto modem_index : modem_indexes) {
		if(modem_index == modem_imei && modem_imei.find("192.168") != string::npos) return true;
		string modem_information = helpers::terminal_stdout((string)("./modem_information_extraction.sh extract " + modem_index));
		vector<string> imei_info = helpers::split(modem_information, ':', true);
		if(imei_info[0] == "equipment_id") {
	       		if(imei_info[1].find(modem_imei) != string::npos) return true;
		}	
	}
	return false;
}

//TODO: Make work load checking functional
int read_log_calculate_work_load(string modem_path) {
	string func_name = "Read Log Calculate Workload";
	//cout << func_name << "=> started calculating work load" << endl;
	ifstream modem_log_read(modem_path.c_str());
	//XXX: Assumption is the file is good if it's passed in here
	string tmp_buffer;
	int total_count = 0;
	while(getline(modem_log_read, tmp_buffer)) {
		//XXX: timestamp:count
		string timestamp = helpers::split(tmp_buffer, ':', true)[0];
		string count = helpers::split(tmp_buffer, ':', true)[1];
		total_count += atoi(count.c_str());
	}
	modem_log_read.close();
	//cout << func_name << "=> calculating work load ended..." << endl;
	return total_count;
}

void write_to_request_file( string message, string number ) {
	ofstream write_to_request_file(SYS_REQUEST_FILE, ios::app);
	write_to_request_file << "number=" << number << ",message=\"" << message << "\"" << endl;
	write_to_request_file.close();	
}

}

#endif