//               Copyright Paul Colby 2014.
// Distributed under the Boost Software License, Version 1.0.
//       (See accompanying file LICENSE.md or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

/**
 * @file
 * @brief Defines PMDA cache helper functions.
 */

#ifndef __PCP_CPP_CACHE_HPP__
#define __PCP_CPP_CACHE_HPP__

#include "config.hpp"
#include "exception.hpp"
#include "types.hpp"

PCP_CPP_BEGIN_NAMESPACE

namespace pcp {
namespace cache {

/**
 * @brief Flags that may be applied to cache lookups.
 */
enum lookup_flags {
    require_active = 0x1 ///< Throw if the found cache entry is not active.
};

/**
 * @brief Pipe operator for combining lookup_flags values.
 *
 * This function performs a logical OR of two lookup_flags sets. This is a
 * convenience function, allowing lookup_flags enum values to be used both
 * standalone, and in combination.
 *
 * @param a First set of flags.
 * @param b Second set of flags.
 *
 * @return Combined set of both \a a and \a b flags.
 */
inline lookup_flags operator|(lookup_flags a, lookup_flags b)
{
    return static_cast<lookup_flags>(
        static_cast<int>(a) | static_cast<int>(b)
    );
}

template <typename Type>
struct lookup_result_type {
    char * name;
    instance_id_type instance_id;
    Type opaque;
    int status;
};

template <typename Type>
lookup_result_type<Type> lookup(const pmInDom indom,
                                const instance_id_type instance_id,
                                const lookup_flags flags = require_active)
{
    lookup_result_type<Type> result;
    void * opaque;
    result.status = pmdaCacheLookup(indom, instance_id, &result.name, &opaque);
    if (result.status < 0) {
        throw pcp::exception(result.status);
    }
    if ((flags & require_active) && (result.status != PMDA_CACHE_ACTIVE)) {
        throw pcp::exception(result.status, "not active"); ///< @todo Better message.
    }
    result.instance_id = instance_id;
    result.opaque = static_cast<Type>(opaque);
    return result;
}

template <typename Type>
lookup_result_type<Type> lookup(const pmInDom indom, const std::string &name,
                                const lookup_flags flags = require_active)
{
    lookup_result_type<Type> result;
    void * opaque;
    result.status = pmdaCacheLookupName(indom, name.c_str(), &result.instance_id, &opaque);
    if (result.status < 0) {
        throw pcp::exception(result.status);
    }
    if ((flags & require_active) && (result.status != PMDA_CACHE_ACTIVE)) {
        throw pcp::exception(result.status, "not active"); ///< @todo Better message.
    }
    result.name = NULL;
    result.opaque = static_cast<Type>(opaque);
    return result;
}

template <typename Type>
lookup_result_type<Type> lookup(const pmInDom indom, const std::string &name,
                                const std::string &key,
                                const lookup_flags flags = require_active)
{
    lookup_result_type<Type> result;
    void * opaque;
    result.status = pmdaCacheLookupKey(indom, name.c_str(), key.size(),
                                       key.c_str(), &result.name,
                                       &result.instance_id, &opaque);
    if (result.status < 0) {
        throw pcp::exception(result.status);
    }
    if ((flags & require_active) && (result.status != PMDA_CACHE_ACTIVE)) {
        throw pcp::exception(result.status, "not active"); ///< @todo Better message.
    }
    result.opaque = static_cast<Type>(opaque);
    return result;
}

size_t purge(const pmInDom indom, const time_t recent)
{
    const int result = pmdaCachePurge(indom, recent);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

#ifndef PCP_CPP_NO_BOOST
size_t purge(const pmInDom indom, const boost::posix_time::time_duration &recent)
{
    return purge(indom, recent.seconds());
}
#endif

instance_id_type store(const pmInDom indom, const std::string &name,
                       const int flags = PMDA_CACHE_ADD,
                       void * const opaque = NULL)
{
    const int result = pmdaCacheStore(indom, flags, name.c_str(), opaque);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

instance_id_type store(const pmInDom indom, const std::string &name,
                       void * const opaque, const int flags = PMDA_CACHE_ADD)
{
    const int result = pmdaCacheStore(indom, flags, name.c_str(), opaque);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

instance_id_type store(const pmInDom indom, const std::string &name,
                       const std::string &key, const int flags = 0,
                       void * const opaque = NULL)
{
    const int result = pmdaCacheStoreKey(indom, flags, name.c_str(), key.size(),
                                         key.c_str(), opaque);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

instance_id_type store(const pmInDom indom, const std::string &name,
                       const std::string &key, void * const opaque,
                       const int flags = 0)
{
    const int result = pmdaCacheStoreKey(indom, flags, name.c_str(), key.size(),
                                         key.c_str(), opaque);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

int perform(const pmInDom indom, const int operation)
{
    const int result = pmdaCacheOp(indom, operation);
    if (result < 0) {
        throw pcp::exception(result);
    }
    return result;
}

} } // pcp::cache namespace.

PCP_CPP_END_NAMESPACE

#endif
