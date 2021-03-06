#pragma once


class v2d {
    private:
        float inv_sqrt(float x)
            { union { float f; uint32_t u; } y = {x};
            y.u = 0x5F1FFFF9ul - (y.u >> 1);
            return 0.703952253f * y.f * (2.38924456f - x * y.f * y.f);
            }
        float randMapped() {
            return(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
        }
    public:
    //Create an empty vector
    v2d() { x = 0.0f; y = 0.0f; }
    //Create a vector from _x and _y
    v2d(const float _x, const float _y) : x(_x), y(_y) {}
    //Set vector to _x and _y
    v2d set(float _x, float _y) { x = _x; y = _y; return(*this); }
    //Randomize this vector
    v2d randomize(float scale = 1) { float r = randMapped() * 2 * 3.141592f; set(cos(r) * scale, sin(r) * scale); return(*this); }
    //Set vector to zero
    void zero() { x = 0.0f; y = 0.0f; }

    v2d operator + (const v2d& r) { return v2d(this->x + r.x, this->y + r.y); }
    v2d operator + (const float r) { return v2d(this->x + r, this->y + r); }
    v2d operator += (const v2d& r) { this->x += r.x; this->y += r.y; return(*this); }
    v2d operator += (const float r) { this->x += r; this->y += r; return(*this); }

    v2d operator - (const v2d& r) { return v2d(this->x - r.x, this->y - r.y); }
    v2d operator - (const float r) { return v2d(this->x - r, this->y - r); }
    v2d operator -= (const v2d& r) { this->x -= r.x; this->y -= r.y; return(*this); }
    v2d operator -= (const float r) { this->x -= r; this->y -= r; return(*this); }

    v2d operator * (const v2d& r) { return v2d(this->x * r.x, this->y * r.y); }
    v2d operator * (const float r) { return v2d(this->x * r, this->y * r); }
    v2d operator *= (const v2d& r) { this->x *= r.x; this->y *= r.y; return(*this); }
    v2d operator *= (const float r) { this->x *= r; this->y *= r; return(*this); }

    v2d operator / (const v2d& r) { return v2d(this->x / r.x, this->y / r.y); }
    v2d operator / (const float r) { return v2d(this->x / r, this->y / r); }
    v2d operator /= (const v2d& r) { this->x /= r.x; this->y /= r.y; return(*this); }
    v2d operator /= (const float r) { this->x /= r; this->y /= r; return(*this); }

    bool operator > (v2d& r) { return(sqrLen() > r.sqrLen());}
    bool operator < (v2d& r) { return(sqrLen() < r.sqrLen());}
    bool operator == (v2d& r) { return(sqrLen() == r.sqrLen());}
    bool operator != (v2d& r) { return(sqrLen() != r.sqrLen());}

    v2d norm() { *this = *this * invLen(); return(*this); }
    v2d setLen(float mag) { 
        this->operator*=(invLen() * mag);
        return(*this); 
    }
    float len() { 
        return sqrtf((x*x) + (y*y));
    }

    float invLen() {
        return inv_sqrt(sqrLen());
    }
    float sqrLen() { return(x * x + y * y); }
    float sqrDist(const v2d& b) { v2d a = *this - b; return(a.x * a.x + a.y * a.y); }

    v2d limit(float b)
    {
        if (sqrLen() <= b * b) {
            return(*this);
        }
        setLen(b);
        return(*this);
    }

    v2d rotate(float rad){
        float cs = cos(rad), sn = sin(rad);
        float px = x * cs - y * sn; 
        float py = x * sn + y * cs;
        return v2d(px, py);
    }
    float x;
    float y;
};