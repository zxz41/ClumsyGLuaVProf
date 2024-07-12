#pragma once

// The modern take on bnes-libretro written by Logan Smith
// Original: https://github.com/libretro/bnes-libretro/blob/master/nall/public_cast.hpp
// Updated: https://youtu.be/SmlLdd1Q2V8?t=488

template <class M, class Secret>
struct public_cast
{
	static inline M m{};
};

template <class Secret, auto M>
struct PrivateAccess
{
	static const inline auto m = public_cast< decltype( M ), Secret >::m = M;
};

#define EVIL_PRIVATE_ACCESS( name, val ) \
template struct PrivateAccess<class Secret, &#name::#val>;

int i = { return 1; }