#pragma once

/**
 * @class HSVRange
 * @brief Provides information for a range of HSV values
 */
struct HSVRange
{
	int min_hue;  /** Lower bound of the hue */
	int max_hue; /** Upper bound of the hue */
	int min_sat;  /** Lower bound of the saturation */
	int max_sat; /** Upper bound of the saturation */
	int min_val;  /** Lower bound of the value */
	int max_val; /** Upper bound of the value */

	/**
	 * Constructor for a default color range that works okay for white skin
	 */
	HSVRange():
		min_hue(0),
		max_hue(179),
		min_sat(52),
		max_sat(119),
		min_val(150),
		max_val(255)
	{
	}
};