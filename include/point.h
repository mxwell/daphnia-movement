#ifndef __SSU_KMY_POINT_H_
#define __SSU_KMY_POINT_H_

#include <types.h>

/* Class describes state of modelled system and
 * gives a convenient way to write expressions
 * of integration
 */

class Point
{
	friend class Model;
public:
	Point() {}
	Point(ld X, ld Y, ld Vx, ld Vy, ld x, ld v, ld e, ld psi) :
		X(X), Y(Y), Vx(Vx), Vy(Vy),
		x(x), v(v), e(e), psi(psi) {}
	friend const Point operator+(const Point&, const Point&);
	friend const Point operator-(const Point&, const Point&);
	friend const Point operator*(const ld&, const Point&);
	friend const Point operator*(const Point&, const ld&);
	friend const Point operator/(const Point&, const ld&);
	const char *to_string();
	const char *pos_to_string();
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const;
	double get_Vx() const;
	double get_Vy() const;
private:
	/* position of particle */
	ld X;
	ld Y;
	/* speed of particle */
	ld Vx;
	ld Vy;
	/* relative position of motor */
	ld x;
	/* relative speed of motor */
	ld v;
	/* depot */
	ld e;
	/* angle of motor oscillations */
	ld psi;
	char buf[128];
};

#endif /* __SSU_KMY_POINT_H_ */
