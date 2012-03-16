// Andrew Naplavkov

#ifndef BRIG_BOOST_ENVELOPE_HPP
#define BRIG_BOOST_ENVELOPE_HPP

#include <boost/variant/apply_visitor.hpp>
#include <brig/boost/detail/envelope_visitor.hpp>
#include <brig/boost/geometry.hpp>

namespace brig { namespace boost {

inline box envelope(const geometry& geom)
{
  return ::boost::apply_visitor(detail::envelope_visitor(), geom);
}

} } // brig::boost

#endif // BRIG_BOOST_ENVELOPE_HPP
