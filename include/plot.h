#ifndef PLOT_H
#define PLOT_H

#define TO_DEG (180 / M_PI)
#define TO_RAD (M_PI / 180)

typedef struct plot_vec2_t      plot_vec2_t;
typedef struct plot_vec3_t      plot_vec3_t;
typedef struct plot_path_t      plot_path_t;
typedef struct plot_path_part_t plot_path_part_t;

typedef struct plotbot_t plotbot_t;
typedef struct plot_t    plot_t;

struct plot_vec2_t {
	double x;
	double y;
};

struct plot_vec3_t {
	double x;
	double y;
	double z;
};

struct plot_path_part_t {
	plot_vec2_t start;
	plot_vec2_t mid;
	plot_vec2_t end;
	plot_vec2_t origin;

	double radius;

	double start_angle;
	double end_angle;
	double delta_angle;
};

struct plot_path_t {
	plot_vec2_t* points;
	// TODO get rid of radii because it isn't used which is unfortunate because
	// I like thinking I'm better than people because I know that the plural of
	// radius is radii. Oh well... I'll keep it in a bit longer.
	plot_vec2_t* radii; // radii is the plural of radius. it looks kind of dumb
	                    // so I'm using it.

	plot_path_part_t* parts;
	int               point_count;
	int               size;
};

struct plotbot_t {
	double x;
	double y;
	double rot;

	double width;
	double height;

	// movement calculation stuff
	double wheel_radius;
	double axel_width;
};

struct plot_t {
	double width;
	double height;

	plot_vec2_t origin;

#define PLOT_MAX_PATHS 256
	plot_path_t   paths[PLOT_MAX_PATHS];
	const char*   path_names[PLOT_MAX_PATHS];
	unsigned long path_hashes[PLOT_MAX_PATHS];

	int sel;
};

extern plot_t plot;

plot_t* plot_new(double width, double height);
int     plot_load_img(char* path_name);

int plot_path_add(char* name);
int plot_path_del(char* name);
int plot_path_sel(char* name);

int plot_path_calc();

int plot_point_add(double x, double y);
int plot_point_del(int index);
int plot_point_get_index(double x, double y, double radius);
int plot_point_set(int index, double x, double y);
int plot_point_set_radius(int index, double x, double y);

int plot_export(char** buffer, int buffer_size);
int plot_import(char** buffer, int buffer_size);

double plot_width();
double plot_height();

double plot_top();
double plot_bottom();
double plot_right();
double plot_left();

plot_vec2_t plot_origin();
plot_vec2_t plot_coord(double x, double y);
double      plot_x(double x);
double      plot_y(double y);

plot_path_part_t calc_path_part(plot_vec2_t start, plot_vec2_t mid,
                                plot_vec2_t end, double radius);

plot_vec2_t calc_point_project(plot_vec2_t p1, plot_vec2_t p2, plot_vec2_t p3);
plot_vec2_t calc_reflect_p3(plot_vec2_t p1, plot_vec2_t p2, plot_vec2_t p3);

/**
 * @brief calculates the distance between two points.
 *
 * @param p1 the first point
 * @param p2 the second point
 * @return double the distance between the two.
 */
double calc_dist(plot_vec2_t p1, plot_vec2_t p2);

double calc_map(double x, double in_min, double in_max, double out_max,
                double out_min);

/**
 * @brief calculates the origin of a circle given two points and a radius.
 *
 * @param p1 the first point
 * @param p2 the second point
 * @param r the circle radius
 * @return plot_point_t the origin of the circle.
 */
plot_vec2_t calc_circ_center(plot_vec2_t p1, plot_vec2_t p2, double r);

/**
 * @brief calculates a circle radius given three points on the circumfrence of
 * the circle. Note that not all possible point combination will return a valid
 * radius.
 *
 * @param p1 the first point
 * @param p2 the second point
 * @param p3 the third point
 * @return double the radius of the circle.
 */
double calc_circ_radius(plot_vec2_t p1, plot_vec2_t p2, plot_vec2_t p3);

/**
 * @brief takes a point and an angle local to one quadrant and calculates the
 * "global" angle of that point.
 *
 * @param angle the local quadrant angle
 * @param x the x of a point in the angle's quadrant
 * @param y the y of a point in the angle's quadrant
 * @return double the global angle of the supplied angle.
 */
double calc_quadrant(double angle, double x, double y);

plot_vec3_t calc_skid_transform(double x, double y, double rot, double count_r,
                                double count_l, double step, double width);

double calc_rot_delta(double x, double y, double t_x, double t_y, double ICCx,
                      double ICCy);
double calc_x_of_sinx(double sinx);
double calc_x_of_cosx(double cosx);

plot_vec2_t calc_skid_velocities(plot_vec2_t origin, double rot_delta,
                                 double width, double radius);

int plotbot_set_pos(double x, double y, double rot);

int plotbot_drive(double right, double left);
int plotbot_drive_to(double x, double y, double radius);

int plotbot_rotate(double speed);
int plotbot_rotate_to(double angle);

#endif
