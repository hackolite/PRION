/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkSetGet.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME SetGet Macros - standard macros for setting/getting instance variables
// .SECTION Description
// The SetGet macros are used to interface to instance variables
// in a standard fashion. This includes properly treating modified time
// and printing out debug information.
//
// Macros are available for built-in types; for character strings; 
// vector arrays of built-in types size 2,3,4; for setting objects; and
// debug, warning, and error printout information.

#ifndef __vtkSetGet_h
#define __vtkSetGet_h

#include "vtkSystemIncludes.h"
#include <math.h>

// A macro to get the name of a type
#define vtkImageScalarTypeNameMacro(type) \
(((type) == VTK_VOID) ? "void" : \
(((type) == VTK_BIT) ? "bit" : \
(((type) == VTK_CHAR) ? "char" : \
(((type) == VTK_SIGNED_CHAR) ? "signed char" : \
(((type) == VTK_UNSIGNED_CHAR) ? "unsigned char" : \
(((type) == VTK_SHORT) ? "short" : \
(((type) == VTK_UNSIGNED_SHORT) ? "unsigned short" : \
(((type) == VTK_INT) ? "int" : \
(((type) == VTK_UNSIGNED_INT) ? "unsigned int" : \
(((type) == VTK_LONG) ? "long" : \
(((type) == VTK_UNSIGNED_LONG) ? "unsigned long" : \
(((type) == VTK_LONG_LONG) ? "long long" : \
(((type) == VTK_UNSIGNED_LONG_LONG) ? "unsigned long long" : \
(((type) == VTK___INT64) ? "__int64" : \
(((type) == VTK_UNSIGNED___INT64) ? "unsigned __int64" : \
(((type) == VTK_FLOAT) ? "float" : \
(((type) == VTK_DOUBLE) ? "double" : \
(((type) == VTK_ID_TYPE) ? "idtype" : \
"Undefined"))))))))))))))))))
  
//
// Set built-in type.  Creates member Set"name"() (e.g., SetVisibility());
//
#define vtkSetMacro(name,type) \
virtual void Set##name (type _arg) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " #name " to " << _arg); \
  if (this->name != _arg) \
    { \
    this->name = _arg; \
    this->Modified(); \
    } \
  } 

//
// Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
//
#define vtkGetMacro(name,type) \
virtual type Get##name () { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " of " << this->name ); \
  return this->name; \
  } 


//
// Set character string.  Creates member Set"name"() 
// (e.g., SetFilename(char *));
//
#define vtkSetStringMacro(name) \
virtual void Set##name (const char* _arg) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to " << (_arg?_arg:"(null)") ); \
  if ( this->name == NULL && _arg == NULL) { return;} \
  if ( this->name && _arg && (!strcmp(this->name,_arg))) { return;} \
  if (this->name) { delete [] this->name; } \
  if (_arg) \
    { \
    size_t n = strlen(_arg) + 1; \
    char *cp1 =  new char[n]; \
    const char *cp2 = (_arg); \
    this->name = cp1; \
    do { *cp1++ = *cp2++; } while ( --n ); \
    } \
   else \
    { \
    this->name = NULL; \
    } \
  this->Modified(); \
  } 

//
// Get character string.  Creates member Get"name"() 
// (e.g., char *GetFilename());
//
#define vtkGetStringMacro(name) \
virtual char* Get##name () { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " of " << (this->name?this->name:"(null)")); \
  return this->name; \
  } 

//
// Set built-in type where value is constrained between min/max limits.
// Create member Set"name"() (eg., SetRadius()). #defines are 
// convenience for clamping open-ended values.
// The Get"name"MinValue() and Get"name"MaxValue() members return the
// min and max limits.
//
#define vtkSetClampMacro(name,type,min,max) \
virtual void Set##name (type _arg) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to " << _arg ); \
  if (this->name != (_arg<min?min:(_arg>max?max:_arg))) \
    { \
    this->name = (_arg<min?min:(_arg>max?max:_arg)); \
    this->Modified(); \
    } \
  } \
virtual type Get##name##MinValue () \
  { \
  return min; \
  } \
virtual type Get##name##MaxValue () \
  { \
  return max; \
  }

