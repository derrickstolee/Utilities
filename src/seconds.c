/***********************************************************

Copyright Derrick Stolee 2011.

 This file is part of SearchLib.

    SearchLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SearchLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SearchLib.  If not, see <http://www.gnu.org/licenses/>.

*************************************************************/

/*
 * seconds.c
 *
 *  Created on: Dec 19, 2010
 *      Author: derrickstolee
 */

#include <stdio.h>

int main(void)
{
	double seconds;

	while ( scanf("%lf", &seconds) != EOF )
	{
		long long int seconds_int = seconds;

		double mins, hrs, days, yrs;

		mins = seconds / (double) 60;
		hrs = seconds / (double) 3600;
		days = seconds / (double) (3600 * 24);
		yrs = seconds / (double) (3600 * 24 * 365);

		if ( yrs > 1 )
		{
			printf("%3d&%02d y", (int) yrs, ((int) (yrs * 100.0) % 100));
		}
		else if ( days > 1 )
		{
			printf("%3d&%02d d", (int) days, ((int) (days * 100.0) % 100));
		}
		else if ( hrs > 1 )
		{
			printf("%3d&%02d h", (int) hrs, ((int) (hrs * 100.0) % 100));
		}
		else if ( mins > 1 )
		{
			printf("%3d&%02d m", (int) mins, ((int) (mins * 100.0) % 100));
		}
		else
		{
			printf("%3d&%02d s", (int) seconds_int, ((int) (seconds * 100.0) % 100));
		}

		printf("  \\\\\n");
	}

	return 0;
}
