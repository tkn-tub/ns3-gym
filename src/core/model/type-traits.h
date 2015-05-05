/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2007 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef TYPE_TRAITS_H
#define TYPE_TRAITS_H

/**
 * \file
 * \ingroup object
 * TypeTraits introspection template.
 */

/** Type trait reference values */
template <typename T>
struct TypeTraits
{
private:
  struct NullType {};                             //!< Null value type traits
  template <typename U> struct UnConst            //!< Non-const type
  {
    typedef U Result;                             //!< Non-const result type
  };
  template <typename U> struct UnConst<const U>   //!< Non-const template type traits
  {
    typedef U Result;                             //!< Non-const result type
  };
  template <typename U> struct ReferenceTraits    //!< Non-reference type traits
  {
    enum { IsReference = 0                        /**< Non-reference value */ };
    typedef U ReferencedType;                     //!< Non-referenced result type
  };
  template <typename U> struct ReferenceTraits<U&>//!< Reference type traits
  {
    enum { IsReference = 1                        /**< Reference value */ };
    typedef U ReferencedType;                     //!< Referenced result type
  };
  template <typename U> struct PointerTraits      //!< Non-pointer type traits
  {
    enum { IsPointer = 0};                        //!< Non-pointer value
    typedef U PointeeType;                        //!< Non-pointer result type
  };
  template <typename U> struct PointerTraits<U *> //!< Pointer type traits
  {
    enum { IsPointer = 1};                        //!< Pointer value
    typedef U PointeeType;                        //!< Pointer result type
  };
  template <typename U> struct FunctionPtrTraits  //!< Non-function pointer type traits
  {
    enum { IsFunctionPointer = 0};                //!< Non-function pointer value
    typedef NullType ReturnType;                  //!< Non-function pointer result type
  };
  template <typename U> 
  struct FunctionPtrTraits <U (*)(void)>          //!< Function pointer type traits
  {
    enum { IsFunctionPointer = 1};                //!< Function pointer value
    enum { nArgs = 0};                            //!< Number of arguments
    typedef U ReturnType;                         //!< Return type
  };
  template <typename U, typename V1> 
  struct FunctionPtrTraits <U (*)(V1)>            //!< Function pointer type traits
  {						                                   
    enum { IsFunctionPointer = 1};                //!< Function pointer value	   
    enum { nArgs = 1};				  //!< Number of arguments	   
    typedef U ReturnType;			  //!< Return type
    typedef V1 Arg1Type;                          //!< First argument type
  };
  template <typename U, typename V1, typename V2> 
  struct FunctionPtrTraits <U (*)(V1,V2)>         //!< Function pointer type traits
  {						                                   
    enum { IsFunctionPointer = 1};		  //!< Function pointer value	   
    enum { nArgs = 2};				  //!< Number of arguments	   
    typedef U ReturnType;			  //!< Return type
    typedef V1 Arg1Type;			  //!< First argument type	   
    typedef V2 Arg2Type;                          //!< Second argument type
  };
  template <typename U, typename V1, typename V2,
            typename V3> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3)>      //!< Function pointer type traits
  {						                                   
    enum { IsFunctionPointer = 1};		  //!< Function pointer value	   
    enum { nArgs = 3};				  //!< Number of arguments	   
    typedef U ReturnType;			  //!< Return type
    typedef V1 Arg1Type;			  //!< First argument type	   
    typedef V2 Arg2Type;			  //!< Second argument type	   
    typedef V3 Arg3Type;                          //!< Third argument type
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4)>   //!< Function pointer type traits
  {						                                   
    enum { IsFunctionPointer = 1};		  //!< Function pointer value	   
    enum { nArgs = 4};				  //!< Number of arguments	   
    typedef U ReturnType;			  //!< Return type
    typedef V1 Arg1Type;			  //!< First argument type	   
    typedef V2 Arg2Type;			  //!< Second argument type	   
    typedef V3 Arg3Type;			  //!< Third argument type	   
    typedef V4 Arg4Type;                          //!< Fourth argument type
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4,
            typename V5> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4,V5)>//!< Function pointer type traits
  {						                                   
    enum { IsFunctionPointer = 1};		  //!< Function pointer value	   
    enum { nArgs = 5};				  //!< Number of arguments	   
    typedef U ReturnType;			  //!< Return type
    typedef V1 Arg1Type;			  //!< First argument type	   
    typedef V2 Arg2Type;			  //!< Second argument type	   
    typedef V3 Arg3Type;			  //!< Third argument type	   
    typedef V4 Arg4Type;			  //!< Fourth argument type	   
    typedef V5 Arg5Type;                          //!< Fifth argument type
  };
  template <typename U, typename V1, typename V2,
            typename V3, typename V4,
            typename V5, typename V6> 
  struct FunctionPtrTraits <U (*)(V1,V2,V3,V4,V5,V6)>  //!< Function pointer type traits
  {						                                        
    enum { IsFunctionPointer = 1};		       //!< Function pointer value	   
    enum { nArgs = 6};				       //!< Number of arguments	   
    typedef U ReturnType;			       //!< Return type
    typedef V1 Arg1Type;			       //!< First argument type	   
    typedef V2 Arg2Type;			       //!< Second argument type	   
    typedef V3 Arg3Type;			       //!< Third argument type	   
    typedef V4 Arg4Type;			       //!< Fourth argument type	   
    typedef V5 Arg5Type;			       //!< Fifth argument type	   
    typedef V6 Arg6Type;                               //!< Sixth argument type
  };
  template <typename U> struct PtrToMemberTraits  //!< Pointer to member type traits
  {
    enum { IsPointerToMember = 0};                //!< Non-pointer to member value
  };
  template <typename U, typename V> 
  struct PtrToMemberTraits <U (V::*) (void)>      //!< Pointer to member type traits
  {
    enum { IsPointerToMember = 1};                //!< Pointer to member value
    enum { nArgs = 0};                            //!< Number of arguments
    typedef U ReturnType;                         //!< Return type
  };
  template <typename U, typename V> 
  struct PtrToMemberTraits <U (V::*) (void) const>//!< Pointer to const member type traits
  {						                                    
    enum { IsPointerToMember = 1};		  //!< Pointer to member value	    
    enum { nArgs = 0};				  //!< Number of arguments	    
    typedef U ReturnType;			  //!< Return type		    
  };
  template <typename U, typename V,typename W1> 
  struct PtrToMemberTraits <U (V::*) (W1)>        //!< Pointer to member type traits
  {						                                    
    enum { IsPointerToMember = 1};		  //!< Pointer to member value	    
    enum { nArgs = 1};				  //!< Number of arguments	    
    typedef U ReturnType;			  //!< Return type		    
    typedef W1 Arg1Type;                          //!< First argument type
  };
  template <typename U, typename V,typename W1> 
  struct PtrToMemberTraits <U (V::*) (W1) const>  //!< Pointer to const member type traits
  {						                                    	  
    enum { IsPointerToMember = 1};		  //!< Pointer to member value	    	  
    enum { nArgs = 1};				  //!< Number of arguments	    	  
    typedef U ReturnType;			  //!< Return type		    	  
    typedef W1 Arg1Type;                          //!< First argument type
  };
  template <typename U, typename V,typename W1, typename W2> 
  struct PtrToMemberTraits <U (V::*) (W1,W2)>     //!< Pointer to member type traits
  {						                                    
    enum { IsPointerToMember = 1};		  //!< Pointer to member value	    
    enum { nArgs = 2};				  //!< Number of arguments	    
    typedef U ReturnType;			  //!< Return type		    
    typedef W1 Arg1Type;			  //!< First argument type	    
    typedef W2 Arg2Type;			  //!< Second argument type	    
  };
  template <typename U, typename V,typename W1, typename W2> 
  struct PtrToMemberTraits <U (V::*) (W1,W2) const>  //!< Pointer to const member type traits
  {						                                       	  
    enum { IsPointerToMember = 1};		     //!< Pointer to member value	    	  
    enum { nArgs = 2};				     //!< Number of arguments	    	  
    typedef U ReturnType;			     //!< Return type		    	  
    typedef W1 Arg1Type;			     //!< First argument type		  
    typedef W2 Arg2Type;			     //!< Second argument type		  
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3)>  //!< Pointer to member type traits
  {						                                    
    enum { IsPointerToMember = 1};		  //!< Pointer to member value	    
    enum { nArgs = 3};				  //!< Number of arguments	    
    typedef U ReturnType;			  //!< Return type		    
    typedef W1 Arg1Type;			  //!< First argument type	    
    typedef W2 Arg2Type;			  //!< Second argument type	    
    typedef W3 Arg3Type;			  //!< Third argument type	    
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3) const>  //!< Pointer to const member type traits
  {							                                  	     
    enum { IsPointerToMember = 1};			//!< Pointer to member value	     
    enum { nArgs = 3};					//!< Number of arguments	    	     
    typedef U ReturnType;				//!< Return type		    	     
    typedef W1 Arg1Type;				//!< First argument type		     
    typedef W2 Arg2Type;				//!< Second argument type		     
    typedef W3 Arg3Type;				//!< Third argument type		     
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4)>  //!< Pointer to member type traits
  {						                                       
    enum { IsPointerToMember = 1};		     //!< Pointer to member value	    
    enum { nArgs = 4};				     //!< Number of arguments	    
    typedef U ReturnType;			     //!< Return type		    
    typedef W1 Arg1Type;			     //!< First argument type	    
    typedef W2 Arg2Type;			     //!< Second argument type	    
    typedef W3 Arg3Type;			     //!< Third argument type	    
    typedef W4 Arg4Type;			     //!< Fourth argument type	    
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4) const>  //!< Pointer to const member type traits
  {							                                     	
    enum { IsPointerToMember = 1};			   //!< Pointer to member value	     	
    enum { nArgs = 4};					   //!< Number of arguments	    	
    typedef U ReturnType;				   //!< Return type		    	
    typedef W1 Arg1Type;				   //!< First argument type		
    typedef W2 Arg2Type;				   //!< Second argument type		
    typedef W3 Arg3Type;				   //!< Third argument type		
    typedef W4 Arg4Type;				   //!< Fourth argument type		
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5)>  //!< Pointer to member type traits
  {							                                  
    enum { IsPointerToMember = 1};			//!< Pointer to member value      
    enum { nArgs = 5};					//!< Number of arguments	       
    typedef U ReturnType;				//!< Return type		       
    typedef W1 Arg1Type;				//!< First argument type	       
    typedef W2 Arg2Type;				//!< Second argument type	       
    typedef W3 Arg3Type;				//!< Third argument type	       
    typedef W4 Arg4Type;				//!< Fourth argument type	       
    typedef W5 Arg5Type;				//!< Fifth argument type	       
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5) const>  //!< Pointer to const member type traits
  {							                                        	   
    enum { IsPointerToMember = 1};			      //!< Pointer to member value	     	   
    enum { nArgs = 5};					      //!< Number of arguments	    	   
    typedef U ReturnType;				      //!< Return type		    	   
    typedef W1 Arg1Type;				      //!< First argument type		   
    typedef W2 Arg2Type;				      //!< Second argument type		   
    typedef W3 Arg3Type;				      //!< Third argument type		   
    typedef W4 Arg4Type;				      //!< Fourth argument type		   
    typedef W5 Arg5Type;				      //!< Fifth argument type		   
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5, typename W6> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5,W6)>  //!< Pointer to member type traits
  {							                                     
    enum { IsPointerToMember = 1};			   //!< Pointer to member value      
    enum { nArgs = 6};					   //!< Number of arguments	  
    typedef U ReturnType;				   //!< Return type		  
    typedef W1 Arg1Type;				   //!< First argument type	  
    typedef W2 Arg2Type;				   //!< Second argument type	  
    typedef W3 Arg3Type;				   //!< Third argument type	  
    typedef W4 Arg4Type;				   //!< Fourth argument type	  
    typedef W5 Arg5Type;				   //!< Fifth argument type	  
    typedef W6 Arg6Type;				   //!< Sixth argument type	  
  };
  template <typename U, typename V,
            typename W1, typename W2,
            typename W3, typename W4,
            typename W5, typename W6> 
  struct PtrToMemberTraits <U (V::*) (W1,W2,W3,W4,W5,W6) const>  //!< Pointer to const member type traits
  {								                                         
    enum { IsPointerToMember = 1};				 //!< Pointer to member value	      
    enum { nArgs = 6};						 //!< Number of arguments	    	      
    typedef U ReturnType;					 //!< Return type		    	      
    typedef W1 Arg1Type;					 //!< First argument type		      
    typedef W2 Arg2Type;					 //!< Second argument type		      
    typedef W3 Arg3Type;					 //!< Third argument type		      
    typedef W4 Arg4Type;					 //!< Fourth argument type		      
    typedef W5 Arg5Type;					 //!< Fifth argument type		      
    typedef W6 Arg6Type;					 //!< Sixth argument type		      
  };

public:
  typedef typename UnConst<T>::Result NonConstType;                    //!< Non-const type
  typedef typename ReferenceTraits<T>::ReferencedType ReferencedType;  //!< Referenced type
  typedef typename PointerTraits<T>::PointeeType PointeeType;          //!< Pointee type
  enum { IsPointerToMember = PtrToMemberTraits<T>::IsPointerToMember}; //!< Pointer to member predicate
  enum { IsPointer = PointerTraits<T>::IsPointer};                     //!< Pointer predicate
  enum { IsReference = ReferenceTraits<T>::IsReference};               //!< Reference predicate
  enum { IsFunctionPointer = FunctionPtrTraits<T>::IsFunctionPointer}; //!< Function pointer predicate
  typedef PtrToMemberTraits<T> PointerToMemberTraits;                  //!< Pointer to member traits type
  typedef FunctionPtrTraits<T> FunctionPointerTraits;                  //!< Function pointer traits
};


#endif /* TYPE_TRAITS_H */
