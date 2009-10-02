using namespace std;

class Parameters: public string, vector <string>
{
private:
	string txt_file;
	string xml_file;
	string compiler;
	vector v;
public:
//	vector v;
	void set_txt_file(string s)
	{
		txt_file = s;
	}

	void set_xml_file(string s)
	{
		xml_file = s;
	}

	void set_compiler( string s)
	{
		compiler = s;
	}

	string get_txt_file(void)
	{
		return txt_file;
	}

	string get_xml_file(void)
	{
		return xml_file;
	}

	string get_compiler(void)
	{
		return compiler;
	}

	void print_cmd_line()//(vector<string> v)
	{
		for(unsigned int i = 0 ; i < v.size(); i++){
			string s = v[i];
			cout << s << endl;
		}
		cout << endl;
	}

	string get_next_param()//(vector<string> v)
	{
			string s = v.back();
			v.pop_back();
			return s;
	}

	int get_len()
	{
		return (int) v.size();
	}

	Parameters(vector <string> v0): v(v0), txt_file(UNDEFINED), xml_file(UNDEFINED), compiler(UNDEFINED) {};//, size (v.size()){};
	~Parameters(void){};
};
