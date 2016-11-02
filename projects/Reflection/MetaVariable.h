#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "AutoLister.h"
#include "MetaType.h"

#ifndef META_VARIABLE
#define META_VARIABLE

#define META_REGISTER_VAR(v) \
	MetaVariable g_metaVariable_##v(#v, &v)

struct MetaVariable : public AutoLister<MetaVariable>
{
	template <typename Var>
	MetaVariable(const char* name_, Var* var)
		: name(name_)
		, address(var)
		, type(GetMetaType(*var))
	{}

	const char* const name;
	void* const address;
	const MetaType& type;
};

void MetaPrintVariables(std::ostream& os)
{
	for (MetaVariable* mv = MetaVariable::Head(); mv; mv = mv->Next())
		os << mv->type.Name() << " " << mv->name << " = " << mv->type.ToString(mv->address) << "\n";
}

#endif