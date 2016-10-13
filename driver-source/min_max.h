#ifndef MIN_MAX_H
#define MIN_MAX_H

template<typename T>
T min(T r, T l)
{
	if (r < l)
		return r;
	return l;
}

template<typename T>
T max(T r, T l)
{
	if (r > l)
		return r;
	return l;
}

template<typename T1, typename T2>
T1 min(T1 r, T2 l)
{
	if (r < l)
		return r;
	return l;
}

template<typename T1, typename T2>
T1 max(T1 r, T2 l)
{
	if (r > l)
		return r;
	return l;
}

#endif
