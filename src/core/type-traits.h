#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

template <typename T>
struct TypeTraits;

template <typename T>
struct TypeTraits
{
  typedef T ReferencedType;
};

template <typename T>
struct TypeTraits<const T &>
{
  typedef T ReferencedType;
};

template <typename T>
struct TypeTraits<T &>
{
  typedef T ReferencedType;
};


#endif /* TYPE_TRAITS_H */
