#pragma once

namespace sead
{
class RuntimeTypeInfo;
}

// The RTTI methods are declarations until their binary implementations are recovered.
#define SEAD_RTTI_BASE( Class ) \
public: \
        static const sead::RuntimeTypeInfo* getRuntimeTypeInfoStatic(); \
        virtual const sead::RuntimeTypeInfo* getRuntimeTypeInfo() const;
