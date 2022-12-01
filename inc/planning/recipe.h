#pragma once
#ifndef PLANNING_RECIPE_H
#define PLANNING_RECIPE_H

#include <vector>

#include "item.h"

namespace planning {

	class recipe {

	private:

		item Output;
		int InputCount;
		item *InputList;
		float LaborTime;

	};

}

#endif // !PLANNING_RECIPE_H
