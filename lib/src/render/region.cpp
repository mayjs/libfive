#include <algorithm>

#include "ao/render/region.hpp"

Region::Region(Interval x, Interval y, Interval z, float res)
    : Region(x, y, z, res, res, res)
{
    // Nothing to do here
}

Region::Region(Interval x, Interval y, Interval z,
               float rx, float ry, float rz)
    : X(x, rx), Y(y, ry), Z(z, rz)
{
    // Nothing to do here
}

Region::Region(Axis x, Axis y, Axis z)
    : X(x), Y(y), Z(z)
{
    // Nothing to do here
}

////////////////////////////////////////////////////////////////////////////////

Subregion Region::view() const
{
    return Subregion(*this);
}

Region Region::powerOfTwo() const
{
    size_t vox = std::max({X.values.size(), Y.values.size(), Z.values.size()});
    size_t n = 1 << (size_t)ceil(log(vox) / log(2));

    // Calculate how much extra needs to be added to an axis
    auto d = [=](const Axis& a){
        return (a.bounds.upper() - a.bounds.lower()) *
               (float(n) / a.values.size() - 1);
    };

    // Return the expanded axis bounds
    auto a = [=](const Axis& a){
        auto da = d(a);
        return Axis({a.bounds.lower() - da/2,
                     a.bounds.upper() + da/2}, n);
    };

    auto r = Region(a(X), a(Y), a(Z));

    assert(r.X.values.size() == n);
    assert(r.Y.values.size() == n);
    assert(r.Z.values.size() == n);

    return r;
}

////////////////////////////////////////////////////////////////////////////////

Region::Axis::Axis(Interval i, float res)
    : bounds(i)
{
    size_t size = std::max((size_t)1,
                           (size_t)(res * (i.upper() - i.lower())));
    for (unsigned index=0; index < size; ++index)
    {
        const float frac = (index + 0.5) / size;
        values.push_back(i.lower() * (1 - frac) + i.upper() * frac);
    }
}

Region::Axis::Axis(Interval i, size_t size)
    : bounds(i)
{
    for (unsigned index=0; index < size; ++index)
    {
        const float frac = (index + 0.5) / size;
        values.push_back(i.lower() * (1 - frac) + i.upper() * frac);
    }
}
