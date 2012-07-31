
//=============================================================================
//
// [IKM] 2012-07-29: placeholder for the map class
//
//=============================================================================
#ifndef __AGS_CN_UTIL__MAP_H
#define __AGS_CN_UTIL__MAP_H

namespace AGS
{
namespace Common
{
namespace Util
{

template<typename TKey, typename TValue> struct CMap
{
    template<typename TFirst, typename TSecond> struct CPair
    {
        TFirst fist;
        TSecond second;
    };

    CPair<TKey, TValue> * arr;
    int count;
    int capacity;

    bool Add(const TKey &key, const TValue &value) { return false; }
    void Set(const TKey &key, const TValue &value) { }
    bool Find(const TKey &key, TValue &value) { return false; }
};

} // namespace Util
} // namespace Common
} // namespace AGS

#endif // __AGS_CN_UTIL__MAP_H