//
// This macro defines a body of set object macro. It can be used either in 
// the header file vtkSetObjectMacro or in the implementation one
// vtkSetObjectMacro. It sets the pointer to object; uses vtkObject 
// reference counting methodology. Creates method 
// Set"name"() (e.g., SetPoints()).
//
#define vtkSetObjectBodyMacro(name,type,args)                   \
  {                                                             \
  vtkDebugMacro(<< this->GetClassName() << " (" << this         \
                << "): setting " << #name " to " << args );     \
  if (this->name != args)                                       \
    {                                                           \
    type* tempSGMacroVar = this->name;                          \
    this->name = args;                                          \
    if (this->name != NULL) { this->name->Register(this); }     \
    if (tempSGMacroVar != NULL)                                 \
      {                                                         \
      tempSGMacroVar->UnRegister(this);                         \
      }                                                         \
    this->Modified();                                           \
    }                                                           \
  }

//
// Set pointer to object; uses vtkObject reference counting methodology.
// Creates method Set"name"() (e.g., SetPoints()). This macro should
// be used in the header file.
//
#define vtkSetObjectMacro(name,type)            \
virtual void Set##name (type* _arg)             \
  {                                             \
  vtkSetObjectBodyMacro(name,type,_arg);        \
  }

//
// Set pointer to object; uses vtkObject reference counting methodology.
// Creates method Set"name"() (e.g., SetPoints()). This macro should
// be used in the implementation file. You will also have to write
// prototype in the header file. The prototype should look like this:
// virtual void Set"name"("type" *);
//
// Please use vtkCxxSetObjectMacro not vtkSetObjectImplementationMacro.
// The first one is just for people who already used it.
#define vtkSetObjectImplementationMacro(class,name,type)        \
  vtkCxxSetObjectMacro(class,name,type)

#define vtkCxxSetObjectMacro(class,name,type)   \
void class::Set##name (type* _arg)              \
  {                                             \
  vtkSetObjectBodyMacro(name,type,_arg);        \
  }

//
// Get pointer to object.  Creates member Get"name" (e.g., GetPoints()).
// This macro should be used in the header file.
//
#define vtkGetObjectMacro(name,type)                                    \
virtual type *Get##name ()                                              \
  {                                                                     \
  vtkDebugMacro(<< this->GetClassName() << " (" << this                 \
                << "): returning " #name " address " << this->name );   \
  return this->name;                                                    \
  }

//
// Create members "name"On() and "name"Off() (e.g., DebugOn() DebugOff()).
// Set method must be defined to use this macro.
//
#define vtkBooleanMacro(name,type) \
virtual void name##On () { this->Set##name((type)1);}; \
virtual void name##Off () { this->Set##name((type)0);}

//
// Following set macros for vectors define two members for each macro.  The first 
// allows setting of individual components (e.g, SetColor(float,float,float)), 
// the second allows setting from an array (e.g., SetColor(float* rgb[3])).
// The macros vary in the size of the vector they deal with.
//
#define vtkSetVector2Macro(name,type) \
virtual void Set##name (type _arg1, type _arg2) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to (" << _arg1 << "," << _arg2 << ")"); \
  if ((this->name[0] != _arg1)||(this->name[1] != _arg2)) \
    { \
    this->name[0] = _arg1; \
    this->name[1] = _arg2; \
    this->Modified(); \
    } \
  }; \
void Set##name (type _arg[2]) \
  { \
  this->Set##name (_arg[0], _arg[1]); \
  } 

#define vtkGetVector2Macro(name,type) \
virtual type *Get##name () \
{ \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " pointer " << this->name); \
  return this->name; \
} \
virtual void Get##name (type &_arg1, type &_arg2) \
  { \
    _arg1 = this->name[0]; \
    _arg2 = this->name[1]; \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " = (" << _arg1 << "," << _arg2 << ")"); \
  }; \
virtual void Get##name (type _arg[2]) \
  { \
  this->Get##name (_arg[0], _arg[1]);\
  } 

#define vtkSetVector3Macro(name,type) \
virtual void Set##name (type _arg1, type _arg2, type _arg3) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to (" << _arg1 << "," << _arg2 << "," << _arg3 << ")"); \
  if ((this->name[0] != _arg1)||(this->name[1] != _arg2)||(this->name[2] != _arg3)) \
    { \
    this->name[0] = _arg1; \
    this->name[1] = _arg2; \
    this->name[2] = _arg3; \
    this->Modified(); \
    } \
  }; \
