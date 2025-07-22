// AP_EXTRA_TYPEDEFS_H

#ifndef AP_EXTRA_TYPEDEFS_H
#define AP_EXTRA_TYPEDEFS_H

typedef struct angle	{
	
	struct ap v;
	int type; // radians or degrees, etc
	
	uint16_t clockwise;
	
} angle;

typedef struct vector_t	{
	
	struct ap t;
	struct ap x;
	struct ap y;
	struct ap z;
	struct ap k;
	
	uint16_t type;

} vector_t;

typedef struct circle_t	{
	
	struct ap radius;
	struct vector_t origin;
	struct ap** segments;
	
	uint16_t type;
} circle_t;
	
typedef struct triangle_t {

	struct ap opp;
	struct ap adj;
	struct ap hyp;
	
	struct ap hyp_adj_angle;
	struct ap hyp_opp_angle;
	struct ap opp_adj_angle;
	
	uint16_t type; // isoscoles, equilateral, rightangle
} triangle_t;

#endif

