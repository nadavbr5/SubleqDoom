#include "Driver.hpp"


/** 2.2 Driver forward declarations **/

namespace driver {

	int tmain(int ac, char* av[]);
	void usage();
	void parseoptions(int ac, char* av[]);
	string loadfile(const string& file);
	string loadstream(std::istream& in);
	void fillbuckets();
	void cutmainfilename();
	string getlogo() { return LOGO; }

} //driver

namespace hsqcomp {
	string hsqcompile(vector<string>& vname, vector<string>& vtext);
	string itrcompile(vector<string>& vname, vector<string>& vtext);
} // compiler

namespace asqcomp {
	string asqcompile(vector<string>& vtext);
} // asqcomp

namespace emulator {
	void sqemulate(vector<string>& vtext, bool big);
} // emulator



int driver::tmain(int ac, char* av[])
{
	if (ac < 2) { usage(); return 0; }
	parseoptions(ac, av);
	cutmainfilename();
	fillbuckets();
	if (!bcout && logo && action) cout << getlogo() << '\n';

	if (action == 3)
	{
		string myitr = hsqcomp::itrcompile(hsq_bucket_nm, hsq_bucket_text);
		if (bcout) cout << myitr;
		else
		{
			std::ofstream of((mainfilename + ".itr").c_str());
			of << myitr;
		}
		return 0;
	}

	string myasq = hsqcomp::hsqcompile(hsq_bucket_nm, hsq_bucket_text);
	hsq_bucket_text.clear();

	if (action == 2)
	{
		if (bcout) cout << myasq;
		else
		{
			std::ofstream of((mainfilename + ".asq").c_str());
			of << myasq;
		}
		return 0;
	}

	if (myasq != "")
	{
		asq_bucket_nm.push_back(".");
		asq_bucket_text.push_back(myasq);
	}

	string mysq = asqcomp::asqcompile(asq_bucket_text);
	asq_bucket_text.clear();

	if (action == 1)
	{
		if (bcout) cout << mysq;
		else
		{
			std::ofstream of((mainfilename + ".sq").c_str());
			of << mysq;
		}
		return 0;
	}

	if (mysq != "")
	{
		sq_bucket_nm.push_back(".");
		sq_bucket_text.push_back(mysq);
	}

	emulator::sqemulate(sq_bucket_text, usebignum);

	return 0;
}




/** 2.4 Driver functions implementation **/
/** 2.4.1 usage **/
void driver::usage()
{
	cout << getlogo() << '\n';
	cout << "Usage: hsq [options] [files]\n";
	cout << "options:\n"
		" -e execute/emulate (defalut)\n"
		" -q output subleq code\n"
		" -a output assebmly\n"
		" -d output ITR\n"
		" -h <file> input HSQ (defalut, default for *.hsq)\n"
		" -m <file> input assembly (default for *.asq)\n"
		" -s <file> input subleq code (default for *.sq)\n"
		" -i input from stdin\n"
		" -o output to stdout\n"
		" -n nologo (defalut with -o)\n"
		" -b use bignum on emulation (default int)\n";
}

/** 2.4.2 parse command line options **/
void driver::parseoptions(int ac, char* av[])
{
	langflag = 0;
	for (int i = 1; i < ac; i++)
	{
		if (av[i][0] == '-')
		{
			string o(av[i]);
			for (string::size_type j = 1; j < o.size(); j++)
				if (o[j] == 'e') action = 0;
				else if (o[j] == 'q') action = 1;
				else if (o[j] == 'a') action = 2;
				else if (o[j] == 'd') action = 3;
				else if (o[j] == 'i') bcin = true;
				else if (o[j] == 'o') bcout = true;
				else if (o[j] == 'n') logo = false;
				else if (o[j] == 'b') usebignum = true;
				else if (o[j] == 'h') langflag = 1;
				else if (o[j] == 'm') langflag = 2;
				else if (o[j] == 's') langflag = 3;
				else throw string() + "Unknown option " + o[j];
			continue;
		}
		string nm(av[i]);
		if (mainfilename == "") mainfilename = nm;
		if (langflag == 1) hsq_bucket_nm.push_back(nm);
		if (langflag == 2) asq_bucket_nm.push_back(nm);
		if (langflag == 3) sq_bucket_nm.push_back(nm);
		if (!langflag)
		{
			int sz = nm.size();
			if (nm.size() > 3 && nm.substr(sz - 3) == ".sq")
				sq_bucket_nm.push_back(nm);
			else if (nm.size() > 4 && nm.substr(sz - 4) == ".asq")
				asq_bucket_nm.push_back(nm);
			else
				hsq_bucket_nm.push_back(nm);
		}
	}

}

/** 2.4.3 fill buckets **/
string driver::loadstream(std::istream& in)
{
	string r;

	for (;;)
	{
		char c;
		in.get(c);
		if (!in) break;
		r += c;
	}

	return r;
}

string driver::loadfile(const string& file)
{
	if (file == "") return "";

	string text;
	std::ifstream in(file.c_str());
	if (!in) throw "cannot open " + file;

	return loadstream(in);
}

void driver::fillbuckets()
{
	for (size_t i = 0; i < hsq_bucket_nm.size(); i++)
		hsq_bucket_text.push_back(loadfile(hsq_bucket_nm[i]));
	for (size_t i = 0; i < asq_bucket_nm.size(); i++)
		asq_bucket_text.push_back(loadfile(asq_bucket_nm[i]));
	for (size_t i = 0; i < sq_bucket_nm.size(); i++)
		sq_bucket_text.push_back(loadfile(sq_bucket_nm[i]));

	if (bcin)
	{
		string stdinp = loadstream(std::cin);
		if (stdinp != "")
		{
			if (langflag == 3)
			{
				sq_bucket_nm.push_back("input");
				sq_bucket_text.push_back(stdinp);
			}
			else if (langflag == 2)
			{
				asq_bucket_nm.push_back("input");
				asq_bucket_text.push_back(stdinp);
			}
			else
			{
				hsq_bucket_nm.push_back("input");
				hsq_bucket_text.push_back(stdinp);
			}

		}
		if (mainfilename == "") mainfilename = "input";
	}
}

/** 2.4.4 base name **/
void driver::cutmainfilename()
{
	using namespace driver;
	string::size_type i = mainfilename.find(".");
	if (i == string::npos || i == 0) return;
	mainfilename = mainfilename.substr(0, i);
}