virtual void Set##name (type _arg[3]) \
  { \
  this->Set##name (_arg[0], _arg[1], _arg[2]);\
  } 

#define vtkGetVector3Macro(name,type) \
virtual type *Get##name () \
{ \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " pointer " << this->name); \
  return this->name; \
} \
virtual void Get##name (type &_arg1, type &_arg2, type &_arg3) \
  { \
    _arg1 = this->name[0]; \
    _arg2 = this->name[1]; \
    _arg3 = this->name[2]; \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " = (" << _arg1 << "," << _arg2 << "," << _arg3 << ")"); \
  }; \
virtual void Get##name (type _arg[3]) \
  { \
  this->Get##name (_arg[0], _arg[1], _arg[2]);\
  } 

#define vtkSetVector4Macro(name,type) \
virtual void Set##name (type _arg1, type _arg2, type _arg3, type _arg4) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to (" << _arg1 << "," << _arg2 << "," << _arg3 << "," << _arg4 << ")"); \
  if ((this->name[0] != _arg1)||(this->name[1] != _arg2)||(this->name[2] != _arg3)||(this->name[3] != _arg4)) \
    { \
    this->name[0] = _arg1; \
    this->name[1] = _arg2; \
    this->name[2] = _arg3; \
    this->name[3] = _arg4; \
    this->Modified(); \
    } \
  }; \
virtual void Set##name (type _arg[4]) \
  { \
  this->Set##name (_arg[0], _arg[1], _arg[2], _arg[3]);\
  } 


#define vtkGetVector4Macro(name,type) \
virtual type *Get##name () \
{ \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " pointer " << this->name); \
  return this->name; \
} \
virtual void Get##name (type &_arg1, type &_arg2, type &_arg3, type &_arg4) \
  { \
    _arg1 = this->name[0]; \
    _arg2 = this->name[1]; \
    _arg3 = this->name[2]; \
    _arg4 = this->name[3]; \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " = (" << _arg1 << "," << _arg2 << "," << _arg3 << "," << _arg4 << ")"); \
  }; \
virtual void Get##name (type _arg[4]) \
  { \
  this->Get##name (_arg[0], _arg[1], _arg[2], _arg[3]);\
  } 

#define vtkSetVector6Macro(name,type) \
virtual void Set##name (type _arg1, type _arg2, type _arg3, type _arg4, type _arg5, type _arg6) \
  { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): setting " << #name " to (" << _arg1 << "," << _arg2 << "," << _arg3 << "," << _arg4 << "," << _arg5 << "," << _arg6 << ")"); \
  if ((this->name[0] != _arg1)||(this->name[1] != _arg2)||(this->name[2] != _arg3)||(this->name[3] != _arg4)||(this->name[4] != _arg5)||(this->name[5] != _arg6)) \
    { \
    this->name[0] = _arg1; \
    this->name[1] = _arg2; \
    this->name[2] = _arg3; \
    this->name[3] = _arg4; \
    this->name[4] = _arg5; \
    this->name[5] = _arg6; \
    this->Modified(); \
    } \
  }; \
virtual void Set##name (type _arg[6]) \
  { \
  this->Set##name (_arg[0], _arg[1], _arg[2], _arg[3], _arg[4], _arg[5]);\
  } 

#define vtkGetVector6Macro(name,type) \
virtual type *Get##name () \
{ \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " pointer " << this->name); \
  return this->name; \
} \
virtual void Get##name (type &_arg1, type &_arg2, type &_arg3, type &_arg4, type &_arg5, type &_arg6) \
  { \
    _arg1 = this->name[0]; \
    _arg2 = this->name[1]; \
    _arg3 = this->name[2]; \
    _arg4 = this->name[3]; \
    _arg5 = this->name[4]; \
    _arg6 = this->name[5]; \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " = (" << _arg1 << "," << _arg2 << "," << _arg3 << "," << _arg4 << "," << _arg5 <<"," << _arg6 << ")"); \
  }; \
virtual void Get##name (type _arg[6]) \
  { \
  this->Get##name (_arg[0], _arg[1], _arg[2], _arg[3], _arg[4], _arg[5]);\
  } 

