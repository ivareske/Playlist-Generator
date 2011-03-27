#ifndef RULE_H
#define RULE_H

#include "structs.h"


class rule{

	public:
	rule();
	ruleType type;
	QString value;
	bool shouldBeTrue;
	bool caseSensitive;
	

};


 
#endif

