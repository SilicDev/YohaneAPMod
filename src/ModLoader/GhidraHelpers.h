#pragma once

template <typename TRet, typename T>
TRet CONCAT44(T a, T b)
{
    return (TRet)(((unsigned long long)a << 0x20) + (unsigned long long)b);
}
