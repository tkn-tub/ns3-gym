#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

template <typename T>
struct TypeTraits
{
private:
  struct NullType {};
  template <typename U> struct UnConst
  {
    typedef U Result;
  };
  template <typename U> struct UnConst<const U>
  {
    typedef U Result;
  };
  template <typename U> struct ReferenceTraits
  {
    enum { IsReference = 0};
    typedef U ReferencedType;
  };
  template <typename U> struct ReferenceTraits<U&>
  {
    enum { IsReference = 1};
    typedef U ReferencedType;
  };
  template <typename U> struct PointerTraits
  {
    enum { IsPointer = 0};
    typedef U PointeeType;
  };
  template <typename U> struct PointerTraits<U *>
  {
    enum { IsPointer = 1};
    typedef U PointeeType;
  };
  template <typename U> struct FunctionPtrTraits
  {
    enum { IsFunctionPointer = 0};
  };
  template <typename U> 
  struct FunctionPtrTraits <U (*)(void)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 0};
    typedef U ReturnType;
  };
  template <typename U, typename V1> 
  struct FunctionPtrTraits <U (*)(V1)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 1};
    typedef U ReturnType;
    typedef V1 Arg1Type;
  };
  template <typename U, typename V1, typename V2> 
  struct FunctionPtrTraits <U (*)(V1,V2)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 2};
    typedef U ReturnType;
    typedef V1 Arg1Type;
    typedef V2 Arg2Type;
  };
  template <typename U, typename V1, typename V2,
            typename V3> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 3};
    typedef U ReturnType;
    typedef V1 Arg1Type;
    typedef V2 Arg2Type;
    typedef V3 Arg3Type;
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 4};
    typedef U ReturnType;
    typedef V1 Arg1Type;
    typedef V2 Arg2Type;
    typedef V3 Arg3Type;
    typedef V4 Arg4Type;
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4,
            typename V5> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4,V5)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 5};
    typedef U ReturnType;
    typedef V1 Arg1Type;
    typedef V2 Arg2Type;
    typedef V3 Arg3Type;
    typedef V4 Arg4Type;
    typedef V5 Arg5Type;
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4,
            typename V5, typename V6> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4,V5,V6)>
  {
    enum { IsFunctionPointer = 1};
    enum { nArgs = 6};
    typedef U ReturnType;
    typedef V1 Arg1Type;
    typedef V2 Arg2Type;
    typedef V3 Arg3Type;
    typedef V4 Arg4Type;
    typedef V5 Arg5Type;
    typedef V6 Arg6Type;
  };
  template <typename U> struct PtrToMemberTraits
  {
    enum { IsPointerToMember = 0};
  };
  template <typename U, typename V> 
  struct PtrToMemberTraits <U (V::*) (void)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 0};
    typedef U ReturnType;
  };
  template <typename U, typename V> 
  struct PtrToMemberTraits <U (V::*) (void) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 0};
    typedef U ReturnType;
  };
  template <typename U, typename V,typename W1> 
  struct PtrToMemberTraits <U (V::*) (W1)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 1};
    typedef U ReturnType;
    typedef W1 Arg1Type;
  };
  template <typename U, typename V,typename W1> 
  struct PtrToMemberTraits <U (V::*) (W1) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 1};
    typedef U ReturnType;
    typedef W1 Arg1Type;
  };
  template <typename U, typename V,typename W1, typename W2> 
  struct PtrToMemberTraits <U (V::*) (W1,W2)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 2};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
  };
  template <typename U, typename V,typename W1, typename W2> 
  struct PtrToMemberTraits <U (V::*) (W1,W2) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 2};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 3};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 3};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 4};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 4};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 5};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
    typedef W5 Arg5Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 5};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
    typedef W5 Arg5Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5, typename W6> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5,W6)>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 6};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
    typedef W5 Arg5Type;
    typedef W6 Arg6Type;
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5, typename W6> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5,W6) const>
  {
    enum { IsPointerToMember = 1};
    enum { nArgs = 6};
    typedef U ReturnType;
    typedef W1 Arg1Type;
    typedef W2 Arg2Type;
    typedef W3 Arg3Type;
    typedef W4 Arg4Type;
    typedef W5 Arg5Type;
    typedef W6 Arg6Type;
  };

public:
  typedef typename UnConst<T>::Result NonConstType;
  typedef typename ReferenceTraits<T>::ReferencedType ReferencedType;
  typedef typename PointerTraits<T>::PointeeType PointeeType;
  enum { IsPointerToMember = PtrToMemberTraits<T>::IsPointerToMember};
  enum { IsPointer = PointerTraits<T>::IsPointer};
  enum { IsReference = ReferenceTraits<T>::IsReference};
  enum { IsFunctionPointer = FunctionPtrTraits<T>::IsFunctionPointer};
  typedef PtrToMemberTraits<T> PointerToMemberTraits;
  typedef FunctionPtrTraits<T> FunctionPointerTraits;
};


#endif /* TYPE_TRAITS_H */
