#pragma once

#include "Common.hpp"

/** 2 Driver **/
/** 2.1 Driver command line options **/

namespace driver {

	string mainfilename;
	vector<string> hsq_bucket_nm;
	vector<string> asq_bucket_nm;
	vector<string> sq_bucket_nm;
	vector<string> hsq_bucket_text;
	vector<string> asq_bucket_text;
	vector<string> sq_bucket_text;
	bool logo = true;
	bool usebignum = false;
	int action = 0; // 0-execute; 1-sq, 2-asm; 3-itr
	bool bcin = false;
	bool bcout = false;
	int langflag = 0; // 0-unknown; 1-hsq; 2-asq; 3-sq

} // driver