//
// General set vector macro creates a single method that copies specified
// number of values into object.
// Examples: void SetColor(c,3)
//
#define vtkSetVectorMacro(name,type,count) \
virtual void Set##name(type data[]) \
{ \
  int i; \
  for (i=0; i<count; i++) { if ( data[i] != this->name[i] ) { break; }} \
  if ( i < count ) \
    { \
    for (i=0; i<count; i++) { this->name[i] = data[i]; }\
    this->Modified(); \
    } \
}

//
// Get vector macro defines two methods. One returns pointer to type 
// (i.e., array of type). This is for efficiency. The second copies data
// into user provided array. This is more object-oriented.
// Examples: float *GetColor() and void GetColor(float c[count]).
//
#define vtkGetVectorMacro(name,type,count) \
virtual type *Get##name () \
{ \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " << #name " pointer " << this->name); \
  return this->name; \
} \
virtual void Get##name (type data[count]) \
{ \
  for (int i=0; i<count; i++) { data[i] = this->name[i]; }\
}

// Use a global function which actually calls:
//  vtkOutputWindow::GetInstance()->DisplayText();
// This is to avoid vtkObject #include of vtkOutputWindow
// while vtkOutputWindow #includes vtkObject

extern VTK_COMMON_EXPORT void vtkOutputWindowDisplayText(const char*);
extern VTK_COMMON_EXPORT void vtkOutputWindowDisplayErrorText(const char*);
extern VTK_COMMON_EXPORT void vtkOutputWindowDisplayWarningText(const char*);
extern VTK_COMMON_EXPORT void vtkOutputWindowDisplayGenericWarningText(const char*);
extern VTK_COMMON_EXPORT void vtkOutputWindowDisplayDebugText(const char*);

//
// This macro is used for any output that may not be in an instance method
// vtkGenericWarningMacro(<< "this is debug info" << this->SomeVariable);
//
#define vtkGenericWarningMacro(x) \
{ if (vtkObject::GetGlobalWarningDisplay()) { \
      vtkOStreamWrapper::EndlType endl; \
      vtkOStreamWrapper::UseEndl(endl); \
      vtkOStrStreamWrapper vtkmsg; \
      vtkmsg << "Generic Warning: In " __FILE__ ", line " << __LINE__ << "\n" x \
      << "\n\n"; \
      vtkOutputWindowDisplayGenericWarningText(vtkmsg.str());\
      vtkmsg.rdbuf()->freeze(0);}}

//
// This macro is used for  debug statements in instance methods
// vtkDebugMacro(<< "this is debug info" << this->SomeVariable);
//
#define vtkDebugMacro(x) \
   vtkDebugWithObjectMacro(this,x)

//
// This macro is used to print out warning messages.
// vtkWarningMacro(<< "Warning message" << variable);
//
#define vtkWarningMacro(x)                      \
   vtkWarningWithObjectMacro(this,x)

//
// This macro is used to print out errors
// vtkErrorMacro(<< "Error message" << variable);
//
#define vtkErrorMacro(x)                        \
   vtkErrorWithObjectMacro(this,x)

//
// This macro is used to print out errors
// vtkErrorWithObjectMacro(self, << "Error message" << variable);
//
#define vtkErrorWithObjectMacro(self, x)                        \
   {                                                            \
   if (vtkObject::GetGlobalWarningDisplay())                    \
     {                                                          \
     vtkOStreamWrapper::EndlType endl;                          \
     vtkOStreamWrapper::UseEndl(endl);                          \
     vtkOStrStreamWrapper vtkmsg;                               \
     vtkmsg << "ERROR: In " __FILE__ ", line " << __LINE__      \
            << "\n" << self->GetClassName() << " (" << self     \
            << "): " x << "\n\n";                               \
     if ( self->HasObserver("ErrorEvent") )                     \
       {                                                        \
       self->InvokeEvent("ErrorEvent", vtkmsg.str());           \
       }                                                        \
     else                                                       \
       {                                                        \
       vtkOutputWindowDisplayErrorText(vtkmsg.str());           \
       }                                                        \
     vtkmsg.rdbuf()->freeze(0); vtkObject::BreakOnError();      \
     }                                                          \
   }

//
// This macro is used to print out warnings
// vtkWarningWithObjectMacro(self, "Warning message" << variable);
//
#define vtkWarningWithObjectMacro(self, x)                      \
   {                                                            \
   if (vtkObject::GetGlobalWarningDisplay())                    \
     {                                                          \
     vtkOStreamWrapper::EndlType endl;                          \
     vtkOStreamWrapper::UseEndl(endl);                          \
     vtkOStrStreamWrapper vtkmsg;                               \
     vtkmsg << "Warning: In " __FILE__ ", line " << __LINE__    \
            << "\n" << self->GetClassName() << " (" << self     \
            << "): " x << "\n\n";                               \
     if ( self->HasObserver("WarningEvent") )                   \
       {                                                        \
       self->InvokeEvent("WarningEvent", vtkmsg.str());         \
       }                                                        \
     else                                                       \
       {                                                        \
       vtkOutputWindowDisplayWarningText(vtkmsg.str());         \
       }                                                        \
     vtkmsg.rdbuf()->freeze(0);                                 \
     }                                                          \
   }

#ifdef VTK_LEAN_AND_MEAN
# define vtkDebugWithObjectMacro(self, x)
#else
# define vtkDebugWithObjectMacro(self, x)                                     \
  {                                                                           \
  if (self->GetDebug() && vtkObject::GetGlobalWarningDisplay())               \
    {                                                                         \
    vtkOStreamWrapper::EndlType endl;                                         \
    vtkOStreamWrapper::UseEndl(endl);                                         \
    vtkOStrStreamWrapper vtkmsg;                                              \
    vtkmsg << "Debug: In " __FILE__ ", line " << __LINE__ << "\n"             \
           << self->GetClassName() << " (" << self << "): " x  << "\n\n";     \
    vtkOutputWindowDisplayDebugText(vtkmsg.str());                            \
    vtkmsg.rdbuf()->freeze(0);                                                \
    }                                                                         \
  }
#endif

//
// This macro is used to quiet compiler warnings about unused parameters
// to methods. Only use it when the parameter really shouldn't be used.
// Don't use it as a way to shut up the compiler while you take your
// sweet time getting around to implementing the method.
//
#define vtkNotUsed(x)

#define vtkWorldCoordinateMacro(name) \
virtual vtkCoordinate *Get##name##Coordinate () \
{ \
    vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " #name "Coordinate address " << this->name##Coordinate ); \
    return this->name##Coordinate; \
} \
virtual void Set##name(double x[3]) {this->Set##name(x[0],x[1],x[2]);}; \
virtual void Set##name(double x, double y, double z) \
{ \
    this->name##Coordinate->SetValue(x,y,z); \
} \
virtual double *Get##name() \
{ \
    return this->name##Coordinate->GetValue(); \
}

#define vtkViewportCoordinateMacro(name) \
virtual vtkCoordinate *Get##name##Coordinate () \
{ \
    vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " #name "Coordinate address " << this->name##Coordinate ); \
    return this->name##Coordinate; \
} \
virtual void Set##name(double x[2]) {this->Set##name(x[0],x[1]);}; \
virtual void Set##name(double x, double y) \
{ \
    this->name##Coordinate->SetValue(x,y); \
} \
virtual double *Get##name() \
{ \
    return this->name##Coordinate->GetValue(); \
}

// Macro used to determine whether a class is the same class or
// a subclass of the named class.
//
#define vtkTypeMacro(thisClass,superclass) \
  typedef superclass Superclass; \
  private: \
  virtual const char* GetClassNameInternal() const { return #thisClass; } \
  public: \
  static int IsTypeOf(const char *type) \
  { \
    if ( !strcmp(#thisClass,type) ) \
      { \
      return 1; \
      } \
    return superclass::IsTypeOf(type); \
  } \
  virtual int IsA(const char *type) \
  { \
    return this->thisClass::IsTypeOf(type); \
  } \
  static thisClass* SafeDownCast(vtkObjectBase *o) \
  { \
    if ( o && o->IsA(#thisClass) ) \
      { \
      return static_cast<thisClass *>(o); \
      } \
    return NULL;\
  } \
  protected: \
  virtual vtkObjectBase *NewInstanceInternal() const \
  { \
    return thisClass::New(); \
  } \
  public: \
  thisClass *NewInstance() const \
  { \
    return thisClass::SafeDownCast(this->NewInstanceInternal()); \
  }

// Version of vtkTypeMacro that adds the CollectRevisions method.
#define vtkTypeRevisionMacro(thisClass,superclass) \
  protected: \
  void CollectRevisions(ostream& os); \
  public: \
  vtkTypeMacro(thisClass,superclass)

// Macro to implement the standard CollectRevisions method.
#define vtkCxxRevisionMacro(thisClass, revision) \
  void thisClass::CollectRevisions(ostream& sos) \
  { \
    vtkOStreamWrapper os(sos); \
    this->Superclass::CollectRevisions(os); \
    os << #thisClass " " revision "\n"; \
  }

// Macro to implement the standard form of the New() method.
#define vtkStandardNewMacro(thisClass) \
  thisClass* thisClass::New() \
  { \
    vtkObject* ret = vtkObjectFactory::CreateInstance(#thisClass); \
    if(ret) \
      { \
      return static_cast<thisClass*>(ret); \
      } \
    return new thisClass; \
  } \
  vtkInstantiatorNewMacro(thisClass)

// Macro to implement the instantiator's wrapper around the New()
// method.  Use this macro if and only if vtkStandardNewMacro is not
// used by the class.
#define vtkInstantiatorNewMacro(thisClass) \
  extern vtkObject* vtkInstantiator##thisClass##New(); \
  vtkObject* vtkInstantiator##thisClass##New() \
  { \
    return thisClass::New(); \
  }

// The vtkTemplateMacro is used to centralize the set of types
// supported by Execute methods.  It also avoids duplication of long
// switch statement case lists.
//
// This version of the macro allows the template to take any number of
// arguments.  Example usage:
// switch(array->GetDataType())
//   {
//   vtkTemplateMacro(myFunc(static_cast<VTK_TT*>(data), arg2));
//   }
#define vtkTemplateMacroCase(typeN, type, call)     \
  case typeN: { typedef type VTK_TT; call; }; break
#define vtkTemplateMacro(call)                                              \
  vtkTemplateMacroCase(VTK_DOUBLE, double, call);                           \
  vtkTemplateMacroCase(VTK_FLOAT, float, call);                             \
  vtkTemplateMacroCase_ll(VTK_LONG_LONG, long long, call)                   \
  vtkTemplateMacroCase_ll(VTK_UNSIGNED_LONG_LONG, unsigned long long, call) \
  vtkTemplateMacroCase_si64(VTK___INT64, __int64, call)                     \
  vtkTemplateMacroCase_ui64(VTK_UNSIGNED___INT64, unsigned __int64, call)   \
  vtkTemplateMacroCase(VTK_ID_TYPE, vtkIdType, call);                       \
  vtkTemplateMacroCase(VTK_LONG, long, call);                               \
  vtkTemplateMacroCase(VTK_UNSIGNED_LONG, unsigned long, call);             \
  vtkTemplateMacroCase(VTK_INT, int, call);                                 \
  vtkTemplateMacroCase(VTK_UNSIGNED_INT, unsigned int, call);               \
  vtkTemplateMacroCase(VTK_SHORT, short, call);                             \
  vtkTemplateMacroCase(VTK_UNSIGNED_SHORT, unsigned short, call);           \
  vtkTemplateMacroCase(VTK_CHAR, char, call);                               \
  vtkTemplateMacroCase(VTK_SIGNED_CHAR, signed char, call);                 \
  vtkTemplateMacroCase(VTK_UNSIGNED_CHAR, unsigned char, call)

// Add "long long" to the template macro if it is enabled.
#if defined(VTK_TYPE_USE_LONG_LONG)
# define vtkTemplateMacroCase_ll(typeN, type, call) \
            vtkTemplateMacroCase(typeN, type, call);
#else
# define vtkTemplateMacroCase_ll(typeN, type, call)
#endif

// Add "__int64" to the template macro if it is enabled.
#if defined(VTK_TYPE_USE___INT64)
# define vtkTemplateMacroCase_si64(typeN, type, call) \
             vtkTemplateMacroCase(typeN, type, call);
#else
# define vtkTemplateMacroCase_si64(typeN, type, call)
#endif

// Add "unsigned __int64" to the template macro if it is enabled and
// can be converted to double.
#if defined(VTK_TYPE_USE___INT64) && defined(VTK_TYPE_CONVERT_UI64_TO_DOUBLE)
# define vtkTemplateMacroCase_ui64(typeN, type, call) \
             vtkTemplateMacroCase(typeN, type, call);
#else
# define vtkTemplateMacroCase_ui64(typeN, type, call)
#endif

// Legacy versions of vtkTemplateMacro:
#if !defined(VTK_LEGACY_REMOVE)
# define vtkTemplateMacro3(func, a1, a2, a3) \
          vtkTemplateMacro(func (a1, a2, a3))
# define vtkTemplateMacro4(func, a1, a2, a3, a4) \
          vtkTemplateMacro(func (a1, a2, a3, a4))
# define vtkTemplateMacro5(func, a1, a2, a3, a4, a5) \
          vtkTemplateMacro(func (a1, a2, a3, a4, a5))
# define vtkTemplateMacro6(func, a1, a2, a3, a4, a5, a6) \
          vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6))
# define vtkTemplateMacro7(func, a1, a2, a3, a4, a5, a6, a7) \
          vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7))
# define vtkTemplateMacro8(func, a1, a2, a3, a4, a5, a6, a7, a8) \
          vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7, a8))
# define vtkTemplateMacro9(func, a1, a2, a3, a4, a5, a6, a7, a8, a9) \
          vtkTemplateMacro(func (a1, a2, a3, a4, a5, a6, a7, a8, a9))
# define vtkTemplateMacro10(func,a1, a2, a3, a4, a5, a6, a7, a8, a9, a10) \
           vtkTemplateMacro(func(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10))
// These are legacy macros, do not add anymore. Use vtkTemplateMacro instead
#endif

// Old-style legacy code marker macro.
#if !defined(VTK_LEGACY_REMOVE) && !defined(VTK_LEAN_AND_MEAN)
#define VTK_LEGACY_METHOD(oldMethod,versionStringMadeLegacy) \
  vtkErrorMacro(<< #oldMethod \
                << " was obsoleted for version " << #versionStringMadeLegacy \
                << " and will be removed in a future version");
#else
#define VTK_LEGACY_METHOD(oldMethod,versionStringMadeLegacy)
#endif

//----------------------------------------------------------------------------
// Setup legacy code policy.

// Define VTK_LEGACY macro to mark legacy methods where they are
// declared in their class.  Example usage:
//
//   // @deprecated Replaced by MyOtherMethod() as of VTK 5.0.
//   VTK_LEGACY(void MyMethod());
#if defined(VTK_LEGACY_REMOVE)
  // Remove legacy methods completely.  Put a bogus declaration in
  // place to avoid stray semicolons because this is an error for some
  // compilers.  Using a class forward declaration allows any number
  // of repeats in any context without generating unique names.
# define VTK_LEGACY(method) class vtkLegacyMethodRemoved
#elif defined(VTK_LEGACY_SILENT) || defined(VTK_WRAPPING_CXX)
  // Provide legacy methods with no warnings.
# define VTK_LEGACY(method) method
#else
  // Setup compile-time warnings for uses of deprecated methods if
  // possible on this compiler.
# if defined(__GNUC__) && !defined(__INTEL_COMPILER) && (__GNUC__ > 3 || (__GNUC__ == 3 && __GNUC_MINOR__ >= 1))
#  define VTK_LEGACY(method) method __attribute__((deprecated))
# elif defined(_MSC_VER) && _MSC_VER >= 1300
#  define VTK_LEGACY(method) __declspec(deprecated) method
# else
#  define VTK_LEGACY(method) method
# endif
#endif

// Macros to create runtime deprecation warning messages in function
// bodies.  Example usage:
//
//   void vtkMyClass::MyOldMethod()
//   {
//     VTK_LEGACY_BODY(vtkMyClass::MyOldMethod, "VTK 5.0");
//   }
//
//   void vtkMyClass::MyMethod()
//   {
//     VTK_LEGACY_REPLACED_BODY(vtkMyClass::MyMethod, "VTK 5.0",
//                              vtkMyClass::MyOtherMethod);
//   }
#if defined(VTK_LEGACY_REMOVE) || defined(VTK_LEGACY_SILENT)
# define VTK_LEGACY_BODY(method, version)
# define VTK_LEGACY_REPLACED_BODY(method, version, replace)
#else
# define VTK_LEGACY_BODY(method, version) \
  vtkGenericWarningMacro(#method " was deprecated for " version " and will be removed in a future version.")
# define VTK_LEGACY_REPLACED_BODY(method, version, replace) \
  vtkGenericWarningMacro(#method " was deprecated for " version " and will be removed in a future version.  Use " #replace " instead.")
#endif

#endif
