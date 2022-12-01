#pragma once
#ifndef PLANNING_ITEM_H
#define PLANNING_ITEM_H

#include "string.h"

namespace planning {

	class item {

		float Amount;
		string Name;

		item();
		item(const char* aName);
		item(string aName);
		item(float aAmount, const char* aName);
		item(float aAmount, string aName);


	};

}

#endif // !PLANNING_ITEM_H
