/*
 * main.c
 * 
 * Copyright 2013 Ben Tatman <ben@tatmans.co.uk>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

typedef struct {
	Vector x;
	Vector y;
} Resolved;

typedef struct {
	float x;
	float y;
} Position;

Vector balance_vector(Vector a, Vector b);

float divide(float a, float b) {
	if (b == 0)
		b = 1;
	return a/b;
}

float power(float val, int power) {
	float res = 1;
	int i;
	for (i = 0; i < power; i ++) {
		res = res*val;
	}
	return res;
}

float deg_to_rad(float degrees) {
	return degrees * M_PI / 180;
}

float rad_to_deg(float radians) {
	return radians / (M_PI/180);
}

float absol(float in) {
	float new = in;
	if (in < 0) {
		new = 0 - in;
	}
	return new; //If value is +, then we do nothing.
}

Vector new_force(float force, float angle) {
	Vector f;
	f.value = force;
	f.angle = angle;
	return f;
}

Resolved vtof(Vector f) {
	Resolved retur;
	retur.x.angle = 90; //It is 90 degrees from the y axis
	retur.y.angle = 0;
	
	if (f.angle >= 0 && f.angle < 90) {
		retur.x.value = f.value * sin(deg_to_rad(f.angle));
		retur.y.value = f.value * cos(deg_to_rad(f.angle));
	} else if (f.angle >= 90 && f.angle < 180) {
		f.angle = f.angle - 90;
		retur.x.value = f.value * cos(deg_to_rad(f.angle));
		retur.y.value = -f.value * sin(deg_to_rad(f.angle));
	} else if (f.angle >= 180 && f.angle < 270) {
		f.angle = f.angle - 180;
		retur.x.value = -f.value * sin(deg_to_rad(f.angle));
		retur.y.value = -f.value * cos(deg_to_rad(f.angle));
	} else {
		f.angle = f.angle - 270;
		retur.x.value = -f.value * cos(deg_to_rad(f.angle));
		retur.y.value = f.value * sin(deg_to_rad(f.angle));
	}
	
	return retur;
}

Vector ftov(Resolved f) {
	Vector p;
	
	p.value = sqrt(power(f.x.value, 2) + power(f.y.value, 2));

	if ((f.x.value < 0)	&& (f.y.value < 0)) {
		// it matches >= 180 < 270
		p.angle = atan(divide(absol(f.x.value), absol(f.y.value)));
		p.angle = 180 + rad_to_deg(p.angle);
	} else if ((f.x.value < 0) && (f.y.value >= 0)) {
		// then it is in the final one, or > 270
		p.angle = atan(divide(f.y.value, absol(f.x.value)));
		p.angle = 270 + rad_to_deg(p.angle);
	} else if ((f.x.value >= 0) && (f.y.value < 0)) {
		// then it is >= 90, < 180
		p.angle = atan(divide(absol(f.y.value), f.x.value));
		p.angle = 90 + rad_to_deg(p.angle);
	} else {
		// then it is the first one, or 0 < x < 90
		p.angle = atan(divide(f.x.value,f.y.value));
		p.angle = rad_to_deg(p.angle);
	}
	return p;
}




Vector balance_vector(Vector a, Vector b) {


	Resolved ax = vtof(a);
	Resolved bx = vtof(b);


	Position new;
	new.x = ax.x.value + bx.x.value;
	new.y = ax.y.value + bx.y.value;

	//printf("New %f %f\n", new.x, new.y);

	Vector result;

	result.value = sqrt((new.x*new.x)+(new.y*new.y)); // if the values are negative then them times themselves is positive. For it to be under 0, such as sqrt(-1) it would need to be a complex number, hence no validation
	float tmpangle;
	//printf("New %f %f\n", absol(new.x), absol(new.y));
	if (new.x >= 0 && new.y >= 0) {
		tmpangle = rad_to_deg(atan(divide(absol(new.x),absol(new.y))));
		// We are in the top right
	} else if (new.x >= 0 && new.y <= 0) {
		tmpangle = rad_to_deg(atan(divide(absol(new.y),absol(new.x)))) + 90;
	} else if (new.x <= 0 && new.y <= 0) {
		tmpangle = rad_to_deg(atan(divide(absol(new.x),absol(new.y)))) + 180;
	} else if (new.x <= 0 && new.y >= 0) {
		tmpangle = rad_to_deg(atan(divide(absol(new.y),absol(new.x)))) + 270;
	}
	result.angle = tmpangle;
	return result;
}
