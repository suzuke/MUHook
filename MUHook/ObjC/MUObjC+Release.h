//
//  MUObjC+Release.h
//  MUHook
//
//  Created by Magic-Unique on 2019/3/26.
//

#import "MUHookMacro.h"

#if DEBUG == 1
#else

#pragma mark - Main

#define MUHMain                                 static __attribute__((constructor)) initialize

#pragma mark - Quick Statement

#define _MUHClass(c)                            objc_getClass(#c)
#define MUHClass(c)                             _MUHClass(c)

#define _MUHMetaClass(c)                        objc_getMetaClass(#c)
#define MUHMetaClass(c)                         _MUHMetaClass(c)

#define MUHSendClassMsg(c, factory)             [MUHClass(c) factory]

#define MUHAlloc(c)                             ((c *)[MUHClass(c) alloc])

#define MUHAllocInitWith(c, init)               [MUHAlloc(c) init]

#define MUHInitClass(c)                         init_##c

#pragma mark - Implementation

#define _MUHPropertyImplementation(c, mm, type, name)\
MUHInstanceImplementation(c, get_##name, type) {\
return MUHSelfAsct(name, mm);\
}\
MUHInstanceImplementation(c, set_##name, void, type name) {\
MUHSelfAsct(name, mm) = name;\
}
#define MUHPropertyImplementation(c, mm, type, name) _MUHPropertyImplementation(c, mm, type, name)

#define _MUHInstanceImplementation(c, name, returnType, args...) \
__unused static returnType (*_unique_objc_ori$##c##$##name)   ( c * self, SEL _cmd, ##args ); \
__unused static returnType   _unique_objc_new$##c##$##name    ( c * self, SEL _cmd, ##args )
#define MUHInstanceImplementation(c, name, returnType, args...) _MUHInstanceImplementation(c, name, returnType, ##args)

#define _MUHClassImplementation(c, name, returnType, args...) \
__unused static returnType (*_unique_objc_ori$##c##$##name)   ( __strong const Class self, SEL _cmd, ##args ); \
__unused static returnType   _unique_objc_new$##c##$##name    ( __strong const Class self, SEL _cmd, ##args )
#define MUHClassImplementation(c, name, returnType, args...) _MUHClassImplementation(c, name, returnType, ##args)

#pragma mark - Execute Orig or Super

#define _MUHOrig(c, name, args...)          (!_unique_objc_ori$##c##$##name?0:_unique_objc_ori$##c##$##name (self, _cmd, ##args))
#define MUHOrig(c, name, args...)           _MUHOrig(c, name, ##args)

#define _MUHSuper(c, name, args...)         (!_unique_objc_ori$##c##$##name?0:_unique_objc_ori$##c##$##name (self, _cmd, ##args))
#define MUHSuper(c, name, args...)          _MUHSuper(c, name, ##args)

#pragma mark - Hook

#define _MUHHookInstanceMessage(c, name, sel) \
MUHookMessageEx(MUHClass(c), @selector(sel), (IMP)&_unique_objc_new$##c##$##name, (IMP*)&_unique_objc_ori$##c##$##name)
#define MUHHookInstanceMessage(c, name, sel) _MUHHookInstanceMessage(c, name, sel)

#define _MUHHookClassMessage(c, name, sel) \
MUHookMessageEx(MUHMetaClass(c), @selector(sel), (IMP)&_unique_objc_new$##c##$##name, (IMP*)&_unique_objc_ori$##c##$##name)
#define MUHHookClassMessage(c, name, sel) _MUHHookClassMessage(c, name, sel)

#pragma mark - Create

#define _MUHAddProperty(c, type, name, setter, getter) \
MUHAddInstanceMessage(c, set_##name, void, setter, type);\
MUHAddInstanceMessage(c, get_##name, type, getter);
#define MUHAddProperty(c, type, name, setter, getter) _MUHAddProperty(c, type, name, setter, getter)

#define _MUHAddInstanceMessage(c, name, ret, sel, args...) \
MUAddMessageEx(MUHClass(c), @selector(sel), (IMP)&_unique_objc_new$##c##$##name, MUHEncode(ret, id, SEL, ##args), (IMP*)&_unique_objc_ori$##c##$##name)
#define MUHAddInstanceMessage(c, name, ret, sel, args...) _MUHAddInstanceMessage(c, name, ret, sel, ##args)

#define _MUHAddClassMessage(c, name, ret, sel, args...) \
MUAddMessageEx(MUHMetaClass(c), @selector(sel), (IMP)&_unique_objc_new$##c##$##name, MUHEncode(ret, Class, SEL, ##args), (IMP*)&_unique_objc_ori$##c##$##name)
#define MUHAddClassMessage(c, name, ret, sel, args...) _MUHAddClassMessage(c, name, ret, sel, ##args)

#endif
